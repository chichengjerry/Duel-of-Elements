#include "barrier.h"
#include "player.h"

#define BARRIER_TEXTURE		_T("data/TEXTURE/forcefield.png")

BARRIER::BARRIER(TERRAIN * terrain)
{
	LPDIRECT3DDEVICE9 pDevice = D3D::GetDevice();

	if (FAILED(D3DXCreateTextureFromFile(
		pDevice,
		BARRIER_TEXTURE,
		&pTex))) {
		return;
	}

	SetVertex(terrain);
}

BARRIER::~BARRIER()
{
	SAFE_RELEASE(pVtx);
	SAFE_RELEASE(pTex);
}

void BARRIER::Update()
{
}

HRESULT BARRIER::Draw(CAMERA * camera)
{
	LPDIRECT3DDEVICE9 pDevice = D3D::GetDevice();

	//// ���C���e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);	// ���� = �]����(SRC) + �]����(DEST)
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	D3DXMATRIX mtx;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtx);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtx);

	// ���_�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
	pDevice->SetStreamSource(0, pVtx, 0, sizeof(VERTEX_3D));

	// �C���f�b�N�X�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, pTex);

	for (int i = 0; i < 4; i++) {
		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * NUM_VERTEX, NUM_POLYGON);
	}

	// ���C�e�B���O��L����
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	// �ʏ�u�����h�ɖ߂�
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��

	return S_OK;
}

HRESULT BARRIER::SetVertex(TERRAIN * terrain)
{
	LPDIRECT3DDEVICE9 pDevice = D3D::GetDevice();

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * 4,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,					// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,						// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,					// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&pVtx,				// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))								// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	D3DXVECTOR3 vertex[8] = {
		{-1.0f,  1.0f,  1.0f},
		{ 1.0f,  1.0f,  1.0f},
		{-1.0f,  1.0f, -1.0f},
		{ 1.0f,  1.0f, -1.0f},
		{-1.0f, -1.0f,  1.0f},
		{ 1.0f, -1.0f,  1.0f},
		{-1.0f, -1.0f, -1.0f},
		{ 1.0f, -1.0f, -1.0f},
	};

	DWORD indices[4][NUM_VERTEX] = {
		{ 0, 1, 4, 5 },
		{ 3, 2, 7, 6 },
		{ 2, 0, 6, 4 },
		{ 1, 3, 5, 7 }
	};

	FLOAT width = terrain->nMapSize * terrain->nBlockSize / 4.0f + PLAYER_SIZE;
	FLOAT height = GM_MAX_HEIGHT + PLAYER_SIZE;
	FLOAT textScale = width / 16.0f;

	VERTEX_3D* pTempVtx;

	pVtx->Lock(0, 0, (void**)&pTempVtx, 0);

	for (int i = 0; i < 4; i++) {

		pTempVtx[0].vtx = D3DXVECTOR3(width * vertex[indices[i][0]].x, height * vertex[indices[i][0]].y, width * vertex[indices[i][0]].z);
		pTempVtx[1].vtx = D3DXVECTOR3(width * vertex[indices[i][1]].x, height * vertex[indices[i][1]].y, width * vertex[indices[i][1]].z);
		pTempVtx[2].vtx = D3DXVECTOR3(width * vertex[indices[i][2]].x, height * vertex[indices[i][2]].y, width * vertex[indices[i][2]].z);
		pTempVtx[3].vtx = D3DXVECTOR3(width * vertex[indices[i][3]].x, height * vertex[indices[i][3]].y, width * vertex[indices[i][3]].z);
		
		pTempVtx[0].nor = 
		pTempVtx[1].nor = 
		pTempVtx[2].nor = 
		pTempVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		
		pTempVtx[0].dif = 
		pTempVtx[1].dif = 
		pTempVtx[2].dif = 
		pTempVtx[3].dif = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		
		pTempVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pTempVtx[1].tex = D3DXVECTOR2(0.0f, 1.0f);
		pTempVtx[2].tex = D3DXVECTOR2(1.0f, 0.0f);
		pTempVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pTempVtx += NUM_VERTEX;
	}

	pVtx->Unlock();

	return S_OK;
}
