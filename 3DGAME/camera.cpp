//=============================================================================
//
// �����I�� [camera.cpp]
//
//=============================================================================
#include "camera.h"
#include "player.h"

//*****************************************************************************
// �ޥ����x
//*****************************************************************************
#define	VALUE_MOVE_CAMERA	(2.0f)					// �������Ƅ���
#define	VALUE_ROTATE_CAMERA	(D3DX_PI * 0.01f)		// �����λ�ܞ��

#define	INTERVAL_CAMERA_R	(12.5f)					// ��ǥ��ҕ�����ȤޤǤξ��x
#define	RATE_CHASE_CAMERA_P	(0.35f)					// ������ҕ��ؤ��a���S��
#define	RATE_CHASE_CAMERA_R	(0.20f)					// ������עҕ��ؤ��a���S��

#define	CHASE_HEIGHT_P		(100.0f)				// ׷�E�r��ҕ��θߤ�
#define	CHASE_HEIGHT_R		(10.0f)					// ׷�E�r��עҕ��θߤ�

//*****************************************************************************
// �ץ�ȥ���������
//*****************************************************************************


CAMERA::CAMERA(D3DXVECTOR3 pos, PLAYER* player)
{
	p_player = player;
	v_pos = pos;
	v_tar = player->srt.pos;

	DWORD height = CL_HEIGHT;
	DWORD width = CL_WIDTH / 2;
	DWORD x = player->id * CL_WIDTH / 2;
	viewport = { x, 0, width, height, 0.0f, 1.0f };
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
	v_tar = p_player->srt.pos;
}

CAMERAS::CAMERAS(MAINGAME * pGame)
{
	p_game = pGame;
}
