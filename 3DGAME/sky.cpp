//=============================================================================
//
// 地面処理 [field.cpp]
// Author : 
//
//=============================================================================
#include "sky.h"
#include "camera.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_SKY	_T("data/TEXTURE/daylight")	// 読み込むテクスチャファイル名

SKY::SKY()
{
	LPDIRECT3DDEVICE9 pDevice = D3D::GetDevice();
	HRESULT hr;

	LPCWSTR SKY_LIST[6] = { _T("ft"), _T("bk"), _T("lf"), _T("rt"), _T("up"), _T("dn") };
	
	WCHAR str[1024];
	// 位置、向きの初期設定
	pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < SKY_NUM; i++) {
		wsprintf(str, _T("%s_%s.png"), TEXTURE_SKY, SKY_LIST[i]);
		D3DXCreateTextureFromFile(pDevice, str, &pTex[i]);
	}
	// wsprintf(str, "%s.tga", TEXTURE_SKY);
	// hr = D3DXCreateTextureFromFile(pDevice, str, &g_pD3DTextureSky[0]);
// 頂点情報の作成
	hr = MakeSky();
}

SKY::~SKY()
{
	for (int i = 0; i < SKY_NUM; i++) {
		SAFE_RELEASE(pTex[i]);
	}
	SAFE_RELEASE(pVtx);
}

HRESULT SKY::MakeSky()
{
	LPDIRECT3DDEVICE9 pDevice = D3D::GetDevice();

	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * SKY_NUM,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_3D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&pVtx,		// 頂点バッファインターフェースへのポインタ
		NULL)))						// NULLに設定
	{
		return E_FAIL;
	}

	FLOAT boxsize = GR_FAR_Z / 2.0f;

	D3DXVECTOR3 vertex[8] = {
		{-1.0f,  1.0f,  1.0f},
		{ 1.0f,  1.0f,  1.0f},
		{-1.0f,  1.0f, -1.0f},
		{ 1.0f,  1.0f, -1.0f},
		{-1.0f, -1.0f,  1.0f},
		{ 1.0f, -1.0f,  1.0f},
		{-1.0f, -1.0f, -1.0f},
		{ 1.0f, -1.0f, -1.0f},
	};

	DWORD indicy[SKY_NUM][NUM_VERTEX] = {
		{ 0, 1, 4, 5 },
		{ 3, 2, 7, 6 },
		{ 2, 0, 6, 4 },
		{ 1, 3, 5, 7 },
		{ 2, 3, 0, 1 },
		{ 4, 5, 6, 7 }
	};

	//頂点バッファの中身を埋める
	VERTEX_3D *pTmpVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	pVtx->Lock(0, 0, (void**)&pTmpVtx, 0);

	for (int i = 0; i < SKY_NUM; i++)
	{
		// 頂点座標の設定
		pTmpVtx[i * NUM_VERTEX + 0].vtx = boxsize * vertex[indicy[i][0]];
		pTmpVtx[i * NUM_VERTEX + 1].vtx = boxsize * vertex[indicy[i][1]];
		pTmpVtx[i * NUM_VERTEX + 2].vtx = boxsize * vertex[indicy[i][2]];
		pTmpVtx[i * NUM_VERTEX + 3].vtx = boxsize * vertex[indicy[i][3]];

		// 法線ベクトルの設定
		pTmpVtx[i * NUM_VERTEX + 0].nor =
		pTmpVtx[i * NUM_VERTEX + 1].nor =
		pTmpVtx[i * NUM_VERTEX + 2].nor =
		pTmpVtx[i * NUM_VERTEX + 3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// 反射光の設定
		pTmpVtx[i * NUM_VERTEX + 0].dif =
		pTmpVtx[i * NUM_VERTEX + 1].dif =
		pTmpVtx[i * NUM_VERTEX + 2].dif =
		pTmpVtx[i * NUM_VERTEX + 3].dif = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		pTmpVtx[i * NUM_VERTEX + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pTmpVtx[i * NUM_VERTEX + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pTmpVtx[i * NUM_VERTEX + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pTmpVtx[i * NUM_VERTEX + 3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	// 頂点データをアンロックする
	pVtx->Unlock();

	return S_OK;
}

void SKY::Update()
{
}

void SKY::Draw(CAMERA* pCamera)
{
	LPDIRECT3DDEVICE9 pDevice = D3D::GetDevice();
	D3DXMATRIX mtxRot, mtxTranslate;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtx);

	D3DXVECTOR3 pos = pCamera->v_pos;
	DWORD value;
	pDevice->GetSamplerState(0, D3DSAMP_MAGFILTER, &value);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTranslate, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&mtx, &mtx, &mtxTranslate);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtx);

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, pVtx, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);

	// IMPORTANT：エッジのピクセルを重複する
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);	// テクスチャアドレッシング方法(U値)を設定
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);	// テクスチャアドレッシング方法(V値)を設定

	for (int i = 0; i < SKY_NUM; i++) {
		// テクスチャの設定
		pDevice->SetTexture(0, pTex[i]);
		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * NUM_VERTEX, NUM_POLYGON);
	}

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, value);

	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(U値)を設定
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(V値)を設定
}
