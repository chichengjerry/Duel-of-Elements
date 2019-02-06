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

	D3DXMATRIX mtxRot;

	v_tar = srt.pos; // +D3DXVECTOR3(vec.x, vec.y, vec.z);
	// v_tar.y += 20.0f;

	//
	//D3DXVECTOR4 vec = D3DXVECTOR4(0.0f, distance, 0.0f, 1.0f);
	//
	//D3DXMatrixIdentity(&mtxRot);
	//D3DXMatrixRotationYawPitchRoll(&mtxRot, srt.rot.y, D3DXToRadian(pitch), srt.rot.z);
	//D3DXVec4Transform(&vec, &vec, &mtxRot);
	//
	//v_pos = v_tar + D3DXVECTOR3(vec.x, vec.y, vec.z);

	D3DXVECTOR4 vec = D3DXVECTOR4(distance, 0.0f, 0.0f, 1.0f);
	//D3DXMatrixRotationZ(&mtxRot, D3DXToRadian(-10.0f));
	//D3DXVec4Transform(&vec, &vec, &mtxRot);
	D3DXMatrixRotationYawPitchRoll(&mtxRot, srt.rot.y - D3DX_PI / 2, srt.rot.z, srt.rot.x);
	D3DXVec4Transform(&vec, &vec, &mtxRot);

	v_pos = v_tar - D3DXVECTOR3(vec.x, vec.y, vec.z);

	v_tar.y += 25.0f;
	v_pos.y += 25.0f;
	// ビューマトリックスの作成
	D3DXMatrixIdentity(&m_view);
	D3DXMatrixLookAtLH(&m_view,
		&v_pos,		// カメラの視点
		&v_tar,		// カメラの注視点
		&v_up);		// カメラの上方向

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_view);
}
