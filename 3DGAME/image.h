#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "main.h"
#include "core.h"
#include "d3d.h"

typedef struct IMAGE {
	LPDIRECT3DVERTEXBUFFER9		pVtx;
	LPDIRECT3DTEXTURE9			pTex;
	DWORD						nFrameIndex;
	DWORD						nFrameTotal;
	BOOL						bAnimating;

	D3DXVECTOR3					vtx[NUM_VERTEX];
	D3DXCOLOR					col;
	FLOAT						rot;
	FLOAT						scl;

	IMAGE(LPDIRECT3DTEXTURE9 pTex, D3DXVECTOR3 vtx[]);
	~IMAGE();

	HRESULT					SetVertex(D3DXVECTOR3 vtx[]);

	HRESULT					Draw();
	void					Update();
} IMAGE;


#endif // !__IMAGE_H__
