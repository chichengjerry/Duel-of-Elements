//-----------------------------------------------
// [sky.cpp]
// スカイボックス処理
// 作者 Chicheng Zhang
//-----------------------------------------------
#include "sky.h"
#include "camera.h"

#define	TEXTURE_SKY_PREFIX	_T("data/TEXTURE/sky/daylight")
#define	TEXTURE_SUN			_T("data/TEXTURE/sky/sun.png")

//
// コンストラクター
//
SKY::SKY()
{
	LPDIRECT3DDEVICE9 pDevice = D3D::GetDevice();

	LPCWSTR SKY_LIST[6] = { _T("ft"), _T("bk"), _T("lf"), _T("rt"), _T("up"), _T("dn") };
	
	// 位置、向きの初期設定
	pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < SKY_NUM; i++) {
		WCHAR str[256];

		wsprintf(str, _T("%s_%s.png"), TEXTURE_SKY_PREFIX, SKY_LIST[i]);
		D3D::LoadTexture(&pTex[i], str);
	}
	D3D::LoadTexture(&pTexSun, TEXTURE_SUN);

	light = new LIGHT();

	sun = new EMITTER(pTexSun,
		1,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXCOLOR(1.0f, 1.0f, 0.25f, 1.0f), 500.0f);
	
	SetSky();
}

//
// デストラクター
//
SKY::~SKY()
{
	SAFE_DELETE(light);
	SAFE_RELEASE(pVtx);

	for (int i = 0; i < SKY_NUM; i++) {
		SAFE_RELEASE(pTex[i]);
	}
	SAFE_RELEASE(pTexSun);
}

//
// スカイボックスを初期化する。
//
HRESULT SKY::SetSky()
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

	DWORD indices[SKY_NUM][NUM_VERTEX] = {
		{ 0, 1, 4, 5 },
		{ 3, 2, 7, 6 },
		{ 2, 0, 6, 4 },
		{ 1, 3, 5, 7 },
		{ 2, 3, 0, 1 },
		{ 4, 5, 6, 7 }
	};

	//頂点バッファの中身を埋める
	VERTEX_3D *pTempVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	pVtx->Lock(0, 0, (void**)&pTempVtx, 0);

	for (int i = 0; i < SKY_NUM; i++)
	{
		// 頂点座標の設定
		pTempVtx[0].vtx = boxsize * vertex[indices[i][0]];
		pTempVtx[1].vtx = boxsize * vertex[indices[i][1]];
		pTempVtx[2].vtx = boxsize * vertex[indices[i][2]];
		pTempVtx[3].vtx = boxsize * vertex[indices[i][3]];

		// 法線ベクトルの設定
		pTempVtx[0].nor =
		pTempVtx[1].nor =
		pTempVtx[2].nor =
		pTempVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// 反射光の設定
		pTempVtx[0].dif =
		pTempVtx[1].dif =
		pTempVtx[2].dif =
		pTempVtx[3].dif = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		pTempVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pTempVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pTempVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pTempVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pTempVtx += NUM_VERTEX;
	}

	// 頂点データをアンロックする
	pVtx->Unlock();

	return S_OK;
}

//
// スカイボックスの更新
//
void SKY::Update()
{
	light->Update();

	sun->particles[0].pos = pos - (D3DXVECTOR3)light->lightSun.Direction * GR_FAR_Z / 2;
	sun->SetDiffuse(0, light->GetLightColor());
	sun->Update();

	VERTEX_3D *pTempVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	pVtx->Lock(0, 0, (void**)&pTempVtx, 0);

	for (int i = 0; i < SKY_NUM; i++)
	{
		pTempVtx[0].dif =
		pTempVtx[1].dif =
		pTempVtx[2].dif =
		pTempVtx[3].dif = light->GetSkyColor();

		pTempVtx += NUM_VERTEX;
	}

	// 頂点データをアンロックする
	pVtx->Unlock();
}

//
// スカイボックスの描画。常にカメラとともに移動する。
//
void SKY::Draw(CAMERA* pCamera)
{
	LPDIRECT3DDEVICE9 pDevice = D3D::GetDevice();
	D3DXMATRIX mtxTranslate;

	D3DXVECTOR3 pos = pCamera->v_pos;
	pos.x = roundf(pos.x);
	pos.y = roundf(pos.y);
	pos.z = roundf(pos.z);

	DWORD magfilter;
	pDevice->GetSamplerState(0, D3DSAMP_MAGFILTER, &magfilter);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtx);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTranslate, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&mtx, &mtx, &mtxTranslate);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtx);

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, pVtx, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// IMPORTANT：エッジのピクセルを重複する
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);	// テクスチャアドレッシング方法(U値)を設定
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);	// テクスチャアドレッシング方法(V値)を設定

	for (int i = 0; i < SKY_NUM; i++) {
		// テクスチャの設定
		pDevice->SetTexture(0, pTex[i]);
		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * NUM_VERTEX, NUM_POLYGON);
	}

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, magfilter);
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(U値)を設定
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(V値)を設定

	sun->particles[0].pos = pos - (D3DXVECTOR3)light->lightSun.Direction * GR_FAR_Z / 2;

	if (light->isDayLight) {
		sun->Draw(pCamera);
	}
}
