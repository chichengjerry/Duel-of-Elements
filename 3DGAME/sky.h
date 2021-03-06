//-----------------------------------------------
// [sky.h]
// スカイボックス処理
// 作者 Chicheng Zhang
//-----------------------------------------------
#ifndef __SKY_H__
#define __SKY_H__

#include "main.h"
#include "core.h"
#include "gamemain.h"
#include "light.h"
#include "particle.h"

typedef enum SKYENUM {
	SKY_FR,
	SKY_BK,
	SKY_LF,
	SKY_RT,
	SKY_UP,
	SKY_DN,

	SKY_NUM
} SKYENUM;

struct SKY {
	LPDIRECT3DTEXTURE9		pTex[SKY_NUM] = { NULL };	// テクスチャへのポインタ
	LPDIRECT3DTEXTURE9		pTexSun = NULL;				// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 pVtx = NULL;				// 頂点バッファへのポインタ
	D3DXMATRIX				mtx;						// ワールドマトリックス
	D3DXVECTOR3				pos;						// 現在の位置
	LIGHT*					light;
	EMITTER*				sun;

	SKY();
	~SKY();

	HRESULT				SetSky();
	void				Update();
	void				Draw(CAMERA* pCamera);
};

#endif
