//=============================================================================
//
// 地面処理 [field.h]
// Author : 
//
//=============================================================================
#ifndef _SKY_H_
#define _SKY_H_

#include "main.h"
#include "core.h"
#include "d3d.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

typedef enum SKYENUM {
	SKY_FR,
	SKY_BK,
	SKY_LF,
	SKY_RT,
	SKY_UP,
	SKY_DN,

	SKY_NUM
} SKYENUM;

typedef struct SKY {
	LPDIRECT3DTEXTURE9		pTex[SKY_NUM] = { NULL };	// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 pVtx = NULL;	// 頂点バッファへのポインタ
	D3DXMATRIX				mtx;				// ワールドマトリックス
	D3DXVECTOR3				pos;					// 現在の位置

	SKY();
	~SKY();

	HRESULT				MakeSky();
	void				Update();
	void				Draw(CAMERA* pCamera);
} SKY;

#endif
