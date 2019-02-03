#include "camera.h"

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
	this->pitch = -60.0f;
	this->distance = 50.0f;
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
		((FLOAT)CL_WIDTH / (FLOAT)CL_HEIGHT) / 2,			// アスペクト比
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
	LPDIRECT3DDEVICE9 pDevice = D3D::GetDevice();

	if (player->id == 0) {
		if (DINPUT::GetKeyboardPress(DIK_Z)) {
			if (pitch > -90.0f)
				pitch -= 1.0f;
		}
		else if (DINPUT::GetKeyboardPress(DIK_X)) {
			if (pitch < -30.0f)
				pitch += 1.0f;
		}
		else if (DINPUT::GetKeyboardPress(DIK_C)) {
			this->pitch = -60.0f;
			this->distance = 50.0f;
		}
	}

	if (player->id == 1) {
		if (DINPUT::GetKeyboardPress(DIK_N)) {
			if (pitch > -90.0f)
				pitch -= 1.0f;
		}
		else if (DINPUT::GetKeyboardPress(DIK_M)) {
			if (pitch < -30.0f)
				pitch += 1.0f;
		}
		else if (DINPUT::GetKeyboardPress(DIK_COMMA)) {
			this->pitch = -60.0f;
			this->distance = 50.0f;
		}
	}

	SRT* srt = &player->srt;

	D3DXVECTOR4 vec = D3DXVECTOR4(0.0f, 0.0f, 10.0f, 1.0f);
	D3DXMATRIX mtxRot;

	// D3DXMatrixIdentity(&mtxRot);
	// D3DXMatrixRotationYawPitchRoll(&mtxRot, srt->rot.y, srt->rot.x, srt->rot.z);
	// D3DXVec4Transform(&vec, &vec, &mtxRot);

	v_tar = srt->pos; // +D3DXVECTOR3(vec.x, vec.y, vec.z);
	vec = D3DXVECTOR4(0.0f, distance, 0.0f, 1.0f);

	D3DXMatrixIdentity(&mtxRot);
	D3DXMatrixRotationYawPitchRoll(&mtxRot, player->srt.rot.y, D3DXToRadian(pitch), player->srt.rot.z);
	D3DXVec4Transform(&vec, &vec, &mtxRot);

	v_pos = v_tar + D3DXVECTOR3(vec.x, vec.y, vec.z);

	D3DXMatrixIdentity(&m_view);

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH(&m_view,
		&v_pos,		// カメラの視点
		&v_tar,		// カメラの注視点
		&v_up);		// カメラの上方向

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_view);
}
