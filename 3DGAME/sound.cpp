#include "sound.h"
#include "d3d.h"

#define STRINWARNING	_T("���棡")

IXAudio2*				DSOUND::pXAudio2 = NULL;					// XAudio2���֥������ȤؤΥ��󥿩`�ե�����
IXAudio2MasteringVoice*	DSOUND::pMasteringVoice = NULL;			// �ޥ����`�ܥ���
IXAudio2SourceVoice*	DSOUND::pSourceVoices[SOUND_MAX] = {};	// ���`���ܥ���
BYTE*					DSOUND::pAudioData[SOUND_MAX] = {};	// ���`�ǥ����ǩ`��
DWORD					DSOUND::dwAudioSize[SOUND_MAX] = {};	// ���`�ǥ����ǩ`��������

SOUNDPARAM				DSOUND::sounds[SOUND_MAX] = {
	{_T("data/BGM/bgm000.wav"), true},		// BGM0
	{_T("data/BGM/bgm001.wav"), true},		// BGM1
	{_T("data/SE/shot000.wav"), false},			// ���k����
	{_T("data/SE/explosion000.wav"), false},	// ���k��
	{_T("data/SE/coin000.wav"), false},			// ��������
};

HRESULT DSOUND::Init(HWND hWnd)
{
	HRESULT hr;

	// COM�饤�֥��γ��ڻ�
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio2���֥������Ȥ�����
	hr = XAudio2Create(&pXAudio2, 0);
	if (FAILED(hr))
	{
		MessageBox(hWnd, _T("XAudio2���֥������Ȥ����ɤ�ʧ����"), STRINWARNING, MB_ICONWARNING);

		// COM�饤�֥��νK�˄I��
		CoUninitialize();
		return E_FAIL;
	}

	// �ޥ����`�ܥ���������
	hr = pXAudio2->CreateMasteringVoice(&pMasteringVoice);
	if (FAILED(hr))
	{
		MessageBox(hWnd, _T("�ޥ����`�ܥ��������ɤ�ʧ����"), STRINWARNING, MB_ICONWARNING);

		if (pXAudio2)
		{
			// XAudio2���֥������Ȥ��_��
			pXAudio2->Release();
			pXAudio2 = NULL;
		}

		// COM�饤�֥��νK�˄I��
		CoUninitialize();

		return E_FAIL;
	}

	// ������ɥǩ`���γ��ڻ�
	for (int nCntSound = 0; nCntSound < SOUND_MAX; nCntSound++)
	{
		HANDLE hFile;
		DWORD dwChunkSize = 0;
		DWORD dwChunkPosition = 0;
		DWORD dwFiletype;
		WAVEFORMATEXTENSIBLE wfx;
		XAUDIO2_BUFFER buffer;

		// �Хåե��Υ��ꥢ
		memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

		// ������ɥǩ`���ե����������
		hFile = CreateFile(sounds[nCntSound].filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(hWnd, _T("������ɥǩ`���ե���������ɤ�ʧ����(1)"), STRINWARNING, MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}
		if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{// �ե�����ݥ��󥿤����^���Ƅ�
			MessageBox(hWnd, _T("������ɥǩ`���ե���������ɤ�ʧ����(2)"), STRINWARNING, MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}

		// WAVE�ե�����Υ����å�
		hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, _T("WAVE�ե�����Υ����å���ʧ����(1)"), STRINWARNING, MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, _T("WAVE�ե�����Υ����å���ʧ����(2)"), STRINWARNING, MB_ICONWARNING);
			return S_FALSE;
		}
		if (dwFiletype != 'EVAW')
		{
			MessageBox(hWnd, _T("WAVE�ե�����Υ����å���ʧ����(3)"), STRINWARNING, MB_ICONWARNING);
			return S_FALSE;
		}

		// �ե��`�ޥåȥ����å�
		hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, _T("�ե��`�ޥåȥ����å���ʧ����(1)"), STRINWARNING, MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, _T("�ե��`�ޥåȥ����å���ʧ����(2)"), STRINWARNING, MB_ICONWARNING);
			return S_FALSE;
		}

		// ���`�ǥ����ǩ`���i���z��
		hr = CheckChunk(hFile, 'atad', &dwAudioSize[nCntSound], &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, _T("���`�ǥ����ǩ`���i���z�ߤ�ʧ����(1)"), STRINWARNING, MB_ICONWARNING);
			return S_FALSE;
		}
		pAudioData[nCntSound] = (BYTE*)malloc(dwAudioSize[nCntSound]);
		hr = ReadChunkData(hFile, pAudioData[nCntSound], dwAudioSize[nCntSound], dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, _T("���`�ǥ����ǩ`���i���z�ߤ�ʧ����(2)"), STRINWARNING, MB_ICONWARNING);
			return S_FALSE;
		}

		// ���`���ܥ���������
		hr = pXAudio2->CreateSourceVoice(&pSourceVoices[nCntSound], &(wfx.Format));
		if (FAILED(hr))
		{
			MessageBox(hWnd, _T("���`���ܥ��������ɤ�ʧ����"), STRINWARNING, MB_ICONWARNING);
			return S_FALSE;
		}

		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes = dwAudioSize[nCntSound];
		buffer.pAudioData = pAudioData[nCntSound];
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount = 0;

		// ���`�ǥ����Хåե��ε��h
		pSourceVoices[nCntSound]->SubmitSourceBuffer(&buffer);
	}

	return S_OK;
}

