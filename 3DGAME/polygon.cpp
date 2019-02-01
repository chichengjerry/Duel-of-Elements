#include "polygon.h"

POLYGON::POLYGON(LPDIRECT3DTEXTURE9 pTex, D3DXVECTOR3 vertices[])
{
	LPDIRECT3DDEVICE9 pDevice = D3D::GetDevice();

	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * NUM_VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&pVtx,
		NULL))) {
		return;
	}
	this->pTex = pTex;
	this->srt = SRT();
	this->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	this->isBillboard = FALSE;
	memcpy(this->vtx, vertices, sizeof(D3DXVECTOR3) * 4);
	SetVertex(pDevice);
}

POLYGON::~POLYGON()
{
	SAFE_RELEASE(pVtx);
	SAFE_RELEASE(pTex);
}

HRESULT POLYGON::SetVertex(LPDIRECT3DDEVICE9 pDevice)
{

	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_2D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&pVtx,		// 頂点バッファインターフェースへのポインタ
		NULL)))						// NULLに設定
	{
		return E_FAIL;
	}

	//頂点バッファの中身を埋める
	VERTEX_3D* pVertices;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	pVtx->Lock(0, 0, (void**)&pVertices, 0);

	// 頂点座標の設定
	pVertices[0].vtx = vtx[0];
	pVertices[1].vtx = vtx[1];
	pVertices[2].vtx = vtx[2];
	pVertices[3].vtx = vtx[3];

	pVertices[0].nor =
	pVertices[1].nor =
	pVertices[2].nor =
	pVertices[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// 反射光の設定
	pVertices[0].dif =
	pVertices[1].dif =
	pVertices[2].dif =
	pVertices[3].dif = col;

	// テクスチャ座標の設定
	pVertices[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVertices[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVertices[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVertices[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点データをアンロックする
	pVtx->Unlock();

	return S_OK;
}

HRESULT POLYGON::Draw(CAMERA * pCamera)
{
	LPDIRECT3DDEVICE9	pDevice = D3D::GetDevice();
	D3DXMATRIX			mtxView, mtxScale, mtxRot, mtxTranslate;

	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);				// Z比較なし
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtx);

	if (pCamera && isBillboard) {
		// ビューマトリックスを取得

		mtxView = *(pCamera->GetView());

		// 転置
		D3DXMatrixTranspose(&mtx, &mtxView);
		mtx._41 =
			mtx._42 =
			mtx._43 = 0.0f;
	}

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, srt.rot.y, srt.rot.x, srt.rot.z);
	D3DXMatrixMultiply(&mtx, &mtx, &mtxRot);

	// スケールを反映
	D3DXMatrixScaling(&mtxScale, srt.scl.x, srt.scl.y, srt.scl.z);
	D3DXMatrixMultiply(&mtx, &mtx, &mtxScale);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTranslate, srt.pos.x, srt.pos.y, srt.pos.z);
	D3DXMatrixMultiply(&mtx, &mtx, &mtxTranslate);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtx);

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, pVtx, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, pTex);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, NUM_VERTEX, NUM_POLYGON);

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);				// Z比較あり

	return S_OK;
}

D3DXMATRIX * POLYGON::GetMatrix()
{
	return &mtx;
}

SRT * POLYGON::GetSrt()
{
	return &srt;
}

void POLYGON::Update()
{
}
