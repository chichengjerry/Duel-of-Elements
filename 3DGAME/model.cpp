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

	// ���֥������Ȥ�픵�Хåե�������
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,	// 픵�ǩ`���ä˴_������Хåե�������(�Х��ȅgλ)
		D3DUSAGE_WRITEONLY,			// 픵�Хåե���ʹ�÷���
		FVF_VERTEX_2D,				// ʹ�ä���픵�ե��`�ޥå�
		D3DPOOL_MANAGED,			// �꥽�`���ΥХåե��򱣳֤�����ꥯ�饹��ָ��
		&pVtx,		// 픵�Хåե����󥿩`�ե��`���ؤΥݥ���
		NULL)))						// NULL���O��
	{
		return E_FAIL;
	}

	//픵�Хåե������������
	VERTEX_3D* pVertices;

	// 픵�ǩ`���ι�����å�����픵�Хåե��ؤΥݥ��󥿤�ȡ��
	pVtx->Lock(0, 0, (void**)&pVertices, 0);

	// 픵����ˤ��O��
	pVertices[0].vtx = vtx[0];
	pVertices[1].vtx = vtx[1];
	pVertices[2].vtx = vtx[2];
	pVertices[3].vtx = vtx[3];

	// �������O��
	pVertices[0].dif =
	pVertices[1].dif =
	pVertices[2].dif =
	pVertices[3].dif = col;

	// �ƥ����������ˤ��O��
	pVertices[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVertices[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVertices[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVertices[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 픵�ǩ`���򥢥��å�����
	pVtx->Unlock();
}

HRESULT POLYGON::Draw(CAMERA * pCamera)
{
	LPDIRECT3DDEVICE9	pDevice = D3D::GetDevice();
	D3DXMATRIX			mtxView, mtxScale, mtxRot, mtxTranslate;

	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);				// Z���^�ʤ�
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ��`��ɥޥȥ�å����γ��ڻ�
	D3DXMatrixIdentity(&mtx);

	if (pCamera && isBillboard) {
		// �ӥ�`�ޥȥ�å�����ȡ��

		mtxView = *(pCamera->GetView());

		// ܞ��
		D3DXMatrixTranspose(&mtx, &mtxView);
		mtx._41 =
			mtx._42 =
			mtx._43 = 0.0f;
	}

	// ��ܞ��ӳ
	D3DXMatrixRotationYawPitchRoll(&mtxRot, srt.rot.y, srt.rot.x, srt.rot.z);
	D3DXMatrixMultiply(&mtx, &mtx, &mtxRot);

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
