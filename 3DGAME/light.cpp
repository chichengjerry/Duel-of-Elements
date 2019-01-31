#include "light.h"
#include "d3d.h"

LIGHT::LIGHT()
{
	LPDIRECT3DDEVICE9 pDevice = D3D::GetDevice();
	D3DXVECTOR3 vecDir;

	// D3DLIGHT9構造体を0でクリアする
	ZeroMemory(&light[0], sizeof(D3DLIGHT9));

	// ライトのタイプの設定
	light[0].Type = D3DLIGHT_DIRECTIONAL;

	// 拡散光
	light[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// 環境光
	light[0].Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);

	// ライトの方向の設定
	vecDir = D3DXVECTOR3(0.20f, -0.60f, 0.80f);
	D3DXVec3Normalize((D3DXVECTOR3*)&light[0].Direction, &vecDir);

	// ライトをレンダリングパイプラインに設定
	pDevice->SetLight(0, &light[0]);

	// ライトの設定
	pDevice->LightEnable(0, TRUE);


	// D3DLIGHT9構造体を0でクリアする
	ZeroMemory(&light[1], sizeof(D3DLIGHT9));

	// ライトのタイプの設定
	light[1].Type = D3DLIGHT_DIRECTIONAL;

	// 拡散光
	light[1].Diffuse = D3DXCOLOR(0.75f, 0.75f, 0.75f, 1.0f);

	// 環境光
	light[1].Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);

	// ライトの方向の設定
	vecDir = D3DXVECTOR3(-0.20f, 1.00f, -0.50f);
	D3DXVec3Normalize((D3DXVECTOR3*)&light[1].Direction, &vecDir);

	// ライトをレンダリングパイプラインに設定
	pDevice->SetLight(1, &light[1]);

	// ライトの設定
	pDevice->LightEnable(1, TRUE);

	// D3DLIGHT9構造体を0でクリアする
	ZeroMemory(&light[2], sizeof(D3DLIGHT9));

	// ライトのタイプの設定
	light[2].Type = D3DLIGHT_DIRECTIONAL;

	// 拡散光
	light[2].Diffuse = D3DXCOLOR(0.25f, 0.25f, 0.25f, 1.0f);

	// 環境光
	light[2].Ambient = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);

	// ライトの方向の設定
	vecDir = D3DXVECTOR3(0.80f, 0.10f, 0.50f);
	D3DXVec3Normalize((D3DXVECTOR3*)&light[2].Direction, &vecDir);

	// ライトをレンダリングパイプラインに設定
	pDevice->SetLight(2, &light[2]);

	// ライトの設定
	pDevice->LightEnable(2, TRUE);


	// ライティングモード
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

}

LIGHT::~LIGHT()
{
}
