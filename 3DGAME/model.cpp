#include "model.h"

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
		texSrc,											// ファイルの名前
		&pTex)))										// 読み込むメモリー
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

	// マテリアル情報一旦保存
	pDevice->GetMaterial(&mat);

	// マテリアル情報に対するポインタを取得
	pD3DXMat = (D3DXMATERIAL*)pMatBuf->GetBufferPointer();

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtx);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&rot, srt.rot.y, srt.rot.x, srt.rot.z);
	D3DXMatrixMultiply(&mtx, &mtx, &rot);

	// 移動を反映
	D3DXMatrixTranslation(&pos, srt.pos.x, srt.pos.y, srt.pos.z);
	D3DXMatrixMultiply(&mtx, &mtx, &pos);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtx);

	for (DWORD nMatCount = 0; nMatCount < nMatNum; nMatCount++, pD3DXMat++)
	{

		// マテリアルの設定
		pD3DXMat->MatD3D.Diffuse = col;
		pDevice->SetMaterial(&pD3DXMat->MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, pTex);

		// 描画
		pMesh->DrawSubset(nMatCount);
	}

	// マテリアル情報回復
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
