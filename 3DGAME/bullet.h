#ifndef __BULLET_H__
#define __BULLET_H__

#include "main.h"
#include "core.h"
#include "gamemain.h"
#include "particle.h"

struct BULLET {
	ELEMENTTYPE		bulletType;
	D3DXCOLOR		col;
	EMITTER*		bullet;
	PLAYER*			src;
	BOOL			bAlive;

	BULLET(PLAYER* player, D3DXVECTOR3 tar);
	~BULLET();

	void			Update();
	HRESULT			Draw(TERRAIN* terrian);
};

#endif // !__BULLET_H__
