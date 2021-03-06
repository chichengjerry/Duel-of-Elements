//-----------------------------------------------
// [input.cpp]
// 入力処理
// 作者 Chicheng Zhang
//-----------------------------------------------
#include "input.h"
#include "d3d.h"

//
// ゲームパッド用パラメーター設定
//
#define GP_DEADZONE				2500			// 各軸の25%を無効ゾーンとする
#define GP_RANGE_MAX			1000			// 有効範囲の最大値
#define GP_RANGE_MIN			-1000			// 有効範囲の最小値

//
// 静的メンバーの初期化 
//

LPDIRECTINPUT8			DINPUT::pDInput = NULL;
LPDIRECTINPUTDEVICE8	DINPUT::pDIDevKeyboard = NULL;
BYTE				   	DINPUT::keyStates[NUM_KEY_MAX] = { NULL };		 	// キーボードの押下状態を保持するワーク
BYTE				   	DINPUT::keyStatesTrigger[NUM_KEY_MAX] = { NULL };	// キーボードのトリガー状態を保持するワーク
BYTE				   	DINPUT::keyStatesRelease[NUM_KEY_MAX] = { NULL };	// キーボードのリリース状態を保持するワーク
BYTE				   	DINPUT::keyStatesRepeat[NUM_KEY_MAX] = { NULL }; 	// キーボードのリピート状態を保持するワーク
int				   		DINPUT::keyStatesRepeatCnt[NUM_KEY_MAX] = { NULL };	// キーボードのリピートカウンタ

LPDIRECTINPUTDEVICE8	DINPUT::pGamePad[NUM_PAD_MAX] = { NULL };// パッドデバイス

DWORD					DINPUT::padState[NUM_PAD_MAX] = { NULL };	// パッド情報（複数対応）
DWORD					DINPUT::padTrigger[NUM_PAD_MAX] = { NULL };
UINT					DINPUT::padCount = 0;

//
// キーの押下状態をチェックする。
//
BOOL DINPUT::KeyPressed(BYTE key)
{
	return !!(keyStates[key] & 0x80);
}

//
// キーのリリース状態をチェックする。
//
BOOL DINPUT::KeyReleased(BYTE key)
{
	return !!(keyStatesRelease[key] & 0x80);
}

//
// キーのトリガー状態をチェックする。
//
BOOL DINPUT::KeyTriggered(BYTE key)
{
	return !!(keyStatesTrigger[key] & 0x80);
}

//
// キーのリピート状態をチェックする。
//
BOOL DINPUT::KeyRepeated(BYTE key)
{
	return !!(keyStatesRepeat[key] & 0x80);
}

//
// 特定のゲームパッドの押下状態をチェックする。
//
BOOL DINPUT::ButtonPressed(int padNo, DWORD button)
{
	return (button & padState[padNo]);
}

//
// すべてのゲームパッドの中に押下状態をチェックする。
//
BOOL DINPUT::ButtonPressed(DWORD button)
{
	for (UINT i = 0; i < padCount; i++) {
		if (ButtonPressed(i, button))
			return TRUE;
	}
	return FALSE;
}

//
// 特定のゲームパッドのトリガー状態をチェックする。
//
BOOL DINPUT::ButtonTriggered(int padNo, DWORD button)
{
	return (button & padTrigger[padNo]);
}

//
// すべてのゲームパッドの中にトリガー状態をチェックする。
//
BOOL DINPUT::ButtonTriggered(DWORD button)
{
	for (UINT i = 0; i < padCount; i++) {
		if (ButtonTriggered(i, button))
			return TRUE;
	}
	return FALSE;
}

//
// 入力処理初期化。
//
HRESULT DINPUT::Init(HINSTANCE hInst, HWND hWnd)
{
	InitKeyboard(hInst, hWnd);
	InitGamePad(hInst, hWnd);

	return S_OK;
}

//
// キーボード入力処理初期化。
//
HRESULT DINPUT::InitKeyboard(HINSTANCE hInst, HWND hWnd)
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
	hr = pDIDevKeyboard->Acquire();

	return hr;
}

