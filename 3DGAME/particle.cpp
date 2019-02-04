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

	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * GM_MAX_PARTICLES,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,							// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,								// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,							// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&pVtx,					// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))										// NULL�ɐݒ�
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

			// �e�N�X�`�����W�̐ݒ�
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

	// ���e�X�g��L����
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// ���C���e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	// ���Z����
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);	// ���� = �]����(SRC) + �]����(DEST)
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

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTrans, particles[i].pos.x, particles[i].pos.y, particles[i].pos.z);
			D3DXMatrixMultiply(&mtx, &mtx, &mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &mtx);

			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			pDevice->SetStreamSource(0, pVtx, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, pTex);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (i * 4), NUM_POLYGON);
		}
	}
	// ���C�e�B���O��L����
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// �ʏ�u�����h�ɖ߂�
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	return S_OK;
}
