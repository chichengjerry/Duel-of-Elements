#include "sound.h"
#include "d3d.h"

#define STRINWARNING	_T("少御��")

IXAudio2*				DSOUND::pXAudio2 = NULL;					// XAudio2オブジェクトへのインタ�`フェイス
IXAudio2MasteringVoice*	DSOUND::pMasteringVoice = NULL;			// マスタ�`ボイス
IXAudio2SourceVoice*	DSOUND::pSourceVoices[SOUND_MAX] = {};	// ソ�`スボイス
BYTE*					DSOUND::pAudioData[SOUND_MAX] = {};	// オ�`ディオデ�`タ
DWORD					DSOUND::dwAudioSize[SOUND_MAX] = {};	// オ�`ディオデ�`タサイズ

SOUNDPARAM				DSOUND::sounds[SOUND_MAX] = {
	{_T("data/BGM/bgm000.wav"), true},		// BGM0
	{_T("data/BGM/bgm001.wav"), true},		// BGM1
	{_T("data/SE/shot000.wav"), false},			// �ｰk符咄
	{_T("data/SE/explosion000.wav"), false},	// 卯�k咄
	{_T("data/SE/coin000.wav"), false},			// コイン咄
};

HRESULT DSOUND::Init(HWND hWnd)
{
	HRESULT hr;

	// COMライブラリの兜豚晒
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio2オブジェクトの恬撹
	hr = XAudio2Create(&pXAudio2, 0);
	if (FAILED(hr))
	{
		MessageBox(hWnd, _T("XAudio2オブジェクトの恬撹に払�。�"), STRINWARNING, MB_ICONWARNING);

		// COMライブラリの�K阻�I尖
		CoUninitialize();
		return E_FAIL;
	}

	// マスタ�`ボイスの伏撹
	hr = pXAudio2->CreateMasteringVoice(&pMasteringVoice);
	if (FAILED(hr))
	{
		MessageBox(hWnd, _T("マスタ�`ボイスの伏撹に払�。�"), STRINWARNING, MB_ICONWARNING);

		if (pXAudio2)
		{
			// XAudio2オブジェクトの�_慧
			pXAudio2->Release();
			pXAudio2 = NULL;
		}

		// COMライブラリの�K阻�I尖
		CoUninitialize();

		return E_FAIL;
	}

	// サウンドデ�`タの兜豚晒
	for (int nCntSound = 0; nCntSound < SOUND_MAX; nCntSound++)
	{
		HANDLE hFile;
		DWORD dwChunkSize = 0;
		DWORD dwChunkPosition = 0;
		DWORD dwFiletype;
		WAVEFORMATEXTENSIBLE wfx;
		XAUDIO2_BUFFER buffer;

		// バッファのクリア
		memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

		// サウンドデ�`タファイルの伏撹
		hFile = CreateFile(sounds[nCntSound].filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(hWnd, _T("サウンドデ�`タファイルの伏撹に払�。�(1)"), STRINWARNING, MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}
		if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{// ファイルポインタを枠�^に卞��
			MessageBox(hWnd, _T("サウンドデ�`タファイルの伏撹に払�。�(2)"), STRINWARNING, MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}

		// WAVEファイルのチェック
		hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, _T("WAVEファイルのチェックに払�。�(1)"), STRINWARNING, MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, _T("WAVEファイルのチェックに払�。�(2)"), STRINWARNING, MB_ICONWARNING);
			return S_FALSE;
		}
		if (dwFiletype != 'EVAW')
		{
			MessageBox(hWnd, _T("WAVEファイルのチェックに払�。�(3)"), STRINWARNING, MB_ICONWARNING);
			return S_FALSE;
		}

		// フォ�`マットチェック
		hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, _T("フォ�`マットチェックに払�。�(1)"), STRINWARNING, MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, _T("フォ�`マットチェックに払�。�(2)"), STRINWARNING, MB_ICONWARNING);
			return S_FALSE;
		}

		// オ�`ディオデ�`タ�iみ�zみ
		hr = CheckChunk(hFile, 'atad', &dwAudioSize[nCntSound], &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, _T("オ�`ディオデ�`タ�iみ�zみに払�。�(1)"), STRINWARNING, MB_ICONWARNING);
			return S_FALSE;
		}
		pAudioData[nCntSound] = (BYTE*)malloc(dwAudioSize[nCntSound]);
		hr = ReadChunkData(hFile, pAudioData[nCntSound], dwAudioSize[nCntSound], dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, _T("オ�`ディオデ�`タ�iみ�zみに払�。�(2)"), STRINWARNING, MB_ICONWARNING);
			return S_FALSE;
		}

		// ソ�`スボイスの伏撹
		hr = pXAudio2->CreateSourceVoice(&pSourceVoices[nCntSound], &(wfx.Format));
		if (FAILED(hr))
		{
			MessageBox(hWnd, _T("ソ�`スボイスの伏撹に払�。�"), STRINWARNING, MB_ICONWARNING);
			return S_FALSE;
		}

		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes = dwAudioSize[nCntSound];
		buffer.pAudioData = pAudioData[nCntSound];
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount = 0;

		// オ�`ディオバッファの鞠�h
		pSourceVoices[nCntSound]->SubmitSourceBuffer(&buffer);
	}

	return S_OK;
}

