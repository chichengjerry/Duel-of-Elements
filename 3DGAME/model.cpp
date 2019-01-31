#include "model.h"
#include "camera.h"
#include "d3d.h"

POLYGON::POLYGON(LPCWSTR src, SRT srt, BOOL isBillboard = FALSE)
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

	this->srt = srt;
	this->isBillboard = isBillboard;
	D3DXMatrixIdentity(&this->mtx);
}

POLYGON::~POLYGON()
{
	SAFE_DELETE(pVtx);
	SAFE_DELETE(pTex);
}

HRESULT POLYGON::Draw(CAMERA * pCamera)
{
	LPDIRECT3DDEVICE9	pDevice = NULL;
	D3DXMATRIX			mtxView, mtxScale, mtxTranslate;

	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);				// Z���^�ʤ�
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ��`��ɥޥȥ�å����γ��ڻ�
	D3DXMatrixIdentity(&mtx);

	if (isBillboard) {
		// �ӥ�`�ޥȥ�å�����ȡ��

		mtxView = *pCamera->GetView();

		// ܞ��
		D3DXMatrixTranspose(&mtx, &mtxView);
		mtx._41 =
			mtx._42 =
			mtx._43 = 0.0f;
	}

	// �����`���ӳ
	D3DXMatrixScaling(&mtxScale, srt.scl.x, srt.scl.y, srt.scl.z);
	D3DXMatrixMultiply(&mtx, &mtx, &mtxScale);

	// �ƄӤ�ӳ
	D3DXMatrixTranslation(&mtxTranslate, srt.pos.x, srt.pos.y, srt.pos.z);
	D3DXMatrixMultiply(&mtx, &mtx, &mtxTranslate);

	// ��`��ɥޥȥ�å������O��
	pDevice->SetTransform(D3DTS_WORLD, &mtx);

	// 픵�Хåե���ǥХ����Υǩ`�����ȥ�`��˥Х����
	pDevice->SetStreamSource(0, pVtx, 0, sizeof(VERTEX_3D));

	// 픵�ե��`�ޥåȤ��O��
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �ƥ���������O��
	pDevice->SetTexture(0, pTex);

	// �ݥꥴ����軭
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, NUM_VERTEX, NUM_POLYGON);

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);				// Z���^����

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

MODEL::MODEL(LPCWSTR src, SRT srt)
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
		return ;
	}
}

MODEL::~MODEL()
{
	SAFE_RELEASE(pMatBuf);
	SAFE_RELEASE(pMesh);
	SAFE_RELEASE(pTex);
}

HRESULT MODEL::Draw(CAMERA * pCamera)
{

	LPDIRECT3DDEVICE9 pDevice = NULL;
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
