#include "bullet.h"
#include "camera.h"
#include "element.h"
#include "player.h"
#include "terrain.h"

#define GM_BULLET_SIZE			10.0f
#define GM_BULLET_SPEED			300.0f
#define BULLET_SPRITE			_T("data/TEXTURE/glow01.jpg")
#define BULLET_WIDE				4.0f
#define BULLET_PARTICLES		32

BULLET::BULLET(PLAYER * player, D3DXVECTOR3* tar)
{
	src = player;
	bulletType = src->attackType;
	col = ElementColor[bulletType];
	bAlive = TRUE;
	fSpeed = player->fMoveSpeed + GM_BULLET_SPEED;

	D3DXVECTOR3 mov;
	if (tar) {
		mov = *tar - player->srt.pos;
	}
	else{
		D3DXMATRIX mtx;
		D3DXVECTOR4 dir = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);
		D3DXMatrixRotationYawPitchRoll(&mtx, player->srt.rot.y, -player->srt.rot.x, player->srt.rot.z);
		D3DXVec4Transform(&dir, &dir, &mtx);
		mov = D3DXVECTOR3(dir.x, dir.y, dir.z);
	}

	D3DXVec3Normalize(&mov, &mov);
	mov *= BULLET_WIDE;
	pos = player->srt.pos;

	bulletParticleEmitter = new EMITTER(BULLET_SPRITE, BULLET_PARTICLES, pos + mov * BULLET_PARTICLES * 2 / BULLET_WIDE, mov * fSpeed, col, GM_BULLET_SIZE);
	for (int i = 1; i < 32; i++) {
		FLOAT scl = (FLOAT)(BULLET_PARTICLES - i) / BULLET_PARTICLES;
		bulletParticleEmitter->particles[i].scl = scl;
		bulletParticleEmitter->particles[i].pos = bulletParticleEmitter->particles[i - 1].pos - mov * scl;
	}
}

BULLET::~BULLET()
{
	SAFE_DELETE(bulletParticleEmitter);
}

void BULLET::Add(BULLET * pBullets[], INT length, PLAYER* player, D3DXVECTOR3* tar)
{
	for (int i = 0; i < length; i++) {
		if (!pBullets[i]) {
			pBullets[i] = new BULLET(player, tar);
			break;
		}
	}
}

void BULLET::Update(TERRAIN * terrain)
{
	pos = bulletParticleEmitter->particles[0].pos;
	hit = (terrain->OutOfBound(pos.x, pos.z) || (pos.y - terrain->GetGroundHeight(pos.x, pos.z)) <= 0.0f);

	bulletParticleEmitter->bAlive = FALSE;
	PARTICLE* pp;

	// Check if all particles are available
	for (int i = 0; i < bulletParticleEmitter->nParticlesCount; i++) {
		pp = &bulletParticleEmitter->particles[i];
		if (!hit) {
			if (!pp->bAlive) {
				pp->bAlive = TRUE;
				break;
			}
			else {
				bulletParticleEmitter->bAlive = TRUE;
			}
		}
		else {
			if (pp->bAlive) {
				bulletParticleEmitter->bAlive = TRUE;
				pp->bAlive = FALSE;
			}
		}
	}

	if (bulletParticleEmitter->bAlive) {
		bulletParticleEmitter->Update();
	}
	
	bAlive = bulletParticleEmitter->bAlive;
}

HRESULT BULLET::Draw(CAMERA* camera)
{
	if (bAlive) {
		return bulletParticleEmitter->Draw(camera);
	}

	else
		return S_OK;
}
