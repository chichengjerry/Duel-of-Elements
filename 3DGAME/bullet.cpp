//-----------------------------------------------
// [bullet.cpp]
// 弾処理
// 作者 Chicheng Zhang
//-----------------------------------------------
#include "bullet.h"
#include "camera.h"
#include "element.h"
#include "explosion.h"
#include "game.h"
#include "player.h"
#include "terrain.h"

#define GM_BULLET_SIZE			20.0f
#define GM_BULLET_SPEED			600.0f
#define BULLET_WIDE				8.0f
#define BULLET_PARTICLES		32

//
// コンストラクター
//
BULLET::BULLET(PLAYER * player, D3DXVECTOR3* tar)
{
	src = player;
	bulletType = src->attackType;
	col = ElementColor[bulletType];
	bAlive = TRUE;
	bHit = FALSE;
	fSpeed = player->fMoveSpeed + GM_BULLET_SPEED;
	if (tar) {
		dir = *tar - player->srt.pos;
	}
	else{
		D3DXMATRIX mtx;
		D3DXVECTOR4 dir_v4 = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);
		D3DXMatrixRotationYawPitchRoll(&mtx, player->srt.rot.y, -player->srt.rot.x, player->srt.rot.z);
		D3DXVec4Transform(&dir_v4, &dir_v4, &mtx);
		dir = D3DXVECTOR3(dir_v4.x, dir_v4.y, dir_v4.z);
	}

	D3DXVec3Normalize(&dir, &dir);
	D3DXVECTOR3 mov = dir * BULLET_WIDE;
	pos = player->srt.pos;

	bulletParticleEmitter = new EMITTER(ELEMENT::pTex, BULLET_PARTICLES, pos + mov * BULLET_PARTICLES * 2 / BULLET_WIDE, mov * fSpeed, col, GM_BULLET_SIZE);
	for (int i = 1; i < 32; i++) {
		FLOAT scl = (FLOAT)(BULLET_PARTICLES - i) / BULLET_PARTICLES;
		bulletParticleEmitter->particles[i].scl = scl;
		bulletParticleEmitter->particles[i].pos = bulletParticleEmitter->particles[i - 1].pos - mov * scl;
	}
}

//
// デストラクター
//
BULLET::~BULLET()
{
	SAFE_DELETE(bulletParticleEmitter);
}

//
// 弾のリストに１つ入れる。
//
void BULLET::Add(BULLET * pBullets[], INT length, PLAYER* player, D3DXVECTOR3* tar)
{
	for (int i = 0; i < length; i++) {
		if (!pBullets[i]) {
			pBullets[i] = new BULLET(player, tar);
			break;
		}
	}
}

//
// プレイヤーに当たっているかをチェックする。
//
BOOL BULLET::HitPlayer(PLAYER * player)
{
	if(bAlive && player != src)
		return IntersectRaySphere(NULL, pos, dir, player->srt.pos, GM_BULLET_SIZE);

	return FALSE;
}

//
// 弾の情報を更新する。
//
void BULLET::Update(TERRAIN * terrain)
{
	pos = bulletParticleEmitter->particles[0].pos;

	if (terrain->IsOutofBound(pos.x, pos.z)) {
		bHit = TRUE;
	}
	FLOAT bulletHeight = terrain->GetGroundHeight(pos.x, pos.z);

	if ((pos.y - bulletHeight) <= 0.0f) {
		bHit = TRUE;

		for (int i = 0; i < BULLET_PARTICLES; i++) {
			D3DXVECTOR3 p = bulletParticleEmitter->particles[i].pos;
			bulletHeight = terrain->GetGroundHeight(p.x, p.z);
			if ((p.y - bulletHeight) >= 0.0f) {
				EXPLOSION::Add(src->pGame->explosion, p);
				break;
			}
		}
	}

	bulletParticleEmitter->bAlive = FALSE;
	PARTICLE* pp;

	// Check if all particles are available
	for (int i = 0; i < bulletParticleEmitter->nParticlesCount; i++) {
		pp = &bulletParticleEmitter->particles[i];
		if (!bHit) {
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

//
// 弾を描画する。
//
HRESULT BULLET::Draw(CAMERA* camera)
{
	if (bAlive) {
		return bulletParticleEmitter->Draw(camera);
	}

	else
		return S_OK;
}
