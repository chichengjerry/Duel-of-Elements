#include "input.h"
#include "d3d.h"

LPDIRECTINPUT8			DINPUT::pDInput = NULL;
LPDIRECTINPUTDEVICE8	DINPUT::pDIDevKeyboard = NULL;
BYTE				   	DINPUT::keyStates[NUM_KEY_MAX] = { NULL };		 	// キーボードの押下状態を保持するワーク
BYTE				   	DINPUT::keyStatesTrigger[NUM_KEY_MAX] = { NULL };	// キーボードのトリガー状態を保持するワーク
BYTE				   	DINPUT::keyStatesRelease[NUM_KEY_MAX] = { NULL };	// キーボードのリリース状態を保持するワーク
BYTE				   	DINPUT::keyStatesRepeat[NUM_KEY_MAX] = { NULL }; 	// キーボードのリピート状態を保持するワーク
int				   		DINPUT::keyStatesRepeatCnt[NUM_KEY_MAX] = { NULL };	// キーボードのリピートカウンタ

BOOL DINPUT::GetKeyboardPress(BYTE key)
{
	return !!(keyStates[key] & 0x80);
}

BOOL DINPUT::GetKeyboardTrigger(BYTE key)
{
	return !!(keyStatesTrigger[key] & 0x80);
}

BOOL DINPUT::GetKeyboardRepeat(BYTE key)
{
	return !!(keyStatesRepeat[key] & 0x80);
}

BOOL DINPUT::GetKeyboardRelease(BYTE key)
{
	return !!(keyStatesRelease[key] & 0x80);;
}

HRESULT DINPUT::Init(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;

	if (!pDInput)
	{
		// DirectInputオブジェクトの作成
		hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION,
			IID_IDirectInput8, (void**)&pDInput, NULL);
	}

	// デバイスオブジェクトを作成
	hr = pDInput->CreateDevice(GUID_SysKeyboard, &pDIDevKeyboard, NULL);
	if (FAILED(hr) || pDIDevKeyboard == NULL)
	{
		MessageBox(hWnd, _T("キーボードがねぇ！"), _T("警告！"), MB_ICONWARNING);
		return hr;
	}

	// データフォーマットを設定
	hr = pDIDevKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
	{
		MessageBox(hWnd, _T("キーボードのデータフォーマットを設定できませんでした。"), _T("警告！"), MB_ICONWARNING);
		return hr;
	}

	// 協調モードを設定（フォアグラウンド＆非排他モード）
	hr = pDIDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if (FAILED(hr))
	{
		MessageBox(hWnd, _T("キーボードの協調モードを設定できませんでした。"), _T("警告！"), MB_ICONWARNING);
		return hr;
	}

	// キーボードへのアクセス権を獲得(入力制御開始)
	pDIDevKeyboard->Acquire();

	return S_OK;
}

void DINPUT::Destroy(void)
{
	SAFE_RELEASE(DINPUT::pDInput);
	SAFE_RELEASE(DINPUT::pDIDevKeyboard);
}

HRESULT DINPUT::Update(void)
{
	HRESULT hr;
	BYTE keyStates[NUM_KEY_MAX];

	// デバイスからデータを取得
	hr = pDIDevKeyboard->GetDeviceState(sizeof(keyStates), keyStates);

	if (SUCCEEDED(hr))
	{
		for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			keyStatesTrigger[nCntKey] = (keyStates[nCntKey] ^ keyStates[nCntKey]) & keyStates[nCntKey];
			keyStatesRelease[nCntKey] = (keyStates[nCntKey] ^ keyStates[nCntKey]) & ~keyStates[nCntKey];
			keyStatesRepeat[nCntKey] = keyStatesTrigger[nCntKey];

			if (keyStates[nCntKey])
			{
				keyStatesRepeatCnt[nCntKey]++;
				if (keyStatesRepeatCnt[nCntKey] >= 20)
				{
					keyStatesRepeat[nCntKey] = keyStates[nCntKey];
				}
				else
				{
					keyStatesRepeat[nCntKey] = 0;
				}
			}
			else
			{
				keyStatesRepeatCnt[nCntKey] = 0;
				keyStatesRepeat[nCntKey] = 0;
			}

			keyStates[nCntKey] = keyStates[nCntKey];
		}
	}
	else
	{
		// キーボードへのアクセス権を取得
		pDIDevKeyboard->Acquire();
	}

	return S_OK;
}
