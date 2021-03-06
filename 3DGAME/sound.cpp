//-----------------------------------------------
// [sound.cpp]
// サウンド処理
// 作者 Chicheng Zhang
//-----------------------------------------------
#include "sound.h"
#include "d3d.h"

#define STRINWARNING	_T("警告！")
//
// 静的メンバーの初期化
//
IXAudio2*				DSOUND::pXAudio2 = NULL;				// XAudio2オブジェクトへのインターフェイス
IXAudio2MasteringVoice*	DSOUND::pMasteringVoice = NULL;			// マスターボイス
IXAudio2SourceVoice*	DSOUND::pSourceVoices[SOUND_MAX] = {};	// ソースボイス
BYTE*					DSOUND::pAudioData[SOUND_MAX] = {};		// オーディオデータ
DWORD					DSOUND::dwAudioSize[SOUND_MAX] = {};	// オーディオデータサイズ

//
// サウンドファイル設定
//
SOUNDPARAM				DSOUND::sounds[SOUND_MAX] = {
	{ _T("data/BGM/intro.wav"), TRUE },
	{ _T("data/BGM/bgm.wav"), TRUE },
	{ _T("data/SE/click.wav"), FALSE },
	{ _T("data/SE/explosion.wav"), FALSE },
	{ _T("data/SE/hit.wav"), FALSE },
	{ _T("data/SE/pickup.wav"), FALSE },
	{ _T("data/SE/shot.wav"), FALSE },
	{ _T("data/SE/terrain.wav"), FALSE }
};

//
// サウンド処理の初期化
//
HRESULT DSOUND::Init(HWND hWnd)
{
	HRESULT hr;

	// COMライブラリの初期化
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio2オブジェクトの作成
	hr = XAudio2Create(&pXAudio2, 0);
	if (FAILED(hr))
	{
		// COMライブラリの終了処理
		CoUninitialize();
		return E_FAIL;
	}

	// マスターボイスの生成
	hr = pXAudio2->CreateMasteringVoice(&pMasteringVoice);
	if (FAILED(hr))
	{
		// XAudio2の解放
		SAFE_RELEASE(pXAudio2);

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
			return HRESULT_FROM_WIN32(GetLastError());

		// ファイルポインタを先頭に移動
		if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
			return HRESULT_FROM_WIN32(GetLastError());

		// WAVEファイルのチェック
		hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
		if (FAILED(hr))
			return hr;

		hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
		if (FAILED(hr))
			return hr;

		if (dwFiletype != 'EVAW')
			return S_FALSE;

		// フォーマットチェック
		hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
		if (FAILED(hr))
			return hr;

		hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
		if (FAILED(hr)) 
			return hr;

		// オーディオデータ読み込み
		hr = CheckChunk(hFile, 'atad', &dwAudioSize[nCntSound], &dwChunkPosition);
		if (FAILED(hr))
			return hr;

		pAudioData[nCntSound] = (BYTE*)malloc(dwAudioSize[nCntSound]);
		hr = ReadChunkData(hFile, pAudioData[nCntSound], dwAudioSize[nCntSound], dwChunkPosition);
		if (FAILED(hr))
			return hr;

		// ソースボイスの生成
		hr = pXAudio2->CreateSourceVoice(&pSourceVoices[nCntSound], &(wfx.Format));
		if (FAILED(hr))
			return hr;

		// オーディオバッファの登録
		buffer.AudioBytes = dwAudioSize[nCntSound];
		buffer.pAudioData = pAudioData[nCntSound];
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount = -sounds[nCntSound].bLoop;

		pSourceVoices[nCntSound]->SubmitSourceBuffer(&buffer);
	}

	return S_OK;
}

void DSOUND::Destroy(void)
{
	for (int nCntSound = 0; nCntSound < SOUND_MAX; nCntSound++)
	{
		if (pSourceVoices[nCntSound])
		{
			pSourceVoices[nCntSound]->Stop();

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

//
// サウンドをプレイする。
//
HRESULT DSOUND::Play(SOUNDLABEL label)
{
	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;

	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = dwAudioSize[label];
	buffer.pAudioData = pAudioData[label];
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = -sounds[label].bLoop;
	// 状態取得
	pSourceVoices[label]->GetState(&xa2state);

	if (xa2state.BuffersQueued != 0)
	{
		// 一時停止
		pSourceVoices[label]->Stop();

	}
	else {
		// オーディオバッファの登録
		pSourceVoices[label]->SubmitSourceBuffer(&buffer);
	}

	pSourceVoices[label]->Start();
	
	return S_OK;
}

//
// サウンドを停止する。
//
void DSOUND::Stop(SOUNDLABEL label)
{
	XAUDIO2_VOICE_STATE xa2state;

	// 状態取得
	pSourceVoices[label]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{
		// 再生中

		pSourceVoices[label]->Stop();
		pSourceVoices[label]->FlushSourceBuffers();
	}
}

//
// すべてのサウンドを停止する。
//
void DSOUND::StopAll(void)
{
	for (int nCntSound = 0; nCntSound < SOUND_MAX; nCntSound++)
	{
		if (pSourceVoices[nCntSound])
		{
			pSourceVoices[nCntSound]->Stop();
			pSourceVoices[nCntSound]->FlushSourceBuffers();
		}
	}
}

//
// チャンクデータをチェックする。
//
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

	// ファイルポインタを先頭に移動
	if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		return HRESULT_FROM_WIN32(GetLastError());

	while (hr == S_OK)
	{
		// チャンクの読み込み
		if (ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
			hr = HRESULT_FROM_WIN32(GetLastError());		
		
		// チャンクデータの読み込み
		if (ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
			hr = HRESULT_FROM_WIN32(GetLastError());

		switch (dwChunkType)
		{
		case 'FFIR':
			// ファイルタイプの読み込み
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
				hr = HRESULT_FROM_WIN32(GetLastError());
			
			break;

		default:
			// ファイルポインタをチャンクデータ分移動
			if (SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
				return HRESULT_FROM_WIN32(GetLastError());
		}

		// ポインター調整
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

//
// チャンクデータの読み込み
//
HRESULT DSOUND::ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	DWORD dwRead;

	// ファイルポインタを指定位置まで移動
	if (SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		return HRESULT_FROM_WIN32(GetLastError());
	
	// データの読み込み
	if (ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
		return HRESULT_FROM_WIN32(GetLastError());

	return S_OK;
}

