#ifndef __POLYGON_H__
#define __POLYGON_H__

#include "main.h"
#include "core.h"
#include "camera.h"
#include "d3d.h"

//
// ¥Ý¥ê¥´¥óÇéˆó
//
typedef struct POLYGON {
	LPDIRECT3DVERTEXBUFFER9	pVtx;
	LPDIRECT3DTEXTURE9		pTex;

	D3DXCOLOR				col;
	D3DXVECTOR3				vtx[NUM_VERTEX];
	D3DXMATRIX				mtx;
	SRT						srt;

	BOOL					isBillboard;

	POLYGON(LPDIRECT3DTEXTURE9 pTex, D3DXVECTOR3 vertices[]);
	~POLYGON();

	HRESULT					SetVertex(LPDIRECT3DDEVICE9 pDevice);
	HRESULT					Draw(CAMERA* pCamera);
	D3DXMATRIX*				GetMatrix();
	SRT*					GetSrt();
	void					Update();
} POLYGON;


#endif // !__POLYGON_H__
