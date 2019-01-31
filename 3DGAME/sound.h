#ifndef __SOUND_H__
#define __SOUND_H__

#include "main.h"

// ������ɥե�����
typedef enum
{
	SOUND_BGM000 = 0,		// BGM0
	SOUND_BGM001,			// BGM1
	SOUND_SE_SHOT,		// ���k����
	SOUND_SE_EXPLOSION,	// ���k��
	SOUND_SE_COIN,		// �nͻ��0

	SOUND_MAX,
} SOUNDLABEL;

typedef struct
{
	LPCWSTR filename;	// �ե�������
	BOOL bLoop;			// ��`�פ��뤫�ɤ���
} SOUNDPARAM;

struct DSOUND {
	static SOUNDPARAM				sounds[SOUND_MAX];
	static IXAudio2*				pXAudio2;					// XAudio2���֥������ȤؤΥ��󥿩`�ե�����
	static IXAudio2MasteringVoice*	pMasteringVoice;			// �ޥ����`�ܥ���
	static IXAudio2SourceVoice*		pSourceVoices[SOUND_MAX];	// ���`���ܥ���
	static BYTE*					pAudioData[SOUND_MAX];		// ���`�ǥ����ǩ`��
	static DWORD					dwAudioSize[SOUND_MAX];		// ���`�ǥ����ǩ`��������

	static HRESULT				Init(HWND hWnd);
	static void					Destroy(void);
	static HRESULT				Play(SOUNDLABEL label);
	static void					Stop(SOUNDLABEL label);
	static void					StopAll(void);
	static HRESULT				CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	static HRESULT				ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);
};

#endif // !__SOUND_H__
