#ifndef __TERRIAN_H__
#define __TERRIAN_H__

#include "main.h"
#include "core.h"
#include "gamemain.h"
#include "ground.h"

#define GM_TERRAIN_SIZE			32

struct TERRAIN {
	GROUND**					ppGround;
	BYTE*						pHeightMap;
	DWORD						dwHeightMapSize;
	DWORD						dwMapSize;

	TERRAIN(DWORD depth);
	~TERRAIN();

	HRESULT						SetVertex();
	HRESULT						Draw(PLAYER* player);
	void						Update();

	void						GenerateHeightMap(BYTE* leftTop, BYTE* rightTop, BYTE* leftBottom, BYTE* rightBottom, INT depth);
	BYTE						GetAverage(BYTE* p1, BYTE* p2, BYTE* p3, BYTE* p4, INT depth);
	BYTE*						GetCenter(BYTE* leftTop, BYTE* rightBottom);
	BYTE*						GetPoint(INT x, INT y);

	FLOAT						GetGroundHeight(FLOAT x, FLOAT z);
};

#endif // !__TERRIAN_H__
