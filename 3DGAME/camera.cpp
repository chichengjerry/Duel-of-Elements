//-----------------------------------------------
// [camera.cpp]
// カメラ処理
// 作者 Chicheng Zhang
//-----------------------------------------------
#include "camera.h"

#define CM_PITCH			D3DXToRadian(-20.0f)
#define CM_DIST_FPS			1.0f
#define CM_DIST_TPS			50.0f

//
// コンストラクター
//
CAMERA::CAMERA(PLAYER* player = NULL, RECT* rect = NULL)
{
	DWORD height = CL_HEIGHT;
	DWORD width = CL_WIDTH;
	DWORD x = 0, y = 0;
	if (player && rect) {
		x = rect->left;
		y = rect->top;
		width = rect->right - rect->left;
		height = rect->bottom - rect->top;
		this->player = player;
	}
	this->viewport = { x, y, width, height, 0.0f, 1.0f };
}

//
// デストラクター
//
CAMERA::~CAMERA()
{
}

//
// カメラのビューマトリックスを取得。
//
D3DXMATRIX * CAMERA::GetView(void)
{
	return &m_view;
}

//
// カメラの情報をデバイスに更新する。
//
void CAMERA::SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = D3D::GetDevice();

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH(&m_view,	&v_pos, &v_tar, &v_up);

	// プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&m_proj,
		D3DXToRadian((FLOAT)GR_VIEWANGLE),
		(FLOAT)viewport.Width / (FLOAT)viewport.Height,
		GR_NEAR_Z,
		GR_FAR_Z);

	pDevice->SetViewport(&viewport);

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_view);

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_proj);
}

//
// カメラの情報を更新する。
//
void CAMERA::Update(void)
{
	LPDIRECT3DDEVICE9 pDevice = D3D::GetDevice();

	SRT srt = player->srt;
	v_tar = srt.pos;
	// ピッチを足す
	v_tar.y += 20.0f;

	D3DXVECTOR3 plane_rot = player->plane->srt.rot;
	D3DXVECTOR3 up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXVECTOR3 vec;
	D3DXMATRIX mtxRot;

	if (player->bFPSCamera) {
		// モデルの回転に応じてカメラ変換
		vec = D3DXVECTOR3(CM_DIST_FPS, 0.0f, 0.0f);
		D3DXMatrixRotationYawPitchRoll(&mtxRot, plane_rot.y, plane_rot.x, plane_rot.z);
		D3DXVec3TransformCoord(&up, &up, &mtxRot);
	}
	else {
		vec = D3DXVECTOR3(CM_DIST_TPS, 0.0f, 0.0f);
		D3DXMatrixRotationZ(&mtxRot, CM_PITCH);
		D3DXVec3TransformCoord(&vec, &vec, &mtxRot);
	}
	// カメラの座標変換
	D3DXMatrixRotationYawPitchRoll(&mtxRot, srt.rot.y - D3DX_PI / 2, srt.rot.z, srt.rot.x);
	D3DXVec3TransformCoord(&vec, &vec, &mtxRot);

	v_pos = v_tar - D3DXVECTOR3(vec.x, vec.y, vec.z);
	v_up = D3DXVECTOR3(up.x, up.y, up.z);
}
