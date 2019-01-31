#include "d3d.h"
#include "stage.h"
#include "sound.h"
#include "input.h"

LPDIRECT3D9			D3D::pD3D = NULL;								// Direct3D オブジェクト
LPDIRECT3DDEVICE9	D3D::pDevice = NULL;							// Deviceオブジェクト(宙鮫に駅勣)

MAINGAME* game;

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

	// Direct3Dオブジェクトの恬撹
	pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (pD3D == NULL)
	{
		return E_FAIL;
	}
	D3D::pD3D = pD3D;

	// �F壓のディスプレイモ�`ドを函誼
	if (FAILED(pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテ�`ションパラメ�`タの�O協
	ZeroMemory(&d3dpp, sizeof(d3dpp));						// ワ�`クをゼロクリア
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferWidth = CL_WIDTH;				// ゲ�`ム鮫中サイズ(嫌)
	d3dpp.BackBufferHeight = CL_HEIGHT;			// ゲ�`ム鮫中サイズ(互さ)
	d3dpp.BackBufferFormat = d3ddm.Format;				// カラ�`モ�`ドの峺協
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	// 啌�駮添鼎僕�豚してフリップする
	d3dpp.Windowed = bWindow;					// ウィンドウモ�`ド
	d3dpp.EnableAutoDepthStencil = TRUE;						// デプスバッファ�┌撻丱奪侫。�とステンシルバッファを恬撹
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;				// デプスバッファとして16bitを聞う

	if (bWindow)
	{// ウィンドウモ�`ド
		d3dpp.FullScreen_RefreshRateInHz = 0;								// リフレッシュレ�`ト
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;	// インタ�`バル
	}
	else
	{// フルスクリ�`ンモ�`ド
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;			// リフレッシュレ�`ト
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		// インタ�`バル
	}

	// デバイスの伏撹
	// ディスプレイアダプタを燕すためのデバイスを恬撹
	// 宙鮫と��泣�I尖をハ�`ドウェアで佩なう
	if (FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &pDevice)))
	{
		// 貧��の�O協が払�，靴燭�
		// 宙鮫をハ�`ドウェアで佩い、��泣�I尖はCPUで佩なう
		if (FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &pDevice)))
		{
			// 貧��の�O協が払�，靴燭�
			// 宙鮫と��泣�I尖をCPUで佩なう
			if (FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp, &pDevice)))
			{
				// 兜豚晒払��
				return E_FAIL;
			}
		}
	}
	D3D::pDevice = pDevice;
	// レンダリングステ�`トパラメ�`タの�O協
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// �Y中をカリング
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Zバッファを聞喘
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// αブレンドを佩う
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソ�`スカラ�`の峺協
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネ�`ションカラ�`の峺協

	// サンプラ�`ステ�`トパラメ�`タの�O協
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャアドレッシング圭隈(U��)を�O協
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャアドレッシング圭隈(V��)を�O協
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// テクスチャ�s弌フィルタモ�`ドを�O協
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// テクスチャ��寄フィルタモ�`ドを�O協

	// テクスチャステ�`ジステ�`トパラメ�`タの�O協
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);		// アルファブレンディング�I尖
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);		// 恷兜のアルファ哈方
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);		// ２桑朕のアルファ哈方

	// 兜豚晒
	DINPUT::Init(hInst, hWnd);
	DSOUND::Init(hWnd);

	game = new MAINGAME(2);

	DSOUND::Play(SOUND_BGM000);
	return S_OK;
}

HRESULT D3D::Render(void)
{
	pDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// Direct3Dによる宙鮫の�_兵
	if (SUCCEEDED(pDevice->BeginScene()))
	{
		game->Draw();

		// Direct3Dによる宙鮫の�K阻
		pDevice->EndScene();
	}

	// バックバッファとフロントバッファの秘れ紋え
	pDevice->Present(NULL, NULL, NULL, NULL);
	return S_OK;
}

HRESULT D3D::Update(void)
{
	DINPUT::Update();

	game->Update();

	return S_OK;
}
