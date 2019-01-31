#include "input.h"
#include "d3d.h"

LPDIRECTINPUT8			DINPUT::pDInput = NULL;
LPDIRECTINPUTDEVICE8	DINPUT::pDIDevKeyboard = NULL;
BYTE				   	DINPUT::keyStates[NUM_KEY_MAX] = { NULL };		 	// ���`�ܩ`�ɤ�Ѻ��״�B�򱣳֤����`��
BYTE				   	DINPUT::keyStatesTrigger[NUM_KEY_MAX] = { NULL };	// ���`�ܩ`�ɤΥȥꥬ�`״�B�򱣳֤����`��
BYTE				   	DINPUT::keyStatesRelease[NUM_KEY_MAX] = { NULL };	// ���`�ܩ`�ɤΥ��`��״�B�򱣳֤����`��
BYTE				   	DINPUT::keyStatesRepeat[NUM_KEY_MAX] = { NULL }; 	// ���`�ܩ`�ɤΥ�ԩ`��״�B�򱣳֤����`��
int				   		DINPUT::keyStatesRepeatCnt[NUM_KEY_MAX] = { NULL };	// ���`�ܩ`�ɤΥ�ԩ`�ȥ�����

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
		// DirectInput���֥������Ȥ�����
		hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION,
			IID_IDirectInput8, (void**)&pDInput, NULL);
	}

	// �ǥХ������֥������Ȥ�����
	hr = pDInput->CreateDevice(GUID_SysKeyboard, &pDIDevKeyboard, NULL);
	if (FAILED(hr) || pDIDevKeyboard == NULL)
	{
		MessageBox(hWnd, _T("���`�ܩ`�ɤ��ͤ���"), _T("���棡"), MB_ICONWARNING);
		return hr;
	}

	// �ǩ`���ե��`�ޥåȤ��O��
	hr = pDIDevKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
	{
		MessageBox(hWnd, _T("���`�ܩ`�ɤΥǩ`���ե��`�ޥåȤ��O���Ǥ��ޤ���Ǥ�����"), _T("���棡"), MB_ICONWARNING);
		return hr;
	}

	// �f�{��`�ɤ��O�����ե������饦��ɣ���������`�ɣ�
	hr = pDIDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if (FAILED(hr))
	{
		MessageBox(hWnd, _T("���`�ܩ`�ɤ΅f�{��`�ɤ��O���Ǥ��ޤ���Ǥ�����"), _T("���棡"), MB_ICONWARNING);
		return hr;
	}

	// ���`�ܩ`�ɤؤΥ��������ؤ�@��(���������_ʼ)
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

	// �ǥХ�������ǩ`����ȡ��
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
		// ���`�ܩ`�ɤؤΥ��������ؤ�ȡ��
		pDIDevKeyboard->Acquire();
	}

	return S_OK;
}
