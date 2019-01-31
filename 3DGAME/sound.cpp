#include "sound.h"
#include "d3d.h"

#define STRINWARNING	_T("警告！")

IXAudio2*				DSOUND::pXAudio2 = NULL;					// XAudio2オブジェクトへのインターフェイス
IXAudio2MasteringVoice*	DSOUND::pMasteringVoice = NULL;			// マスターボイス
IXAudio2SourceVoice*	DSOUND::pSourceVoices[SOUND_MAX] = {};	// ソースボイス
BYTE*					DSOUND::pAudioData[SOUND_MAX] = {};	// オーディオデータ
DWORD					DSOUND::dwAudioSize[SOUND_MAX] = {};	// オーディオデータサイズ

SOUNDPARAM				DSOUND::sounds[SOUND_MAX] = {
	{_T("data/BGM/bgm000.wav"), true},		// BGM0
	{_T("data/BGM/bgm001.wav"), true},		// BGM1
	{_T("data/SE/shot000.wav"), false},			// 弾発射音
	{_T("data/SE/explosion000.wav"), false},	// 爆発音
	{_T("data/SE/coin000.wav"), false},			// コイン音
};

HRESULT DSOUND::Init(HWND hWnd)
{
	HRESULT hr;

	// COMライブラリの初期化
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio2オブジェクトの作成
	hr = XAudio2Create(&pXAudio2, 0);
	if (FAILED(hr))
	{
		MessageBox(hWnd, _T("XAudio2オブジェクトの作成に失敗！"), STRINWARNING, MB_ICONWARNING);

		// COMライブラリの終了処理
		CoUninitialize();
		return E_FAIL;
	}

	// マスターボイスの生成
	hr = pXAudio2->CreateMasteringVoice(&pMasteringVoice);
	if (FAILED(hr))
	{
		MessageBox(hWnd, _T("マスターボイスの生成に失敗！"), STRINWARNING, MB_ICONWARNING);

		if (pXAudio2)
		{
			// XAudio2オブジェクトの開放
			pXAudio2->Release();
			pXAudio2 = NULL;
		}

		// COMライブラリの終了処理
		CoUninitialize();

		return E_FAIL;
	}

	// サウンドデータの初期化
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

		// サウンドデータファイルの生成
		hFile = CreateFile(sounds[nCntSound].filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(hWnd, _T("サウンドデータファイルの生成に失敗！(1)"), STRINWARNING, MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}
		if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{// ファイルポインタを先頭に移動
			MessageBox(hWnd, _T("サウンドデータファイルの生成に失敗！(2)"), STRINWARNING, MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}

		// WAVEファイルのチェック
		hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, _T("WAVEファイルのチェックに失敗！(1)"), STRINWARNING, MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, _T("WAVEファイルのチェックに失敗！(2)"), STRINWARNING, MB_ICONWARNING);
			return S_FALSE;
		}
		if (dwFiletype != 'EVAW')
		{
			MessageBox(hWnd, _T("WAVEファイルのチェックに失敗！(3)"), STRINWARNING, MB_ICONWARNING);
			return S_FALSE;
		}

		// フォーマットチェック
		hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, _T("フォーマットチェックに失敗！(1)"), STRINWARNING, MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, _T("フォーマットチェックに失敗！(2)"), STRINWARNING, MB_ICONWARNING);
			return S_FALSE;
		}

		// オーディオデータ読み込み
		hr = CheckChunk(hFile, 'atad', &dwAudioSize[nCntSound], &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, _T("オーディオデータ読み込みに失敗！(1)"), STRINWARNING, MB_ICONWARNING);
			return S_FALSE;
		}
		pAudioData[nCntSound] = (BYTE*)malloc(dwAudioSize[nCntSound]);
		hr = ReadChunkData(hFile, pAudioData[nCntSound], dwAudioSize[nCntSound], dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, _T("オーディオデータ読み込みに失敗！(2)"), STRINWARNING, MB_ICONWARNING);
			return S_FALSE;
		}

		// ソースボイスの生成
		hr = pXAudio2->CreateSourceVoice(&pSourceVoices[nCntSound], &(wfx.Format));
		if (FAILED(hr))
		{
			MessageBox(hWnd, _T("ソースボイスの生成に失敗！"), STRINWARNING, MB_ICONWARNING);
			return S_FALSE;
		}

		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes = dwAudioSize[nCntSound];
		buffer.pAudioData = pAudioData[nCntSound];
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount = 0;

		// オーディオバッファの登録
		pSourceVoices[nCntSound]->SubmitSourceBuffer(&buffer);
	}

	return S_OK;
}

void DSOUND::Destroy(void)
{
	// 一時停止
	for (int nCntSound = 0; nCntSound < SOUND_MAX; nCntSound++)
	{
		if (pSourceVoices[nCntSound])
		{
			// 一時停止
			pSourceVoices[nCntSound]->Stop(0);

			// ソースボイスの破棄
			pSourceVoices[nCntSound]->DestroyVoice();
			pSourceVoices[nCntSound] = NULL;

			// オーディオデータの開放
			free(pAudioData[nCntSound]);
			pAudioData[nCntSound] = NULL;
		}
	}

	// マスターボイスの破棄
	pMasteringVoice->DestroyVoice();
	pMasteringVoice = NULL;

	SAFE_RELEASE(pXAudio2);

	// COMライブラリの終了処理
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

	// 状態取得
	pSourceVoices[label]->GetState(&xa2state);

	if (xa2state.BuffersQueued != 0)
	{// 再生中
		// 一時停止
		pSourceVoices[label]->Stop(0);

		// オーディオバッファの削除
		pSourceVoices[label]->FlushSourceBuffers();
	}

	// オーディオバッファの登録
	pSourceVoices[label]->SubmitSourceBuffer(&buffer);

	// 再生
	pSourceVoices[label]->Start(0);

	return S_OK;
}

void DSOUND::Stop(SOUNDLABEL label)
{
	XAUDIO2_VOICE_STATE xa2state;

	// 状態取得
	pSourceVoices[label]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// 再生中
		// 一時停止
		pSourceVoices[label]->Stop(0);

		// オーディオバッファの削除
		pSourceVoices[label]->FlushSourceBuffers();
	}
}

void DSOUND::StopAll(void)
{
	for (int nCntSound = 0; nCntSound < SOUND_MAX; nCntSound++)
	{
		if (pSourceVoices[nCntSound])
		{
			// 一時停止
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
	{// ファイルポインタを先頭に移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	while (hr == S_OK)
	{
		if (ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
		{// チャンクの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if (ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
		{// チャンクデータの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch (dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
			{// ファイルタイプの読み込み
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if (SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// ファイルポインタをチャンクデータ分移動
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
// チャンクデータの読み込み
//=============================================================================
HRESULT DSOUND::ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	DWORD dwRead;

	if (SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを指定位置まで移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if (ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
	{// データの読み込み
		return HRESULT_FROM_WIN32(GetLastError());
	}

	return S_OK;
}

