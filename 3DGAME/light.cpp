//-----------------------------------------------
// [light.cpp]
// 光線の設定
// 作者 Chicheng Zhang
//-----------------------------------------------
#include "light.h"
#include "d3d.h"

#define ROTATE_SPEED		D3DXToRadian(2.0f)
#define SUN_COLOR_DAY		D3DXCOLOR(D3DCOLOR_ARGB(255, 255, 255, 255))
#define SUN_COLOR_NIGHT		D3DXCOLOR(D3DCOLOR_ARGB(255,  86,  90, 114))
#define SUN_COLOR_DAWN		D3DXCOLOR(D3DCOLOR_ARGB(255, 240, 187, 117))
#define SKY_COLOR_DAY		SUN_COLOR_DAY
#define SKY_COLOR_NIGHT		D3DXCOLOR(D3DCOLOR_ARGB(255,  43,  45,  57))
#define SKY_COLOR_DAWN		D3DXCOLOR(D3DCOLOR_ARGB(255, 235, 155,  71))
#define FOG_COLOR			D3DXCOLOR(D3DCOLOR_ARGB(255, 122, 145, 188))
#define COLOR_AMBIENT		D3DXCOLOR(0.25f, 0.25f, 0.25f, 1.0f)
#define SUN_DIRECTION		D3DXVECTOR3(0.42f, -1.0f, 0.0f)

//
// ライティングの設定（太陽光と月光）
//
LIGHT::LIGHT()
{
	LPDIRECT3DDEVICE9 pDevice = D3D::GetDevice();
	D3DXVECTOR3 vecDir;

	//太陽光を初期化
	ZeroMemory(&lightSun, sizeof(D3DLIGHT9));
	lightSun.Type = D3DLIGHT_DIRECTIONAL;
	lightSun.Diffuse = SUN_COLOR_DAY;
	lightSun.Ambient = COLOR_AMBIENT;
	vecDir = SUN_DIRECTION;
	D3DXVec3Normalize((D3DXVECTOR3*)&lightSun.Direction, &vecDir);
	pDevice->SetLight(0, &lightSun);
	pDevice->LightEnable(0, TRUE);

	//月光を初期化
	ZeroMemory(&lightMoon, sizeof(D3DLIGHT9));
	lightMoon.Type = D3DLIGHT_DIRECTIONAL;
	lightMoon.Diffuse = SUN_COLOR_NIGHT;
	lightMoon.Ambient = COLOR_AMBIENT;
	vecDir = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXVec3Normalize((D3DXVECTOR3*)&lightMoon.Direction, &vecDir);
	pDevice->SetLight(1, &lightMoon);
	pDevice->LightEnable(1, TRUE);

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//
// ライティングの資源をリリース（現在の時点は資源がない）。
//
LIGHT::~LIGHT()
{
}

//
// 光線の方向を更新する。
//
void LIGHT::Update()
{
	LPDIRECT3DDEVICE9 pDevice = D3D::GetDevice();

	D3DXVECTOR3 src;
	D3DXMATRIX rot;

	// 時間で太陽光の角度を変える
	D3DXMatrixRotationYawPitchRoll(&rot,
		0.5f * ROTATE_SPEED * D3D::fAnimationRate,
		ROTATE_SPEED * D3D::fAnimationRate,
		ROTATE_SPEED * D3D::fAnimationRate);
	D3DXVec3TransformCoord(&src, (D3DXVECTOR3*)&lightSun.Direction, &rot);

	D3DXVec3Normalize(&src, &src);

	lightSun.Direction = *(D3DVECTOR*)&src;
	//月光は太陽光の反対方向にする。
	lightMoon.Direction = *(D3DVECTOR*)&src;
	lightMoon.Direction.y *= -1.0f;

	isDayLight = src.y < 0.0f;
	lightSun.Diffuse = GetLightColor();
	lightMoon.Diffuse = GetLightColor();

	if (isDayLight) {
		pDevice->LightEnable(0, TRUE);
		pDevice->LightEnable(1, FALSE);
	}
	else {
		pDevice->LightEnable(0, FALSE);
		pDevice->LightEnable(1, TRUE);
	}
	pDevice->SetLight(0, &lightSun);
	pDevice->SetLight(1, &lightMoon);
}

//
// 空の色を計算する（テクスチャに色を付ける）。
//
D3DXCOLOR LIGHT::GetSkyColor()
{
	D3DXCOLOR col;

	FLOAT t = min(fabsf(lightSun.Direction.y), 0.5f) * 2;

	if (isDayLight) {
		D3DXColorLerp(&col, &SKY_COLOR_DAWN, &SKY_COLOR_DAY, -t * (t - 2));
	}
	else {
		D3DXColorLerp(&col, &SKY_COLOR_DAWN, &SKY_COLOR_NIGHT, -t * (t - 2));
	}
	return col;
}

//
// フォグの色を計算する。
//
D3DXCOLOR LIGHT::GetFogColor()
{
	D3DXCOLOR col;

	FLOAT t = min(fabsf(lightSun.Direction.y), 0.5f) * 2;

	if (isDayLight) {
		D3DXColorLerp(&col, &SKY_COLOR_DAWN, &FOG_COLOR, -t * (t - 2));
	}
	else {
		D3DXColorLerp(&col, &SKY_COLOR_DAWN, &SKY_COLOR_NIGHT, -t * (t - 2));
	}
	return col;
}
//
// 光の色を計算する。
//
D3DXCOLOR LIGHT::GetLightColor()
{
	D3DXCOLOR col;

	FLOAT t = min(fabsf(lightSun.Direction.y), 0.5f) * 2;

	if (isDayLight) {
		D3DXColorLerp(&col, &SUN_COLOR_DAWN, &SUN_COLOR_DAY, -t * (t - 2));
	}
	else {
		D3DXColorLerp(&col, &SUN_COLOR_DAWN, &SUN_COLOR_NIGHT, -t * (t - 2));
	}
	return col;
}
