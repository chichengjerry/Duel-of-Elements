#ifndef __INPUT_H__
#define __INPUT_H__

#include "main.h"

#define NUM_KEY_MAX				256

struct DINPUT {

	static LPDIRECTINPUT8	   	pDInput;							// IDirectInput8���󥿩`�ե��`���ؤΥݥ���
	static LPDIRECTINPUTDEVICE8	pDIDevKeyboard;						// IDirectInputDevice8���󥿩`�ե��`���ؤΥݥ���(���`�ܩ`��)
	static BYTE				   	keyStates[NUM_KEY_MAX];				// ���`�ܩ`�ɤ�Ѻ��״�B�򱣳֤����`��
	static BYTE				   	keyStatesTrigger[NUM_KEY_MAX];		// ���`�ܩ`�ɤΥȥꥬ�`״�B�򱣳֤����`��
	static BYTE				   	keyStatesRelease[NUM_KEY_MAX];		// ���`�ܩ`�ɤΥ��`��״�B�򱣳֤����`��
	static BYTE				   	keyStatesRepeat[NUM_KEY_MAX];		// ���`�ܩ`�ɤΥ�ԩ`��״�B�򱣳֤����`��
	static int				   	keyStatesRepeatCnt[NUM_KEY_MAX];	// ���`�ܩ`�ɤΥ�ԩ`�ȥ�����

	static BOOL					GetKeyboardPress(BYTE key);
	static BOOL					GetKeyboardTrigger(BYTE key);
	static BOOL					GetKeyboardRepeat(BYTE key);
	static BOOL					GetKeyboardRelease(BYTE key);

	static HRESULT				Init(HINSTANCE hInst, HWND hWnd);
	static void					Destroy(void);
	static HRESULT				Update(void);
};

#endif // !__INPUT_H__
