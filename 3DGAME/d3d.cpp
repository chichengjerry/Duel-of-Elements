#include "d3d.h"
#include "stage.h"
#include "sound.h"
#include "input.h"

LPDIRECT3D9			D3D::pD3D = NULL;								// Direct3D ���֥�������
LPDIRECT3DDEVICE9	D3D::pDevice = NULL;							// Device���֥�������(�軭�˱�Ҫ)

GAMESTATE* game;

void D3D::Destroy(void)
{
	delete game;

	DSOUND::Destroy();

	DINPUT::Destroy();
	SAFE_RELEASE(D3D::pD3D);
	SAFE_RELEASE(D3D::pDevice);
}

LPDIRECT3DDEVICE9 D3D::GetDevice(void)
{
	return D3D::pDevice;
}

HRESULT D3D::Init(HINSTANCE hInst, HWND hWnd)
{
	LPDIRECT3D9 pD3D;
	LPDIRECT3DDEVICE9 pDevice;

	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	BOOL bWindow = TRUE;

	// Direct3D���֥������Ȥ�����
	pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (pD3D == NULL)
	{
		return E_FAIL;
	}
	D3D::pD3D = pD3D;

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
	D3D::pDevice = pDevice;
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
	DINPUT::Init(hInst, hWnd);
	DSOUND::Init(hWnd);

	game = new GAMESTATE(STAGE_VERSUS);

	return S_OK;
}

HRESULT D3D::Render(void)
{
	pDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// Direct3D�ˤ���軭���_ʼ
	if (SUCCEEDED(pDevice->BeginScene()))
	{
		game->Draw();

		// Direct3D�ˤ���軭�νK��
		pDevice->EndScene();
	}

	// �Хå��Хåե��ȥե��ȥХåե�������椨
	pDevice->Present(NULL, NULL, NULL, NULL);
	return S_OK;
}

HRESULT D3D::Update(void)
{
	DINPUT::Update();

	game->Update();

	return S_OK;
}