//
// ゲームパッド入力処理初期化。
//
HRESULT DINPUT::InitGamePad(HINSTANCE hInst, HWND hWnd)
{
	HRESULT		hr;
	padCount = 0;

	// ジョイパッドを探す
	if (!pDInput) {
		hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION,
			IID_IDirectInput8, (void**)&pDInput, NULL);

		if (FAILED(hr)) {
			return hr;
		}
	}

	pDInput->EnumDevices(DI8DEVCLASS_GAMECTRL, (LPDIENUMDEVICESCALLBACK)SearchGamePadCallback, NULL, DIEDFL_ATTACHEDONLY);
	// セットしたコールバック関数が、パッドを発見した数だけ呼ばれる。

	for (UINT i = 0; i < padCount; i++) {
		// ジョイスティック用のデータ・フォーマットを設定
		hr = pGamePad[i]->SetDataFormat(&c_dfDIJoystick);
		if (FAILED(hr)) {
			return hr;
		}

		// モードを設定（フォアグラウンド＆非排他モード）
//		hr = pGamePad[i]->SetCooperativeLevel(hWindow, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
//		if ( FAILED(hr) )
//			return false; // モードの設定に失敗

		// 軸の値の範囲を設定
		// X軸、Y軸のそれぞれについて、オブジェクトが報告可能な値の範囲をセットする。
		// (max-min)は、最大10,000(?)。(max-min)/2が中央値になる。
		// 差を大きくすれば、アナログ値の細かな動きを捕らえられる。(パッドの性能による)
		DIPROPRANGE				diprg;
		ZeroMemory(&diprg, sizeof(diprg));
		diprg.diph.dwSize = sizeof(diprg);
		diprg.diph.dwHeaderSize = sizeof(diprg.diph);
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.lMin = GP_RANGE_MIN;
		diprg.lMax = GP_RANGE_MAX;
		// X軸の範囲を設定
		diprg.diph.dwObj = DIJOFS_X;
		pGamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);
		// Y軸の範囲を設定
		diprg.diph.dwObj = DIJOFS_Y;
		pGamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);

		// 各軸ごとに、無効のゾーン値を設定する。
		// 無効ゾーンとは、中央からの微少なジョイスティックの動きを無視する範囲のこと。
		// 指定する値は、10000に対する相対値(2000なら20パーセント)。
		DIPROPDWORD				dipdw;
		dipdw.diph.dwSize = sizeof(DIPROPDWORD);
		dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
		dipdw.diph.dwHow = DIPH_BYOFFSET;
		dipdw.dwData = GP_DEADZONE;
		//X軸の無効ゾーンを設定
		dipdw.diph.dwObj = DIJOFS_X;
		pGamePad[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
		//Y軸の無効ゾーンを設定
		dipdw.diph.dwObj = DIJOFS_Y;
		pGamePad[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);

		//ジョイスティック入力制御開始
		pGamePad[i]->Acquire();
	}

	return S_OK;
}

//
// 終了処理及びリソースの解放
//
void DINPUT::Destroy(void)
{
#define SAFE_UNAQUIRE(p)	{ if(p) {(p)->Unacquire(); (p)->Release(); } }
	SAFE_UNAQUIRE(DINPUT::pDIDevKeyboard);
	for (int i = 0; i < NUM_PAD_MAX; i++) {
		SAFE_UNAQUIRE(pGamePad[i]);
	}
	SAFE_RELEASE(DINPUT::pDInput);
#undef SAFE_UNAQUIRE
}

//
// 入力を更新する。
//
HRESULT DINPUT::Update(void)
{
	UpdateKeyboard();
	UpdateGamePad();

	return S_OK;
}

