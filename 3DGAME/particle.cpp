#include "particle.h"
#include "camera.h"

EMITTER::EMITTER(LPCWSTR texturePath, D3DXVECTOR3 pos, D3DXVECTOR3 mov, D3DXCOLOR col, FLOAT size)
{
	LPDIRECT3DDEVICE9 pDevice = D3D::GetDevice();

	if (!pTex) {
		if (FAILED(D3DXCreateTextureFromFile(
			pDevice,
			texturePath,
			&pTex))) {
			return;
		};
	}

	this->pos = pos;
	this->mov = mov;
	this->col = col;
	this->size = size;
	
	bAlive = TRUE;

	for (DWORD i = 0; i < dwCount; i++) {
		particles[i].pos = pos;
		particles[i].mov = mov;
		particles[i].col = col;
	}
}

EMITTER::~EMITTER()
{
	SAFE_RELEASE(pTex);
}

void EMITTER::Update()
{
	for (DWORD i = 0; i < dwCount; i++) {
		particles[i].pos += particles[i].mov;
	}
}

HRESULT EMITTER::SetVertex()
{
	LPDIRECT3DDEVICE9 pDevice = D3D::GetDevice();
	VERTEX_3D *pTempVtx;

	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * GM_MAX_PARTICLES,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,							// 頂点バッファの使用法　
		FVF_VERTEX_3D,								// 使用する頂点フォーマット
		D3DPOOL_MANAGED,							// リソースのバッファを保持するメモリクラスを指定
		&pVtx,					// 頂点バッファインターフェースへのポインタ
		NULL)))										// NULLに設定
	{
		return E_FAIL;
	}

	pVtx->Lock(0, dwCount * sizeof(VERTEX_3D), (void**)&pTempVtx, 0);

	for (DWORD i = 0; i < dwCount; i++) {
		{
			pTempVtx[0].vtx = D3DXVECTOR3(-size / 2.0f, -size / 2.0f, 0.0f);
			pTempVtx[1].vtx = D3DXVECTOR3(-size / 2.0f,  size / 2.0f, 0.0f);
			pTempVtx[2].vtx = D3DXVECTOR3( size / 2.0f, -size / 2.0f, 0.0f);
			pTempVtx[3].vtx = D3DXVECTOR3( size / 2.0f,  size / 2.0f, 0.0f);

			pTempVtx[0].dif =
			pTempVtx[1].dif =
			pTempVtx[2].dif =
			pTempVtx[3].dif = particles[i].col;
			
			pTempVtx[0].nor =
			pTempVtx[1].nor =
			pTempVtx[2].nor =
			pTempVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// テクスチャ座標の設定
			pTempVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pTempVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pTempVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pTempVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			pTempVtx++;
		}
	}
	pVtx->Unlock();

	return S_OK;
}

HRESULT EMITTER::Draw(CAMERA* camera)
{
	LPDIRECT3DDEVICE9 pDevice = D3D::GetDevice();

	// αテストを有効に
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// ラインティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	// 加算合成
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);	// 結果 = 転送元(SRC) + 転送先(DEST)
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	D3DXMATRIX mtx, mtxView, mtxTrans;

	mtxView = *camera->GetView();

	for (DWORD i = 0; i < dwCount; i++) {
		if (particles[i].bAlive) {
			D3DXMatrixIdentity(&mtx);
			D3DXMatrixTranspose(&mtx, &mtxView);
			mtx._14 = 0.0f;
			mtx._24 = 0.0f;
			mtx._34 = 0.0f;

			// 移動を反映
			D3DXMatrixTranslation(&mtxTrans, particles[i].pos.x, particles[i].pos.y, particles[i].pos.z);
			D3DXMatrixMultiply(&mtx, &mtx, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &mtx);

			// 頂点バッファをデバイスのデータストリームにバインド
			pDevice->SetStreamSource(0, pVtx, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, pTex);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (i * 4), NUM_POLYGON);
		}
	}
	// ライティングを有効に
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// 通常ブレンドに戻す
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	return S_OK;
}
