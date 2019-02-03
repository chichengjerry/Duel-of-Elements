#include "image.h"

IMAGE::IMAGE(LPDIRECT3DTEXTURE9 pTex, D3DXVECTOR3 vtx[])
{
	this->pTex = pTex;
	this->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	this->rot = 0.0f;
	this->scl = 1.0f;

}

IMAGE::~IMAGE()
{
	SAFE_RELEASE(pVtx);
}

HRESULT IMAGE::SetVertex(D3DXVECTOR3 vtx[])
{
	return E_NOTIMPL;
}

HRESULT IMAGE::Draw()
{
	return E_NOTIMPL;
}

void IMAGE::Update()
{
	if (bAnimating) {
		nFrameIndex = (nFrameIndex + 1) % nFrameTotal;
	}
}
