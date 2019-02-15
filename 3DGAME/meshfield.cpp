#include "meshfield.h"

COLORMAP::COLORMAP(DWORD steps, COLORSTEP *cols)
{
	if (steps < 2) {
		return;
	}
	else {
		this->steps = steps;
		this->mCols = new COLORSTEP[steps];
		for (int i = 0; i < steps; i++) {
			this->mCols[i] = cols[i];
		}
	}
}

D3DXCOLOR COLORMAP::GetColor(FLOAT t)
{
	if (t <= mCols[0].t)
		return mCols[0].color;
	if (t >= mCols[steps - 1].t)
		return mCols[steps - 1].color;

	for (int i = 0; i < steps - 1; i++) {
		if (t >= mCols[i].t && t <= mCols[i + 1].t) {

			// Liner Interpolate
			FLOAT dif = mCols[i + 1].t - mCols[i].t;

			if (dif <= 0.0f) {
				return mCols[i + 1].color;
			}
			t = (t - mCols[i].t) / dif;

			return mCols[i].color * (1 - t) + mCols[i + 1].color * t;
		}
	}

	return D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
}

MESHFIELD::MESHFIELD(DWORD mapSizeX, DWORD mapSizeY, DWORD blockSizeX, DWORD blockSizeY, FLOAT minHeight, FLOAT maxHeight)
{
	pVtxBuf = NULL;
	pIdxBuf = NULL;
	pTexBuf = NULL;

	nBlkSizeX = blockSizeX;
	nBlkSizeZ = blockSizeY;
	nMapSizeX = mapSizeX;
	nMapSizeZ = mapSizeY;
	fMinHeight = minHeight;
	fMaxHeight = maxHeight;
	dwVtxCount = (nMapSizeX + 1) * (nMapSizeZ + 1);
	dwIdxCount = (nMapSizeZ * ((nMapSizeX + 1) + 1) - 1) * 2;
	dwPolyCount = nMapSizeZ * (nMapSizeX * 2 + 4) - 4;
}

MESHFIELD::~MESHFIELD()
{
	SAFE_RELEASE(pVtxBuf);
	SAFE_RELEASE(pIdxBuf);
	SAFE_RELEASE(pTexBuf);
}

HRESULT MESHFIELD::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = D3D::GetDevice();

	D3DXMATRIX mtx;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtx);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtx);

	// ���_�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
	pDevice->SetStreamSource(0, pVtxBuf, 0, sizeof(VERTEX_3D));

	// �C���f�b�N�X�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
	pDevice->SetIndices(pIdxBuf);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, pTexBuf);

	// �|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, dwVtxCount, 0, dwPolyCount);

	return S_OK;
}

void MESHFIELD::Update()
{
}

HRESULT MESHFIELD::SetTexture(LPCWSTR textureSrc)
{
	LPDIRECT3DDEVICE9 pDevice = D3D::GetDevice();
	if (FAILED(D3DXCreateTextureFromFile(
		pDevice,
		textureSrc,
		&pTexBuf))) {
		return E_FAIL;
	}
	return S_OK;
}

