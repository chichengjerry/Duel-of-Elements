#ifndef __SOUND_H__
#define __SOUND_H__

#include "main.h"

// サウンドファイル
typedef enum
{
	SOUND_BGM000 = 0,		// BGM0
	SOUND_BGM001,			// BGM1
	SOUND_SE_SHOT,		// �ｰk符咄
	SOUND_SE_EXPLOSION,	// 卯�k咄
	SOUND_SE_COIN,		// �n融咄0

	SOUND_MAX,
} SOUNDLABEL;

typedef struct
{
	LPCWSTR filename;	// ファイル兆
	BOOL bLoop;			// ル�`プするかどうか
} SOUNDPARAM;

struct DSOUND {
	static SOUNDPARAM				sounds[SOUND_MAX];
	static IXAudio2*				pXAudio2;					// XAudio2オブジェクトへのインタ�`フェイス
	static IXAudio2MasteringVoice*	pMasteringVoice;			// マスタ�`ボイス
	static IXAudio2SourceVoice*		pSourceVoices[SOUND_MAX];	// ソ�`スボイス
	static BYTE*					pAudioData[SOUND_MAX];		// オ�`ディオデ�`タ
	static DWORD					dwAudioSize[SOUND_MAX];		// オ�`ディオデ�`タサイズ

	static HRESULT				Init(HWND hWnd);
	static void					Destroy(void);
	static HRESULT				Play(SOUNDLABEL label);
	static void					Stop(SOUNDLABEL label);
	static void					StopAll(void);
	static HRESULT				CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	static HRESULT				ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);
};

#endif // !__SOUND_H__
