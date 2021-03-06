//-----------------------------------------------
// [explosion.h]
// 爆発効果処理
// 作者 Chicheng Zhang
//-----------------------------------------------
#ifndef __EXPLOSION_H__
#define __EXPLOSION_H__

#include "main.h"
#include "core.h"
#include "d3d.h"
#include "gamemain.h"
#include "particle.h"

#define EXPLOSIVE_FRAME_X			8
#define EXPLOSIVE_FRAME_Y			8
#define EXPLOSIVE_FRAME_TOTAL		(EXPLOSIVE_FRAME_X * EXPLOSIVE_FRAME_Y)
#define EXPLOSIVE_SIZE				50.0f
#define EXPLOSIVE_IMAGE				_T("data/TEXTURE/boom_%d.png")
#define EXPLOSIVE_IMAGE_MAX			3

typedef struct EXPLOSION {
	static LPDIRECT3DTEXTURE9	pTex[3];
	EMITTER*					particle;
	FLOAT						fTime;
	INT							idx;

	EXPLOSION(D3DXVECTOR3 pos);
	~EXPLOSION();

	static void					Add(EXPLOSION* pExplosion[], D3DXVECTOR3 pos);
	static void					LoadTextures();
	static void					ReleaseTextures();

	void						Update();
	HRESULT						Draw(CAMERA* pCamera);
} EXPLOSION;
#endif // !__EXLOSION_H__
