#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include "main.h"
#include "core.h"
#include "d3d.h"
#include "gamemain.h"

#define GM_MAX_PARTICLES	256

typedef struct PARTICLE {
	D3DXVECTOR3				pos;
	D3DXVECTOR3				mov;
	D3DXCOLOR				col;
	BOOL					bAlive;
} PARTICLE;

typedef struct EMITTER {
	LPDIRECT3DTEXTURE9		pTex;
	LPDIRECT3DVERTEXBUFFER9	pVtx;
	D3DXVECTOR3				pos;
	D3DXVECTOR3				mov;
	D3DXCOLOR				col;
	FLOAT					size;

	PARTICLE				particles[GM_MAX_PARTICLES];
	DWORD					dwCount;
	BOOL					bAlive;

	EMITTER(LPCWSTR texturePath, D3DXVECTOR3 pos, D3DXVECTOR3 mov, D3DXCOLOR col, FLOAT size);
	~EMITTER();

	void					Update();
	HRESULT					SetVertex();
	HRESULT					Draw(CAMERA* camera);
} EMITTER;

#endif // !__PARTICLE_H__
