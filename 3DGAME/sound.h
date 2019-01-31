#ifndef __SOUND_H__
#define __SOUND_H__

#include "main.h"

// е╡ежеєе╔е╒ебедеы
typedef enum
{
	SOUND_BGM000 = 0,		// BGM0
	SOUND_BGM001,			// BGM1
	SOUND_SE_SHOT,		// ПО░k╔ф╥Ї
	SOUND_SE_EXPLOSION,	// ▒м░k╥Ї
	SOUND_SE_COIN,		// ╨n═╗╥Ї0

	SOUND_MAX,
} SOUNDLABEL;

typedef struct
{
	LPCWSTR filename;	// е╒ебедеы├√
	BOOL bLoop;			// еый`е╫д╣дыдлд╔дждл
} SOUNDPARAM;

struct DSOUND {
	static SOUNDPARAM				sounds[SOUND_MAX];
	static IXAudio2*				pXAudio2;					// XAudio2еке╓е╕езепе╚д╪д╬едеєе┐й`е╒езеде╣
	static IXAudio2MasteringVoice*	pMasteringVoice;			// е▐е╣е┐й`е▄еде╣
	static IXAudio2SourceVoice*		pSourceVoices[SOUND_MAX];	// е╜й`е╣е▄еде╣
	static BYTE*					pAudioData[SOUND_MAX];		// екй`е╟егеке╟й`е┐
	static DWORD					dwAudioSize[SOUND_MAX];		// екй`е╟егеке╟й`е┐е╡еде║

	static HRESULT				Init(HWND hWnd);
	static void					Destroy(void);
	static HRESULT				Play(SOUNDLABEL label);
	static void					Stop(SOUNDLABEL label);
	static void					StopAll(void);
	static HRESULT				CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	static HRESULT				ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);
};

#endif // !__SOUND_H__
