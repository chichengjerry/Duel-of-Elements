#include "d3d.h"

static D3DVTBL vtable;

void D3DDestroy(LPD3D _this)
{
	if (_this) {
		SAFE_RELEASE(_this->pD3D);
		SAFE_RELEASE(_this->pDevice);
	}
}

LPDIRECT3DDEVICE9 D3DGetDevice(LPD3D _this)
{
	return _this->pDevice;
};

HRESULT D3DInit(LPD3D _this, HINSTANCE hInst, HWND hWnd)
{
	_this->lpVtbl = &vtable;
	LPDIRECT3D9 pD3D = _this->pD3D;
	LPDIRECT3DDEVICE9 pDevice = _this->pDevice;

	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	BOOL bWindow = TRUE;

	// Direct3D���֥������Ȥ�����
	pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (pD3D == NULL)
	{
		return E_FAIL;
	}

	// �F�ڤΥǥ����ץ쥤��`�ɤ�ȡ��
	if (FAILED(pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// �ǥХ����Υץ쥼��Ʃ`�����ѥ��`�����O��
	ZeroMemory(&d3dpp, sizeof(d3dpp));						// ��`���򥼥��ꥢ
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferWidth = CL_WIDTH;				// ���`�໭�極����(��)
	d3dpp.BackBufferHeight = CL_HEIGHT;			// ���`�໭�極����(�ߤ�)
	d3dpp.BackBufferFormat = d3ddm.Format;				// ����`��`�ɤ�ָ��
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	// ӳ���źŤ�ͬ�ڤ��ƥե�åפ���
	d3dpp.Windowed = bWindow;					// ������ɥ���`��
	d3dpp.EnableAutoDepthStencil = TRUE;						// �ǥץ��Хåե����ڥХåե����ȥ��ƥ󥷥�Хåե�������
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;				// �ǥץ��Хåե��Ȥ���16bit��ʹ��

	if (bWindow)
	{// ������ɥ���`��
		d3dpp.FullScreen_RefreshRateInHz = 0;								// ��ե�å����`��
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;	// ���󥿩`�Х�
	}
	else
	{// �ե륹����`���`��
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;			// ��ե�å����`��
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		// ���󥿩`�Х�
	}

	// �ǥХ���������
	// �ǥ����ץ쥤�����ץ��������ΥǥХ���������
	// �軭��픵�I���ϩ`�ɥ��������Фʤ�
	if (FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &pDevice)))
	{
		// ��ӛ���O����ʧ��������
		// �軭��ϩ`�ɥ��������Ф���픵�I���CPU���Фʤ�
		if (FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &pDevice)))
		{
			// ��ӛ���O����ʧ��������
			// �軭��픵�I���CPU���Фʤ�
			if (FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp, &pDevice)))
			{
				// ���ڻ�ʧ��
				return E_FAIL;
			}
		}
	}

	// ������󥰥��Ʃ`�ȥѥ��`�����O��
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// �Y��򥫥��
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Z�Хåե���ʹ��
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// ���֥��ɤ��Ф�
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// �����`������`��ָ��
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���ǥ��ƥ��ͩ`����󥫥�`��ָ��

	// ����ץ�`���Ʃ`�ȥѥ��`�����O��
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// �ƥ������㥢�ɥ�å��󥰷���(U��)���O��
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// �ƥ������㥢�ɥ�å��󥰷���(V��)���O��
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// �ƥ�������sС�ե��륿��`�ɤ��O��
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// �ƥ������㒈��ե��륿��`�ɤ��O��

	// �ƥ������㥹�Ʃ`�����Ʃ`�ȥѥ��`�����O��
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);		// ����ե��֥��ǥ��󥰄I��
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);		// ����Υ���ե�����
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);		// ����Ŀ�Υ���ե�����

	// ���ڻ�


	return S_OK;
}

HRESULT D3DRender(LPD3D _this)
{
	return E_NOTIMPL;
}

void D3DUpdate(LPD3D _this)
{
}
