#include "model.h"

void srt_init(SRT* srt)
{
	srt->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	srt->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	srt->scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
}

#define VTABLE BILLBOARDVTBL
static VTABLE billboard_vtable;
#undef VTABLE

HRESULT billboard_init(BILLBOARD * pBoard, LPCWSTR src)
{
	LPDIRECT3DDEVICE9 pDevice = NULL;

	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * NUM_VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&pBoard->pVtx,
		NULL)))	{
		return E_FAIL;
	}
	if (FAILED(D3DXCreateTextureFromFile(
		pDevice,
		src,			
		&pBoard->pTex))){
		return E_FAIL;
	}

	pBoard->lpVtbl = &billboard_vtable;

	return S_OK;
}

void billboard_uninit(BILLBOARD * pBoard)
{
	SAFE_RELEASE(pBoard->pTex);
	SAFE_RELEASE(pBoard->pVtx);
}

HRESULT billboard_draw(BILLBOARD * pBoard)
{
	LPDIRECT3DDEVICE9	pDevice = NULL;
	D3DXMATRIX			mtxView, mtxScale, mtxTranslate;

	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);				// Z比較なし
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&pBoard->mtx);

	// ビューマトリックスを取得
	// mtxView = GetMtxView();

	// 転置
	D3DXMatrixTranspose(&pBoard->mtx, &mtxView);
	pBoard->mtx._41 =
	pBoard->mtx._42 =
	pBoard->mtx._43 = 0.0f;

	// スケールを反映
	D3DXMatrixScaling(&mtxScale, pBoard->srt.scl.x, pBoard->srt.scl.y, pBoard->srt.scl.z);
	D3DXMatrixMultiply(&pBoard->mtx, &pBoard->mtx, &mtxScale);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTranslate, pBoard->srt.pos.x, pBoard->srt.pos.y, pBoard->srt.pos.z);
	D3DXMatrixMultiply(&pBoard->mtx, &pBoard->mtx, &mtxTranslate);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &pBoard->mtx);

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, pBoard->pVtx, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, pBoard->pTex);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, NUM_VERTEX, NUM_POLYGON);

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);				// Z比較あり

	return S_OK;
}

D3DXMATRIX * billboard_get_matrix(BILLBOARD * pBoard)
{
	return &pBoard->mtx;
}

SRT * billboard_get_srt(BILLBOARD * pBoard)
{
	return &pBoard->srt;
}


#define VTABLE MODELVTBL
static VTABLE model_vtable;
#undef VTABLE

HRESULT model_init(MODEL * pModel, LPCWSTR src)
{
	LPDIRECT3DDEVICE9 pDevice = NULL;

	if (FAILED(D3DXLoadMeshFromX(src,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&pModel->pMatBuf,
		NULL,
		&pModel->nMatNum,
		&pModel->pMesh)))
	{
		return E_FAIL;
	}

	srt_init(&pModel->srt);
	pModel->lpVtbl = &model_vtable;

	return S_OK;
}

void model_uninit(MODEL * pModel)
{
	SAFE_RELEASE(pModel->pMatBuf);
	SAFE_RELEASE(pModel->pMesh);
	SAFE_RELEASE(pModel->pTex);
}

HRESULT model_draw(MODEL * pModel)
{
	LPDIRECT3DDEVICE9 pDevice = NULL;
	D3DXMATRIX rot, pos;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 mat;

	// マテリアル情報一旦保存
	pDevice->GetMaterial(&mat);

	// マテリアル情報に対するポインタを取得
	pD3DXMat = (D3DXMATERIAL*)pModel->pMatBuf->GetBufferPointer();

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&pModel->mtx);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&rot, pModel->srt.rot.y, pModel->srt.rot.x, pModel->srt.rot.z);
	D3DXMatrixMultiply(&pModel->mtx, &pModel->mtx, &rot);

	// 移動を反映
	D3DXMatrixTranslation(&pos, pModel->srt.pos.x, pModel->srt.pos.y, pModel->srt.pos.z);
	D3DXMatrixMultiply(&pModel->mtx, &pModel->mtx, &pos);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &pModel->mtx);

	for (DWORD nMatCount = 0; nMatCount < pModel->nMatNum; nMatCount++, pD3DXMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pD3DXMat->MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, pModel->pTex);

		// 描画
		pModel->pMesh->DrawSubset(nMatCount);
	}

	// マテリアル情報回復
	pDevice->SetMaterial(&mat);

	return S_OK;
}

D3DXMATRIX * model_get_matrix(MODEL * pModel)
{
	return &pModel->mtx;
}

SRT * model_get_srt(MODEL * pModel)
{
	return &pModel->srt;
}
