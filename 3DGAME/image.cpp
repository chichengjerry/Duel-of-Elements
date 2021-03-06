//-----------------------------------------------
// [image.h]
// 2D画像処理
// 作者 Chicheng Zhang
//-----------------------------------------------
#include "image.h"

//
// コンストラクター
//
IMAGE::IMAGE(LPDIRECT3DTEXTURE9 pTex = NULL, D3DRECT* rect = NULL)
{
	this->pTex = pTex;
	this->rot = 0.0f;
	this->scl = 1.0f;

	LPDIRECT3DDEVICE9 pDevice = D3D::GetDevice();

	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,								// 頂点バッファの使用法　
		FVF_VERTEX_2D,									// 使用する頂点フォーマット
		D3DPOOL_MANAGED,								// リソースのバッファを保持するメモリクラスを指定
		&pVtx,							// 頂点バッファインターフェースへのポインタ
		NULL)))											// NULLに設定
	{
		return;
	}
	nFrameIndex = 0;
	nFrameTotal = 1;
	SetVertex(rect);
	SetDiffuseColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	SetTexture();
}

//
// デストラクター
//
IMAGE::~IMAGE()
{
	SAFE_RELEASE(pVtx);
}

//
// 頂点情報を設置する。
//
HRESULT IMAGE::SetVertex(D3DRECT* rect)
{
	VERTEX_2D *pTempVtx;

	D3DXVECTOR3 vtx[4] = {
		D3DXVECTOR3((FLOAT)(rect->x1),				(FLOAT)(rect->y1),				0.0f),
		D3DXVECTOR3((FLOAT)(rect->x1 + rect->x2),	(FLOAT)(rect->y1),				0.0f),
		D3DXVECTOR3((FLOAT)(rect->x1),				(FLOAT)(rect->y1 + rect->y2),	0.0f),
		D3DXVECTOR3((FLOAT)(rect->x1 + rect->x2),	(FLOAT)(rect->y1 + rect->y2),	0.0f),
	};

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	pVtx->Lock(0, 0, (void**)&pTempVtx, 0);

	pTempVtx[0].vtx	= vtx[0];
	pTempVtx[1].vtx = vtx[1];
	pTempVtx[2].vtx = vtx[2];
	pTempVtx[3].vtx = vtx[3];

	pTempVtx[0].rhw = 
	pTempVtx[1].rhw = 
	pTempVtx[2].rhw = 
	pTempVtx[3].rhw = 1.0f;

	pVtx->Unlock();

	return S_OK;
}

//
// テクスチャを設置する。
//
HRESULT IMAGE::SetTexture()
{
	VERTEX_2D *pTempVtx;

	FLOAT inv = 1.0f / nFrameTotal;
	FLOAT p = (FLOAT)nFrameIndex * inv;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	pVtx->Lock(0, 0, (void**)&pTempVtx, 0);

	pTempVtx[0].tex = D3DXVECTOR2(p, 0.0f);
	pTempVtx[1].tex = D3DXVECTOR2(p + inv, 0.0f);
	pTempVtx[2].tex = D3DXVECTOR2(p, 1.0f);
	pTempVtx[3].tex = D3DXVECTOR2(p + inv, 1.0f);

	pVtx->Unlock();

	return S_OK;
}

//
// 色を設置する。
//
HRESULT IMAGE::SetDiffuseColor(D3DXCOLOR color)
{
	//if (this->col == color) {
	//	return S_OK;
	//}
	this->col = color;

	VERTEX_2D *pTempVtx;

	pVtx->Lock(0, 0, (void**)&pTempVtx, 0);

	pTempVtx[0].dif = 
	pTempVtx[1].dif = 
	pTempVtx[2].dif = 
	pTempVtx[3].dif = col;

	pVtx->Unlock();

	return S_OK;
}

//
// 画像を描画する。
//
HRESULT IMAGE::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = D3D::GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, pVtx, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, pTex);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

	return S_OK;
}

//
// 画像を更新する。
//
void IMAGE::Update()
{
	if (bAnimating) {
		nFrameIndex = (nFrameIndex + 1) % nFrameTotal;
	}
}
