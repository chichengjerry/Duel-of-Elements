/***********************************************
 * [model.h]
 * •‚•«•ÎÑI¿Ì
 * GP11A341 24 èàÒYÚG
 ***********************************************/

#ifndef __MODEL_H__
#define __MODEL_H__

#include "main.h"
#include "core.h"
#include "d3d.h"
#include "util.h"

//
// •‚•«•Î«ÈàÛ
//
typedef struct MODEL {
	DWORD					nMatNum;
	LPD3DXBUFFER			pMatBuf;
	LPD3DXMESH				pMesh;
	LPDIRECT3DTEXTURE9		pTex;

	D3DXMATRIX				mtx;
	SRT						srt;
	D3DXCOLOR				col;

	MODEL(LPCWSTR modelSrc, LPCWSTR texSrc);
	~MODEL();

	HRESULT					LoadModel(LPCWSTR modelSrc, LPCWSTR texSrc);
	HRESULT					Draw();
	D3DXMATRIX*				GetMatrix();
	SRT*					GetSrt();
	void					Update();
} MODEL;

#endif // !__MODEL_H__
