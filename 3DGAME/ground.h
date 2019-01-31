#ifndef __GROUND_H__
#define __GROUND_H__

#include "main.h"
#include "core.h"
#include "polygon.h"

//
// µØÃæÇéˆó
//
typedef struct GROUND {
	static DWORD				count;
	static LPDIRECT3DTEXTURE9	pTex;
	POLYGON*					polygon;

	GROUND(D3DXVECTOR3 vertices[]);
	~GROUND();
} GROUND;

#endif // !__GROUND_H__

