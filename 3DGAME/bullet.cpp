#include "bullet.h"
#include "element.h"
#include "player.h"
#include "terrain.h"

#define BULLET_PARTICLES	32

BULLET::BULLET(PLAYER * player, D3DXVECTOR3 tar)
{
	src = player;
	bulletType = src->elementType;
	col = ElementColor[bulletType - 1];
	bAlive = TRUE;

	D3DXVECTOR3 dir = tar - player->srt.pos;
	D3DXVec3Normalize(&dir, &dir);

	bullet = new EMITTER(_T(""), BULLET_PARTICLES, player->srt.pos, dir, col, 5.0f);
	for (int i = 1; i < BULLET_PARTICLES; i++) {
		bullet[i].bAlive = FALSE;
	}
}

void BULLET::Update()
{
	for (int i = 0; i < BULLET_PARTICLES; i++) {
		if (bullet[i].bAlive) {
			bullet[i].pos += bullet[i].mov;
		}
		else {
			bullet[i].bAlive = TRUE;
			bullet[i].pos = bullet[i - 1].pos;
			break;
		}
	}
}
