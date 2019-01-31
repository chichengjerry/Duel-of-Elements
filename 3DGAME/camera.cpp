
#include "camera.h"
#include "player.h"

//*****************************************************************************
// �ޥ����x
//*****************************************************************************
#define	VALUE_MOVE_CAMERA	(2.0f)					// �������Ƅ���
#define	VALUE_ROTATE_CAMERA	(D3DX_PI * 0.01f)		// �����λ�ܞ��


//*****************************************************************************
// �ץ�ȥ���������
//*****************************************************************************


CAMERA::CAMERA(PLAYER* player)
{
	DWORD height = CL_HEIGHT;
	DWORD width = CL_WIDTH / 2;
	DWORD x = player->id * CL_WIDTH / 2;
	this->player = player;
	this->pitch = -45.0f;
	this->distance = 100.0f;
	this->viewport = { x, 0, width, height, 0.0f, 1.0f };

	Update();
}

CAMERA::~CAMERA()
{
}

D3DXMATRIX * CAMERA::GetView(void)
{
	return &m_view;
}

void CAMERA::SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = D3D::GetDevice();

	// �ӥ�`�ޥȥ�å����γ��ڻ�
	D3DXMatrixIdentity(&m_view);

	// �ӥ�`�ޥȥ�å���������
	D3DXMatrixLookAtLH(&m_view,
		&v_pos,		// ������ҕ��
		&v_tar,		// ������עҕ��
		&v_up);		// �������Ϸ���

	// �ץ����������ޥȥ�å����γ��ڻ�
	D3DXMatrixIdentity(&m_proj);

	// �ץ����������ޥȥ�å���������
	D3DXMatrixPerspectiveFovLH(&m_proj,
		D3DXToRadian((FLOAT)GR_VIEWANGLE),										// ҕҰ��
		((FLOAT)CL_WIDTH / (FLOAT)CL_HEIGHT),			// �����ڥ��ȱ�
		GR_NEAR_Z,									// �ӥ�`ƽ���NearZ��
		GR_FAR_Z);									// �ӥ�`ƽ���FarZ��

	pDevice->SetViewport(&viewport);

	// �ӥ�`�ޥȥ�å������O��
	pDevice->SetTransform(D3DTS_VIEW, &m_view);

	// �ץ����������ޥȥ�å������O��
	pDevice->SetTransform(D3DTS_PROJECTION, &m_proj);
}

void CAMERA::Update(void)
{
	v_tar = player->srt.pos;

	v_pos = v_tar - D3DXVECTOR3(distance, 0.0f, 0.0f);

	D3DXVECTOR4 tmp;
	D3DXMATRIX mtxRot;

	D3DXMatrixIdentity(&mtxRot);
	// ��ܞ��ӳ
	D3DXMatrixRotationYawPitchRoll(&mtxRot, player->srt.rot.y, D3DXToRadian(pitch), player->srt.rot.z);
	D3DXVec3Transform(&tmp, &v_pos, &mtxRot);

	v_pos = D3DXVECTOR3(tmp.x, tmp.y, tmp.z);
}
