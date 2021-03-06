//-----------------------------------------------
// [terrain.h]
// 地形処理
// 作者 Chicheng Zhang
//-----------------------------------------------
#ifndef __TERRIAN_H__
#define __TERRIAN_H__

#include "main.h"
#include "core.h"
#include "gamemain.h"
#include "meshfield.h"

struct TERRAIN {
	MESHFIELD*					pField;
	BYTE*						pHeightMap;
	INT							nBlockSize;
	INT							nHeightMapSize;
	INT							nMapSize;

	TERRAIN(DWORD depth);
	~TERRAIN();

	HRESULT						Draw(CAMERA* camera);
	void						Update();

	BOOL						IsOutofBound(FLOAT x, FLOAT z);
	FLOAT						GetGroundHeight(FLOAT x, FLOAT z);
};

#endif // !__TERRIAN_H__