//
// キーボードを更新する。
//
HRESULT DINPUT::UpdateKeyboard(void)
{
	HRESULT hr;
	BYTE keyStatesOld[NUM_KEY_MAX];

	memcpy(keyStatesOld, keyStates, NUM_KEY_MAX);

	// デバイスからデータを取得
	hr = pDIDevKeyboard->GetDeviceState(sizeof(keyStates), keyStates);

	if (SUCCEEDED(hr))
	{
		for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			keyStatesTrigger[nCntKey] = (keyStatesOld[nCntKey] ^ keyStates[nCntKey]) & keyStates[nCntKey];
			keyStatesRelease[nCntKey] = (keyStatesOld[nCntKey] ^ keyStates[nCntKey]) & ~keyStates[nCntKey];
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

//
// ゲームパッドを更新する。
//
HRESULT DINPUT::UpdateGamePad(void)
{
	HRESULT			result = S_OK;
	DIJOYSTATE2		dijs;

	for (UINT i = 0; i < padCount; i++)
	{
		DWORD lastPadState;
		lastPadState = padState[i];
		padState[i] = 0x0UL;	// 初期化

		result = pGamePad[i]->Poll();	// ジョイスティックにポールをかける
		if (FAILED(result)) {
			result = pGamePad[i]->Acquire();
			while (result == DIERR_INPUTLOST) {
				result = pGamePad[i]->Acquire();
			}
		}

		result = pGamePad[i]->GetDeviceState(sizeof(DIJOYSTATE), &dijs);	// デバイス状態を読み取る
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED) {
			result = pGamePad[i]->Acquire();
			while (result == DIERR_INPUTLOST) {
				result = pGamePad[i]->Acquire();
			}
		}

		// ３２の各ビットに意味を持たせ、ボタン押下に応じてビットをオンにする
		//* y-axis (forward)
		if (dijs.lY < 0)					padState[i] |= BUTTON_UP;
		//* y-axis (backward)
		if (dijs.lY > 0)					padState[i] |= BUTTON_DOWN;
		//* x-axis (left)
		if (dijs.lX < 0)					padState[i] |= BUTTON_LEFT;
		//* x-axis (right)
		if (dijs.lX > 0)					padState[i] |= BUTTON_RIGHT;
		
		//* y-axis (forward)
		if (dijs.lRy < 0)					padState[i] |= BUTTON_RROTATE_UP;
		//* y-axis (backward)
		if (dijs.lRx > 0)					padState[i] |= BUTTON_RROTATE_DOWN;
		//* x-axis (left)
		if (dijs.lRx < 0)					padState[i] |= BUTTON_RROTATE_LEFT;
		//* x-axis (right)
		if (dijs.lRx > 0)					padState[i] |= BUTTON_RROTATE_RIGHT;

		//* Ａボタン
		if (dijs.rgbButtons[0] & 0x80)		padState[i] |= BUTTON_X;
		//* Ｂボタン
		if (dijs.rgbButtons[1] & 0x80)		padState[i] |= BUTTON_A;
		//* Ｃボタン
		if (dijs.rgbButtons[2] & 0x80)		padState[i] |= BUTTON_B;
		//* Ｘボタン
		if (dijs.rgbButtons[3] & 0x80)		padState[i] |= BUTTON_Y;
		//* Ｙボタン
		if (dijs.rgbButtons[4] & 0x80)		padState[i] |= BUTTON_LB; //LB
		//* Ｚボタン
		if (dijs.rgbButtons[5] & 0x80)		padState[i] |= BUTTON_RB; //RB
		//* Ｌボタン
		if (dijs.rgbButtons[6] & 0x80)		padState[i] |= BUTTON_LT; //LT
		//* Ｒボタン
		if (dijs.rgbButtons[7] & 0x80)		padState[i] |= BUTTON_RT; //RT
		//* ＳＴＡＲＴボタン
		if (dijs.rgbButtons[8] & 0x80)		padState[i] |= BUTTON_BACK; //BACK
		//* Ｍボタン
		if (dijs.rgbButtons[9] & 0x80)		padState[i] |= BUTTON_START; //START

		// Trigger設定
		padTrigger[i] = ((lastPadState ^ padState[i])	// 前回と違っていて
			& padState[i]);					// しかも今ONのやつ

	}
	return result;
}

//
// コールバック関数でゲームパッドデバイスを初期化。
//
BOOL DINPUT::SearchGamePadCallback(LPDIDEVICEINSTANCE lpddi, LPVOID)
{
	HRESULT result;
	result = pDInput->CreateDevice(lpddi->guidInstance, &pGamePad[padCount++], NULL);

	return DIENUM_CONTINUE;	// 次のデバイスを列挙
}
