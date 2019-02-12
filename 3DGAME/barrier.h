#ifndef __BARRIER_H__
#define __BARRIER_H__

#include "main.h"
#include "core.h"
#include "d3d.h"
#include "gamemain.h"
#include "camera.h"
#include "terrain.h"

typedef struct BARRIER {
	LPDIRECT3DVERTEXBUFFER9 pVtx;
	LPDIRECT3DTEXTURE9		pTex;

	BARRIER(TERRAIN* terrain);
	~BARRIER();

	void					Update();
	HRESULT					Draw(CAMERA* camera);

	HRESULT					SetVertex(TERRAIN* terrain);
} BARRIER;

#endif