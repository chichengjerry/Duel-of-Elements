#include "camera.h"

#define CM_PITCH			D3DXToRadian(-20.0f)
#define CM_DIST_FPS			1.0f
#define CM_DIST_TPS			50.0f

CAMERA::CAMERA(PLAYER* player)
{
	DWORD height = CL_HEIGHT;
	DWORD width = CL_WIDTH / 2;
	DWORD x = player->id * CL_WIDTH / 2;
	this->player = player;
	this->viewport = { x, 0, width, height, 0.0f, 1.0f };
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

	SRT srt = player->srt;
	v_tar = srt.pos;

	D3DXVECTOR3 plane_rot = player->plane->srt.rot;
	D3DXVECTOR4 up = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
	D3DXVECTOR4 vec;
	D3DXMATRIX mtxRot;

	if (player->bFPSCamera) {
		// モデルの回転に応じてカメラ変換
		vec = D3DXVECTOR4(CM_DIST_FPS, 0.0f, 0.0f, 1.0f);
		D3DXMatrixRotationYawPitchRoll(&mtxRot, plane_rot.y, plane_rot.x, plane_rot.z);
		D3DXVec4Transform(&up, &up, &mtxRot);
		D3DXVec4Normalize(&up, &up);
	}
	else {
		// ピッチを足す
		v_tar.y += 20.0f;
		vec = D3DXVECTOR4(CM_DIST_TPS, 0.0f, 0.0f, 1.0f);
		D3DXMatrixRotationZ(&mtxRot, CM_PITCH);
		D3DXVec4Transform(&vec, &vec, &mtxRot);
	}
	// カメラの座標変換
	D3DXMatrixRotationYawPitchRoll(&mtxRot, srt.rot.y - D3DX_PI / 2, srt.rot.z, srt.rot.x);
	D3DXVec4Transform(&vec, &vec, &mtxRot);

	v_pos = v_tar - D3DXVECTOR3(vec.x, vec.y, vec.z);
	v_up = D3DXVECTOR3(up.x, up.y, up.z);

	// ビューマトリックスの作成
	D3DXMatrixIdentity(&m_view);
	D3DXMatrixLookAtLH(&m_view,
		&v_pos,		// カメラの視点
		&v_tar,		// カメラの注視点
		&v_up);		// カメラの上方向

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_view);
}
