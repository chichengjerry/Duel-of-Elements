#ifndef __MESHFIELD_H__
#define __MESHFIELD_H__

#include "main.h"
#include "core.h"
#include "d3d.h"

typedef struct COLORSTEP {
	D3DXCOLOR					color;
	FLOAT						t;
} COLORSTEP;

struct COLORMAP {
	DWORD						steps;
	COLORSTEP*					mCols;

	COLORMAP(DWORD steps, COLORSTEP*);
	D3DXCOLOR					GetColor(FLOAT t);
};

typedef struct MESHFIELD {
	LPDIRECT3DVERTEXBUFFER9		pVtxBuf;
	LPDIRECT3DINDEXBUFFER9		pIdxBuf;
	LPDIRECT3DTEXTURE9			pTexBuf;
	FLOAT						fMinHeight;
	FLOAT						fMaxHeight;
	INT							nBlkSizeX;
	INT							nBlkSizeZ;
	INT							nMapSizeX;
	INT							nMapSizeZ;
	DWORD						dwVtxCount;
	DWORD						dwIdxCount;
	DWORD						dwPolyCount;

	MESHFIELD(DWORD mapSizeX, DWORD mapSizeY, DWORD blockSizeX, DWORD blockSizeY, FLOAT minHeight, FLOAT maxHeight);
	~MESHFIELD();

	HRESULT						Draw();
	void						Update();
	D3DXCOLOR					GetDiffuse(BYTE height, COLORMAP* colorMap);
	FLOAT						GetPositionHeight(FLOAT fx, FLOAT fz);
	BOOL						OutOfBound(FLOAT fx, FLOAT fz);
	HRESULT						SetTexture(LPCWSTR textureSrc);
	HRESULT						SetVertex(BYTE* pHeightMap, COLORMAP* colorMap);

} MESHFIELD;

#endif // !__MESHFIELD_H__