void DSOUND::Destroy(void)
{
	// 匯�r唯峭
	for (int nCntSound = 0; nCntSound < SOUND_MAX; nCntSound++)
	{
		if (pSourceVoices[nCntSound])
		{
			// 匯�r唯峭
			pSourceVoices[nCntSound]->Stop(0);

			// ソ�`スボイスの篤��
			pSourceVoices[nCntSound]->DestroyVoice();
			pSourceVoices[nCntSound] = NULL;

			// オ�`ディオデ�`タの�_慧
			free(pAudioData[nCntSound]);
			pAudioData[nCntSound] = NULL;
		}
	}

	// マスタ�`ボイスの篤��
	pMasteringVoice->DestroyVoice();
	pMasteringVoice = NULL;

	SAFE_RELEASE(pXAudio2);

	// COMライブラリの�K阻�I尖
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

	// 彜�B函誼
	pSourceVoices[label]->GetState(&xa2state);

	if (xa2state.BuffersQueued != 0)
	{// 壅伏嶄
		// 匯�r唯峭
		pSourceVoices[label]->Stop(0);

		// オ�`ディオバッファの��茅
		pSourceVoices[label]->FlushSourceBuffers();
	}

	// オ�`ディオバッファの鞠�h
	pSourceVoices[label]->SubmitSourceBuffer(&buffer);

	// 壅伏
	pSourceVoices[label]->Start(0);

	return S_OK;
}

void DSOUND::Stop(SOUNDLABEL label)
{
	XAUDIO2_VOICE_STATE xa2state;

	// 彜�B函誼
	pSourceVoices[label]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// 壅伏嶄
		// 匯�r唯峭
		pSourceVoices[label]->Stop(0);

		// オ�`ディオバッファの��茅
		pSourceVoices[label]->FlushSourceBuffers();
	}
}

void DSOUND::StopAll(void)
{
	for (int nCntSound = 0; nCntSound < SOUND_MAX; nCntSound++)
	{
		if (pSourceVoices[nCntSound])
		{
			// 匯�r唯峭
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
	{// ファイルポインタを枠�^に卞��
		return HRESULT_FROM_WIN32(GetLastError());
	}

	while (hr == S_OK)
	{
		if (ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
		{// チャンクの�iみ�zみ
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if (ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
		{// チャンクデ�`タの�iみ�zみ
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch (dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
			{// ファイルタイプの�iみ�zみ
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if (SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// ファイルポインタをチャンクデ�`タ蛍卞��
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
// チャンクデ�`タの�iみ�zみ
//=============================================================================
HRESULT DSOUND::ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	DWORD dwRead;

	if (SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを峺協了崔まで卞��
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if (ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
	{// デ�`タの�iみ�zみ
		return HRESULT_FROM_WIN32(GetLastError());
	}

	return S_OK;
}

