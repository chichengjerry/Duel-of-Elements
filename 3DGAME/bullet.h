//-----------------------------------------------
// [bullet.h]
// 弾処理
// 作者 Chicheng Zhang
//-----------------------------------------------
#ifndef __BULLET_H__
#define __BULLET_H__

#include "main.h"
#include "core.h"
#include "gamemain.h"
#include "particle.h"

struct BULLET {
	BOOL						bAlive;
	FLOAT						fSpeed;
	ELEMENTTYPE					bulletType;
	D3DXVECTOR3					pos;
	D3DXVECTOR3					dir
;
	D3DXCOLOR					col;
	EMITTER*					bulletParticleEmitter;
	PLAYER*						src;
	BOOL						bHit;

	BULLET(PLAYER* player, D3DXVECTOR3* tar);
	~BULLET();

	static void					Add(BULLET* pBullets[], INT length, PLAYER* player, D3DXVECTOR3* tar);
	static void					LoadTexture();
	static void					ReleaseTexture();
	BOOL						HitPlayer(PLAYER* player);
	void						Update(TERRAIN* terrain);
	HRESULT						Draw(CAMERA* camera);
};

#endif // !__BULLET_H__
