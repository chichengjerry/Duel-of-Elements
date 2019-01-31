
#include "camera.h"
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	VALUE_MOVE_CAMERA	(2.0f)					// カメラの移動量
#define	VALUE_ROTATE_CAMERA	(D3DX_PI * 0.01f)		// カメラの回転量


//*****************************************************************************
// プロトタイプ宣言
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

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_view);

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH(&m_view,
		&v_pos,		// カメラの視点
		&v_tar,		// カメラの注視点
		&v_up);		// カメラの上方向

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_proj);

	// プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&m_proj,
		D3DXToRadian((FLOAT)GR_VIEWANGLE),										// 視野角
		((FLOAT)CL_WIDTH / (FLOAT)CL_HEIGHT),			// アスペクト比
		GR_NEAR_Z,									// ビュー平面のNearZ値
		GR_FAR_Z);									// ビュー平面のFarZ値

	pDevice->SetViewport(&viewport);

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_view);

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_proj);
}

void CAMERA::Update(void)
{
	v_tar = player->srt.pos;

	v_pos = v_tar - D3DXVECTOR3(distance, 0.0f, 0.0f);

	D3DXVECTOR4 tmp;
	D3DXMATRIX mtxRot;

	D3DXMatrixIdentity(&mtxRot);
	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, player->srt.rot.y, D3DXToRadian(pitch), player->srt.rot.z);
	D3DXVec3Transform(&tmp, &v_pos, &mtxRot);

	v_pos = D3DXVECTOR3(tmp.x, tmp.y, tmp.z);
}