HRESULT MESHFIELD::SetVertex(BYTE* pHeightMap, COLORMAP* colorMap)
{
	LPDIRECT3DDEVICE9 pDevice = D3D::GetDevice();

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * dwVtxCount,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,					// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,						// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,					// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&pVtxBuf,				// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))								// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	// �I�u�W�F�N�g�̃C���f�b�N�X�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateIndexBuffer(sizeof(DWORD) * dwIdxCount,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,					// ���_�o�b�t�@�̎g�p�@�@
		D3DFMT_INDEX32,						// �g�p����C���f�b�N�X�t�H�[�}�b�g
		D3DPOOL_MANAGED,					// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&pIdxBuf,				// �C���f�b�N�X�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))								// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	VERTEX_3D *pVtx;
	FLOAT texSizeX = 64.0f / nMapSizeX;
	FLOAT texSizeZ = 64.0f / nMapSizeZ;

	// ���_�쐬
	pVtxBuf->Lock(0, 0, (void**)&pVtx, 0);

	for (int z = 0; z < (nMapSizeZ + 1); z++){
		for (int x = 0; x < (nMapSizeX + 1); x++){
			FLOAT height = fMinHeight + (fMaxHeight - fMinHeight) * (pHeightMap[z * (nMapSizeX + 1) + x] / 255.0f);
			D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// Set Vertex
			pVtx[z * (nMapSizeX + 1) + x].vtx = D3DXVECTOR3((x - nMapSizeX / 2.0f) * nBlkSizeX, height, (z - nMapSizeZ/ 2.0f) * nBlkSizeZ);

			// Set Normal
			// pVtx[z * (nMapSizeX + 1) + x].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// Set Color
			pVtx[z * (nMapSizeX + 1) + x].dif = GetDiffuse(pHeightMap[z * (nMapSizeX + 1) + x], colorMap);
			
			// Set Texture
			pVtx[z * (nMapSizeX + 1) + x].tex = D3DXVECTOR2(texSizeX * x, texSizeZ * z);
		}
	}

	for (int z = 0; z < (nMapSizeZ + 1); z++) {
		for (int x = 0; x < (nMapSizeX + 1); x++) {
#define	GetVtx(x, z)	((x) < 0 || (z) < 0 || (x) > nMapSizeX || (z) > nMapSizeZ) ? 0 : pVtx[(z) * (nMapSizeX + 1) + (x)].vtx.y
			FLOAT hL = GetVtx(x - 1, z);
			FLOAT hR = GetVtx(x + 1, z);
			FLOAT hD = GetVtx(x, z - 1);
			FLOAT hU = GetVtx(x, z + 1);
#undef GetVtx
			// deduce terrain normal
			D3DXVECTOR3 nor = D3DXVECTOR3(hL - hR, 2.0f, hD - hU);
			D3DXVec3Normalize(&nor, &nor);

			pVtx[z * (nMapSizeX + 1) + x].nor = nor;
		}
	}
	pVtxBuf->Unlock();

	// �C���f�b�N�X�쐬
	DWORD* pIdx;
	DWORD index = 0;

	pIdxBuf->Lock(0, sizeof(DWORD), (void**)&pIdx, 0);

	for (int z = 0; z < nMapSizeZ; z++) {

		// �k�ރ|���S���̂��߂̃_�u��̐ݒ�
		if (z > 0) {
			pIdx[index++] = z * (nMapSizeX + 1);
		}

		for (int x = 0; x < nMapSizeX + 1; x++) {
			pIdx[index++] = z * (nMapSizeX + 1) + x;
			pIdx[index++] = (z + 1) * (nMapSizeX + 1) + x;
		}

		// �k�ރ|���S���̂��߂̃_�u��̐ݒ�
		if (z < (nMapSizeZ - 1))
		{
			pIdx[index++] = (z + 1) * (nMapSizeX + 1) + nMapSizeX;
		}
	}
	pIdxBuf->Unlock();

	return S_OK;
}

D3DXCOLOR MESHFIELD::GetDiffuse(BYTE height, COLORMAP* colorMap)
{
	FLOAT t = height / 255.0f;
	if (colorMap)
		return colorMap->GetColor(t);
	else
		return D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}

FLOAT MESHFIELD::GetPositionHeight(FLOAT fx, FLOAT fz)
{
	FLOAT height = 0.0f;

	fx /= nBlkSizeX;
	fz /= nBlkSizeZ;

	INT gx = (DWORD)floorf(fx);
	INT gz = (DWORD)floorf(fz);

	gx = gx <= -nMapSizeX / 2 ? -nMapSizeX / 2 : gx;
	gz = gz <= -nMapSizeZ / 2 ? -nMapSizeZ / 2 : gz;
	gx = gx >= nMapSizeX / 2 ? nMapSizeX / 2 - 1 : gx;
	gz = gz >= nMapSizeZ / 2 ? nMapSizeX / 2 - 1 : gz;

	fx -= gx;
	fz -= gz;
	gx += nMapSizeX / 2;
	gz += nMapSizeZ / 2;

	VERTEX_3D* pVtx;
	pVtxBuf->Lock(0, sizeof(VERTEX_3D), (void**)&pVtx, 0);

	FLOAT lt, rt, ld, rd;
	lt = pVtx[gz * (nMapSizeX + 1) + gx].vtx.y;
	rt = pVtx[(gz + 1) * (nMapSizeX + 1) + gx].vtx.y;
	ld = pVtx[(gz)* (nMapSizeX + 1) + gx + 1].vtx.y;
	rd = pVtx[(gz + 1) * (nMapSizeX + 1) + gx + 1].vtx.y;

	if (fx < 1.0f - fz) {
		height = BarryCentric(
			D3DXVECTOR3(0.0f, lt, 0.0f),
			D3DXVECTOR3(0.0f, rt, 1.0f),
			D3DXVECTOR3(1.0f, ld, 0.0f),
			D3DXVECTOR2(fx, fz));
	}
	else {
		height = BarryCentric(
			D3DXVECTOR3(0.0f, rt, 1.0f),
			D3DXVECTOR3(1.0f, rd, 1.0f),
			D3DXVECTOR3(1.0f, ld, 0.0f),
			D3DXVECTOR2(fx, fz));
	}

	return height;
}

BOOL MESHFIELD::OutOfBound(FLOAT fx, FLOAT fz)
{
	FLOAT posXmax = (FLOAT)(nBlkSizeX * nMapSizeX / 2);
	FLOAT posZmax = (FLOAT)(nBlkSizeZ * nMapSizeZ / 2);
	FLOAT posXmin = -posXmax;
	FLOAT posZmin = -posZmax;

	return (fx > posXmax || fx < posXmin || fz > posZmax || fz < posZmin);
}
