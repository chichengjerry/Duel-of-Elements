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
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);				// Z���^�ʤ�
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ��`��ɥޥȥ�å����γ��ڻ�
	D3DXMatrixIdentity(&pBoard->mtx);

	// �ӥ�`�ޥȥ�å�����ȡ��
	// mtxView = GetMtxView();

	// ܞ��
	D3DXMatrixTranspose(&pBoard->mtx, &mtxView);
	pBoard->mtx._41 =
	pBoard->mtx._42 =
	pBoard->mtx._43 = 0.0f;

	// �����`���ӳ
	D3DXMatrixScaling(&mtxScale, pBoard->srt.scl.x, pBoard->srt.scl.y, pBoard->srt.scl.z);
	D3DXMatrixMultiply(&pBoard->mtx, &pBoard->mtx, &mtxScale);

	// �ƄӤ�ӳ
	D3DXMatrixTranslation(&mtxTranslate, pBoard->srt.pos.x, pBoard->srt.pos.y, pBoard->srt.pos.z);
	D3DXMatrixMultiply(&pBoard->mtx, &pBoard->mtx, &mtxTranslate);

	// ��`��ɥޥȥ�å������O��
	pDevice->SetTransform(D3DTS_WORLD, &pBoard->mtx);

	// 픵�Хåե���ǥХ����Υǩ`�����ȥ�`��˥Х����
	pDevice->SetStreamSource(0, pBoard->pVtx, 0, sizeof(VERTEX_3D));

	// 픵�ե��`�ޥåȤ��O��
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �ƥ���������O��
	pDevice->SetTexture(0, pBoard->pTex);

	// �ݥꥴ����軭
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, NUM_VERTEX, NUM_POLYGON);

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);				// Z���^����

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

	// �ޥƥꥢ�����һ������
	pDevice->GetMaterial(&mat);

	// �ޥƥꥢ�����ˌ�����ݥ��󥿤�ȡ��
	pD3DXMat = (D3DXMATERIAL*)pModel->pMatBuf->GetBufferPointer();

	// ��`��ɥޥȥ�å����γ��ڻ�
	D3DXMatrixIdentity(&pModel->mtx);

	// ��ܞ��ӳ
	D3DXMatrixRotationYawPitchRoll(&rot, pModel->srt.rot.y, pModel->srt.rot.x, pModel->srt.rot.z);
	D3DXMatrixMultiply(&pModel->mtx, &pModel->mtx, &rot);

	// �ƄӤ�ӳ
	D3DXMatrixTranslation(&pos, pModel->srt.pos.x, pModel->srt.pos.y, pModel->srt.pos.z);
	D3DXMatrixMultiply(&pModel->mtx, &pModel->mtx, &pos);

	// ��`��ɥޥȥ�å������O��
	pDevice->SetTransform(D3DTS_WORLD, &pModel->mtx);

	for (DWORD nMatCount = 0; nMatCount < pModel->nMatNum; nMatCount++, pD3DXMat++)
	{
		// �ޥƥꥢ����O��
		pDevice->SetMaterial(&pD3DXMat->MatD3D);

		// �ƥ���������O��
		pDevice->SetTexture(0, pModel->pTex);

		// �軭
		pModel->pMesh->DrawSubset(nMatCount);
	}

	// �ޥƥꥢ�����؏�
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
