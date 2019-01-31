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
	if (texSrc && FAILED(D3DXCreateTextureFromFile(pDevice,		// �ǥХ����ؤΥݥ���
		texSrc,											// �ե��������ǰ
		&pTex)))										// �i���z�����`
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

	// �ޥƥꥢ�����һ������
	pDevice->GetMaterial(&mat);

	// �ޥƥꥢ�����ˌ�����ݥ��󥿤�ȡ��
	pD3DXMat = (D3DXMATERIAL*)pMatBuf->GetBufferPointer();

	// ��`��ɥޥȥ�å����γ��ڻ�
	D3DXMatrixIdentity(&mtx);

	// ��ܞ��ӳ
	D3DXMatrixRotationYawPitchRoll(&rot, srt.rot.y, srt.rot.x, srt.rot.z);
	D3DXMatrixMultiply(&mtx, &mtx, &rot);

	// �ƄӤ�ӳ
	D3DXMatrixTranslation(&pos, srt.pos.x, srt.pos.y, srt.pos.z);
	D3DXMatrixMultiply(&mtx, &mtx, &pos);

	// ��`��ɥޥȥ�å������O��
	pDevice->SetTransform(D3DTS_WORLD, &mtx);

	for (DWORD nMatCount = 0; nMatCount < nMatNum; nMatCount++, pD3DXMat++)
	{

		// �ޥƥꥢ����O��
		pD3DXMat->MatD3D.Diffuse = col;
		pDevice->SetMaterial(&pD3DXMat->MatD3D);

		// �ƥ���������O��
		pDevice->SetTexture(0, pTex);

		// �軭
		pMesh->DrawSubset(nMatCount);
	}

	// �ޥƥꥢ�����؏�
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
