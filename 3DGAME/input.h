#ifndef __INPUT_H__
#define __INPUT_H__

#include "main.h"

#define NUM_KEY_MAX				256

struct DINPUT {

	static LPDIRECTINPUT8	   	pDInput;							// IDirectInput8インターフェースへのポインタ
	static LPDIRECTINPUTDEVICE8	pDIDevKeyboard;						// IDirectInputDevice8インターフェースへのポインタ(キーボード)
	static BYTE				   	keyStates[NUM_KEY_MAX];				// キーボードの押下状態を保持するワーク
	static BYTE				   	keyStatesTrigger[NUM_KEY_MAX];		// キーボードのトリガー状態を保持するワーク
	static BYTE				   	keyStatesRelease[NUM_KEY_MAX];		// キーボードのリリース状態を保持するワーク
	static BYTE				   	keyStatesRepeat[NUM_KEY_MAX];		// キーボードのリピート状態を保持するワーク
	static int				   	keyStatesRepeatCnt[NUM_KEY_MAX];	// キーボードのリピートカウンタ

	static BOOL					GetKeyboardPress(BYTE key);
	static BOOL					GetKeyboardTrigger(BYTE key);
	static BOOL					GetKeyboardRepeat(BYTE key);
	static BOOL					GetKeyboardRelease(BYTE key);

	static HRESULT				Init(HINSTANCE hInst, HWND hWnd);
	static void					Destroy(void);
	static HRESULT				Update(void);
};

#endif // !__INPUT_H__
