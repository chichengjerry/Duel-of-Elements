#include "model.h"
#include "camera.h"
#include "d3d.h"

POLYGON::POLYGON(LPCWSTR src, D3DXVECTOR3 vertices[])
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
	if (FAILED(D3DXCreateTextureFromFile(
		pDevice,
		src,
		&pTex))) {
		return;
	}
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

	// オブジェクトの��泣バッファを伏撹
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,	// ��泣デ�`タ喘に�_隠するバッファサイズ(バイト�g了)
		D3DUSAGE_WRITEONLY,			// ��泣バッファの聞喘隈　
		FVF_VERTEX_2D,				// 聞喘する��泣フォ�`マット
		D3DPOOL_MANAGED,			// リソ�`スのバッファを隠隔するメモリクラスを峺協
		&pVtx,		// ��泣バッファインタ�`フェ�`スへのポインタ
		NULL)))						// NULLに�O協
	{
		return E_FAIL;
	}

	//��泣バッファの嶄附を托める
	VERTEX_3D* pVertices;

	// ��泣デ�`タの���譴鬟蹈奪�し、��泣バッファへのポインタを函誼
	pVtx->Lock(0, 0, (void**)&pVertices, 0);

	// ��泣恙�砲淋O協
	pVertices[0].vtx = vtx[0];
	pVertices[1].vtx = vtx[1];
	pVertices[2].vtx = vtx[2];
	pVertices[3].vtx = vtx[3];

	// 郡符高の�O協
	pVertices[0].dif =
	pVertices[1].dif =
	pVertices[2].dif =
	pVertices[3].dif = col;

	// テクスチャ恙�砲淋O協
	pVertices[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVertices[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVertices[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVertices[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ��泣デ�`タをアンロックする
	pVtx->Unlock();
}

HRESULT POLYGON::Draw(CAMERA * pCamera)
{
	LPDIRECT3DDEVICE9	pDevice = D3D::GetDevice();
	D3DXMATRIX			mtxView, mtxScale, mtxRot, mtxTranslate;

	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);				// Z曳�^なし
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ワ�`ルドマトリックスの兜豚晒
	D3DXMatrixIdentity(&mtx);

	if (pCamera && isBillboard) {
		// ビュ�`マトリックスを函誼

		mtxView = *(pCamera->GetView());

		// ��崔
		D3DXMatrixTranspose(&mtx, &mtxView);
		mtx._41 =
			mtx._42 =
			mtx._43 = 0.0f;
	}

	// 指��を郡啌
	D3DXMatrixRotationYawPitchRoll(&mtxRot, srt.rot.y, srt.rot.x, srt.rot.z);
	D3DXMatrixMultiply(&mtx, &mtx, &mtxRot);

	// スケ�`ルを郡啌
	D3DXMatrixScaling(&mtxScale, srt.scl.x, srt.scl.y, srt.scl.z);
	D3DXMatrixMultiply(&mtx, &mtx, &mtxScale);

	// 卞�咾魴貫�
	D3DXMatrixTranslation(&mtxTranslate, srt.pos.x, srt.pos.y, srt.pos.z);
	D3DXMatrixMultiply(&mtx, &mtx, &mtxTranslate);

	// ワ�`ルドマトリックスの�O協
	pDevice->SetTransform(D3DTS_WORLD, &mtx);

	// ��泣バッファをデバイスのデ�`タストリ�`ムにバインド
	pDevice->SetStreamSource(0, pVtx, 0, sizeof(VERTEX_3D));

	// ��泣フォ�`マットの�O協
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの�O協
	pDevice->SetTexture(0, pTex);

	// ポリゴンの宙鮫
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, NUM_VERTEX, NUM_POLYGON);

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);				// Z曳�^あり

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

MODEL::MODEL(LPCWSTR modelSrc, LPCWSTR texSrc)
{
	this->nMatNum = 0;
	this->pMatBuf = NULL;
	this->pMesh = NULL;
	this->pTex = NULL;
	this->srt = SRT();
	this->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	LoadModel(modelSrc, texSrc);
}

MODEL::~MODEL()
{
	SAFE_RELEASE(pMatBuf);
	SAFE_RELEASE(pMesh);
	SAFE_RELEASE(pTex);
}

HRESULT MODEL::LoadModel(LPCWSTR src, LPCWSTR texSrc)
{
	LPDIRECT3DDEVICE9 pDevice = D3D::GetDevice();

	if (FAILED(D3DXLoadMeshFromX(src,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&pMatBuf,
		NULL,
		&nMatNum,
		&pMesh)))
	{
		return E_FAIL;
	}
	if (texSrc && FAILED(D3DXCreateTextureFromFile(pDevice,		// デバイスへのポインタ
		texSrc,											// ファイルの兆念
		&pTex)))										// �iみ�zむメモリ�`
	{
		return E_FAIL;
	}
	
	return S_OK;
}

HRESULT MODEL::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = D3D::GetDevice();
	D3DXMATRIX rot, pos;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 mat;

	// マテリアル秤�麭撒�隠贋
	pDevice->GetMaterial(&mat);

	// マテリアル秤�鵑���するポインタを函誼
	pD3DXMat = (D3DXMATERIAL*)pMatBuf->GetBufferPointer();

	// ワ�`ルドマトリックスの兜豚晒
	D3DXMatrixIdentity(&mtx);

	// 指��を郡啌
	D3DXMatrixRotationYawPitchRoll(&rot, srt.rot.y, srt.rot.x, srt.rot.z);
	D3DXMatrixMultiply(&mtx, &mtx, &rot);

	// 卞�咾魴貫�
	D3DXMatrixTranslation(&pos, srt.pos.x, srt.pos.y, srt.pos.z);
	D3DXMatrixMultiply(&mtx, &mtx, &pos);

	// ワ�`ルドマトリックスの�O協
	pDevice->SetTransform(D3DTS_WORLD, &mtx);

	for (DWORD nMatCount = 0; nMatCount < nMatNum; nMatCount++, pD3DXMat++)
	{

		// マテリアルの�O協
		pD3DXMat->MatD3D.Diffuse = col;
		pDevice->SetMaterial(&pD3DXMat->MatD3D);

		// テクスチャの�O協
		pDevice->SetTexture(0, pTex);

		// 宙鮫
		pMesh->DrawSubset(nMatCount);
	}

	// マテリアル秤�鷸���
	pDevice->SetMaterial(&mat);

	return S_OK;
}

D3DXMATRIX * MODEL::GetMatrix()
{
	return &mtx;
}

SRT * MODEL::GetSrt()
{
	return &srt;
}

void MODEL::Update()
{
}

SRT::SRT()
{
	scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

SRT::SRT(D3DXVECTOR3 pos)
{
	scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pos = pos;
}
