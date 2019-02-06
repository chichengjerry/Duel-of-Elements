#ifndef __GROUND_H__
#define __GROUND_H__

#include "main.h"
#include "core.h"
#include "polygon.h"

//
// 地面情報
//
struct GROUND {
	static DWORD				count;
	static LPDIRECT3DTEXTURE9	pTex;
	POLYGON*					polygon;

	GROUND(D3DXVECTOR3 vertices[], D3DXVECTOR3 normals[]);
	~GROUND();

	HRESULT						Draw();
	void						Update();
};

#endif // !__GROUND_H__

