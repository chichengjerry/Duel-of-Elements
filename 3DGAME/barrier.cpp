#include "barrier.h"
#include "player.h"

#define BARRIER_TEXTURE		_T("data/TEXTURE/forcefield.png")

BARRIER::BARRIER(TERRAIN * terrain)
{
	LPDIRECT3DDEVICE9 pDevice = D3D::GetDevice();

	if (FAILED(D3DXCreateTextureFromFile(
		pDevice,
		BARRIER_TEXTURE,
		&pTex))) {
		return;
	}

	SetVertex(terrain);
}

BARRIER::~BARRIER()
{
	SAFE_RELEASE(pVtx);
	SAFE_RELEASE(pTex);
}

void BARRIER::Update()
{
}

HRESULT BARRIER::Draw(CAMERA * camera)
{
	LPDIRECT3DDEVICE9 pDevice = D3D::GetDevice();

	//// ラインティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);	// 結果 = 転送元(SRC) + 転送先(DEST)
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	D3DXMATRIX mtx;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtx);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtx);

	// 頂点バッファをレンダリングパイプラインに設定
	pDevice->SetStreamSource(0, pVtx, 0, sizeof(VERTEX_3D));

	// インデックスバッファをレンダリングパイプラインに設定

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, pTex);

	for (int i = 0; i < 4; i++) {
		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * NUM_VERTEX, NUM_POLYGON);
	}

	// ライティングを有効に
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	// 通常ブレンドに戻す
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定

	return S_OK;
}

HRESULT BARRIER::SetVertex(TERRAIN * terrain)
{
	LPDIRECT3DDEVICE9 pDevice = D3D::GetDevice();

	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * 4,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
		FVF_VERTEX_3D,						// 使用する頂点フォーマット
		D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
		&pVtx,				// 頂点バッファインターフェースへのポインタ
		NULL)))								// NULLに設定
	{
		return E_FAIL;
	}

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

	DWORD indices[4][NUM_VERTEX] = {
		{ 0, 1, 4, 5 },
		{ 3, 2, 7, 6 },
		{ 2, 0, 6, 4 },
		{ 1, 3, 5, 7 }
	};

	FLOAT width = terrain->nMapSize * terrain->nBlockSize / 4.0f + PLAYER_SIZE;
	FLOAT height = GM_MAX_HEIGHT + PLAYER_SIZE;
	FLOAT textScale = width / 16.0f;

	VERTEX_3D* pTempVtx;

	pVtx->Lock(0, 0, (void**)&pTempVtx, 0);

	for (int i = 0; i < 4; i++) {

		pTempVtx[0].vtx = D3DXVECTOR3(width * vertex[indices[i][0]].x, height * vertex[indices[i][0]].y, width * vertex[indices[i][0]].z);
		pTempVtx[1].vtx = D3DXVECTOR3(width * vertex[indices[i][1]].x, height * vertex[indices[i][1]].y, width * vertex[indices[i][1]].z);
		pTempVtx[2].vtx = D3DXVECTOR3(width * vertex[indices[i][2]].x, height * vertex[indices[i][2]].y, width * vertex[indices[i][2]].z);
		pTempVtx[3].vtx = D3DXVECTOR3(width * vertex[indices[i][3]].x, height * vertex[indices[i][3]].y, width * vertex[indices[i][3]].z);
		
		pTempVtx[0].nor = 
		pTempVtx[1].nor = 
		pTempVtx[2].nor = 
		pTempVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		
		pTempVtx[0].dif = 
		pTempVtx[1].dif = 
		pTempVtx[2].dif = 
		pTempVtx[3].dif = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		
		pTempVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pTempVtx[1].tex = D3DXVECTOR2(0.0f, 1.0f);
		pTempVtx[2].tex = D3DXVECTOR2(1.0f, 0.0f);
		pTempVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pTempVtx += NUM_VERTEX;
	}

	pVtx->Unlock();

	return S_OK;
}
