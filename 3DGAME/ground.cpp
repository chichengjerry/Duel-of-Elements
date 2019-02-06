#include "ground.h"

#define TEX_GROUND	_T("data/TEXTURE/rock.png")

LPDIRECT3DTEXTURE9	GROUND::pTex = NULL;
DWORD				GROUND::count = 0;

GROUND::GROUND(D3DXVECTOR3 vertices[], D3DXVECTOR3 normals[])
{
	LPDIRECT3DDEVICE9 pDevice = D3D::GetDevice();

	if (!pTex) {
		if (FAILED(D3DXCreateTextureFromFile(
			pDevice,
			TEX_GROUND,
			&pTex))) {
			return;
		};
	}
	count++;

	polygon = new POLYGON(pTex, vertices, normals);
}

GROUND::~GROUND(){
	delete polygon;

	if (!--count) {
		SAFE_RELEASE(pTex);
	}
}

HRESULT GROUND::Draw()
{
	return polygon->Draw();
}

void GROUND::Update()
{
	polygon->Update();
}
