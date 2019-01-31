#include "ground.h"

#define TEX_GROUND	_T("data/TEXTURE/grass.jpg")

LPDIRECT3DTEXTURE9	GROUND::pTex = NULL;
DWORD				GROUND::count = 0;

GROUND::GROUND(D3DXVECTOR3 vertices[])
{
	LPDIRECT3DDEVICE9 pDevice = D3D::GetDevice();

	if (count == 0) {
		D3DXCreateTextureFromFile(
			pDevice,
			TEX_GROUND,
			&pTex);
	}

	polygon = new POLYGON(pTex, vertices);

	count++;
}

GROUND::~GROUND(){
	count--;

	delete polygon;

	if (!count) {
		SAFE_RELEASE(pTex);
	}
}