void DSOUND::Destroy(void)
{
	// һ�rֹͣ
	for (int nCntSound = 0; nCntSound < SOUND_MAX; nCntSound++)
	{
		if (pSourceVoices[nCntSound])
		{
			// һ�rֹͣ
			pSourceVoices[nCntSound]->Stop(0);

			// ���`���ܥ������Ɨ�
			pSourceVoices[nCntSound]->DestroyVoice();
			pSourceVoices[nCntSound] = NULL;

			// ���`�ǥ����ǩ`�����_��
			free(pAudioData[nCntSound]);
			pAudioData[nCntSound] = NULL;
		}
	}

	// �ޥ����`�ܥ������Ɨ�
	pMasteringVoice->DestroyVoice();
	pMasteringVoice = NULL;

	SAFE_RELEASE(pXAudio2);

	// COM�饤�֥��νK�˄I��
	CoUninitialize();
}

HRESULT DSOUND::Play(SOUNDLABEL label)
{
	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;

	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = dwAudioSize[label];
	buffer.pAudioData = pAudioData[label];
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = 0;

	// ״�Bȡ��
	pSourceVoices[label]->GetState(&xa2state);

	if (xa2state.BuffersQueued != 0)
	{// ������
		// һ�rֹͣ
		pSourceVoices[label]->Stop(0);

		// ���`�ǥ����Хåե�������
		pSourceVoices[label]->FlushSourceBuffers();
	}

	// ���`�ǥ����Хåե��ε��h
	pSourceVoices[label]->SubmitSourceBuffer(&buffer);

	// ����
	pSourceVoices[label]->Start(0);

	return S_OK;
}

void DSOUND::Stop(SOUNDLABEL label)
{
	XAUDIO2_VOICE_STATE xa2state;

	// ״�Bȡ��
	pSourceVoices[label]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// ������
		// һ�rֹͣ
		pSourceVoices[label]->Stop(0);

		// ���`�ǥ����Хåե�������
		pSourceVoices[label]->FlushSourceBuffers();
	}
}

void DSOUND::StopAll(void)
{
	for (int nCntSound = 0; nCntSound < SOUND_MAX; nCntSound++)
	{
		if (pSourceVoices[nCntSound])
		{
			// һ�rֹͣ
			pSourceVoices[nCntSound]->Stop(0);
		}
	}
}
HRESULT DSOUND::CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition)
{
	HRESULT hr = S_OK;
	DWORD dwRead;
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD dwBytesRead = 0;
	DWORD dwOffset = 0;

	if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// �ե�����ݥ��󥿤����^���Ƅ�
		return HRESULT_FROM_WIN32(GetLastError());
	}

	while (hr == S_OK)
	{
		if (ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
		{// ����󥯤��i���z��
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if (ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
		{// ����󥯥ǩ`�����i���z��
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch (dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
			{// �ե����륿���פ��i���z��
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if (SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// �ե�����ݥ��󥿤����󥯥ǩ`�����Ƅ�
				return HRESULT_FROM_WIN32(GetLastError());
			}
		}

		dwOffset += sizeof(DWORD) * 2;
		if (dwChunkType == format)
		{
			*pChunkSize = dwChunkDataSize;
			*pChunkDataPosition = dwOffset;

			return S_OK;
		}

		dwOffset += dwChunkDataSize;
		if (dwBytesRead >= dwRIFFDataSize)
		{
			return S_FALSE;
		}
	}

	return S_OK;
}

//=============================================================================
// ����󥯥ǩ`�����i���z��
//=============================================================================
HRESULT DSOUND::ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	DWORD dwRead;

	if (SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// �ե�����ݥ��󥿤�ָ��λ�äޤ��Ƅ�
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if (ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
	{// �ǩ`�����i���z��
		return HRESULT_FROM_WIN32(GetLastError());
	}

	return S_OK;
}

