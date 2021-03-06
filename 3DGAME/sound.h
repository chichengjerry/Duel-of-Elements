//-----------------------------------------------
// [sound.h]
// サウンド処理
// 作者 Chicheng Zhang
//-----------------------------------------------
#ifndef __SOUND_H__
#define __SOUND_H__

#include "main.h"

//
// サウンドファイルリスト
//
typedef enum
{
	SOUND_BGM_TITLE,
	SOUND_BGM_GAME,
	SOUND_CLICK,
	SOUND_EXPLOSION,
	SOUND_HIT,
	SOUND_PICKUP,
	SOUND_SHOT,
	SOUND_TERRAIN,

	SOUND_MAX,
} SOUNDLABEL;

typedef struct
{
	LPCWSTR filename;	// ファイル名
	BOOL bLoop;			// ループするかどうか
} SOUNDPARAM;

struct DSOUND {
	static SOUNDPARAM				sounds[SOUND_MAX];
	static IXAudio2*				pXAudio2;
	static IXAudio2MasteringVoice*	pMasteringVoice;
	static IXAudio2SourceVoice*		pSourceVoices[SOUND_MAX];
	static BYTE*					pAudioData[SOUND_MAX];
	static DWORD					dwAudioSize[SOUND_MAX];

	static HRESULT					Init(HWND hWnd);
	static void						Destroy(void);
	static HRESULT					Play(SOUNDLABEL label);
	static void						Stop(SOUNDLABEL label);
	static void						StopAll(void);
	static HRESULT					CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	static HRESULT					ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);
};

#endif // !__SOUND_H__
