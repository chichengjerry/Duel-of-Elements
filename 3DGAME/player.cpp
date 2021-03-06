//-----------------------------------------------
// [player.cpp]
// プレイヤー処理
// 作者 Chicheng Zhang
//-----------------------------------------------
#include "player.h"
#include "bullet.h"
#include "camera.h"
#include "element.h"
#include "explosion.h"
#include "game.h"
#include "input.h"
#include "terrain.h"
#include "sound.h"
#include "ui_game.h"

#define GLOW_NUM			2

typedef enum ACTION {
	ACTION_ATK,
	ACTION_SWAP,
	ACTION_SWITCH,
	ACTION_TURNLEFT,
	ACTION_TURNRIGHT,
	ACTION_PULLUP,
	ACTION_PULLDOWN,
	ACTION_SPEEDUP,
	ACTION_SPEEDDOWN,

	ACTION_COUNT
} ACTION;

//
// キーボード入力のマッピング
//
BYTE KEYBOARDMAP[GM_MAX_PLAYER][ACTION_COUNT] = {
	{ DIK_E, DIK_C, DIK_Q, DIK_A, DIK_D, DIK_S, DIK_W, DIK_Z, DIK_X },
	{ DIK_I, DIK_COMMA, DIK_Y, DIK_H, DIK_K, DIK_J, DIK_U, DIK_N, DIK_M }
};

//
// ゲームパッド入力のマッピング
//
DWORD GAMEPADMAP[ACTION_COUNT] = {
	BUTTON_B | BUTTON_X, BUTTON_A, BUTTON_LB | BUTTON_RB, BUTTON_LEFT, BUTTON_RIGHT, BUTTON_DOWN, BUTTON_UP, BUTTON_LT, BUTTON_RT
};

//
// プレイヤーが使っているエレメントの表現用
//
D3DXVECTOR3 pos_v[GLOW_NUM] = {
	D3DXVECTOR3(10.0f, 0.0f, 0.0f),
	D3DXVECTOR3(-10.0f, 0.0f, 0.0f),
};

//
// コンストラクター
//
PLAYER::PLAYER(MAINGAME* game, int playerId, BOOL isControlable)
{
	pGame = game;
	id = playerId;
	bControlable = isControlable;
	bCrash = FALSE;
	bFPSCamera = FALSE;
	nHealth = 100;
	fSize = PLAYER_SIZE;
	nScore = 0;
	fMoveSpeed = (MOVE_MAX_SPEED + MOVE_MIN_SPEED) / 2;
	fRollSpeed = D3DXToRadian(30.0f);
	fYawSpeed = 0.0f;
	attackType = ELEMENTTYPE_NONE;
	defenseType = ELEMENTTYPE_NONE;
	TERRAIN* terrain = pGame->terrain;

	FLOAT spawnX = (GET_RANDOM(terrain->nMapSize / 4, terrain->nMapSize * 3 / 4) - terrain->nMapSize / 2.0f) * terrain->nBlockSize;
	FLOAT spawnZ = (GET_RANDOM(terrain->nMapSize / 4, terrain->nMapSize * 3 / 4) - terrain->nMapSize / 2.0f) * terrain->nBlockSize;
	FLOAT spawnY = terrain->GetGroundHeight(spawnX, spawnZ);
	spawnY = GET_RANDOM(spawnY / 2 + GM_MAX_HEIGHT / 2, GM_MAX_HEIGHT);
	srt = SRT(D3DXVECTOR3(spawnX, spawnY, spawnZ));
	
	srt.rot.y = D3DX_PI /2 - atan2f(-srt.pos.z, -srt.pos.x);

	RECT rect = { (LONG)(CL_WIDTH / 2 * id), 0, (LONG)(CL_WIDTH / 2 * (id + 1)), CL_HEIGHT };
	camera = new CAMERA(this, &rect);

	WCHAR model[1024];
	wsprintf(model, _T("data/MODEL/plane%d.x"), id);

	plane = new MODEL((LPCWSTR)model, NULL);
	propeller = new MODEL(_T("data/MODEL/propeller.x"), NULL);

	glow = new EMITTER(ELEMENT::pTex, GLOW_NUM, srt.pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), ElementColor[ELEMENTTYPE_NONE], 5.0f);

	for (int i = 0; i < GLOW_NUM; i++) {
		glow->particles[i].pos = srt.pos + pos_v[i];
	}
}

//
// デストラクター
//
PLAYER::~PLAYER()
{
	SAFE_DELETE(plane);
	SAFE_DELETE(propeller);
	SAFE_DELETE(camera);
	SAFE_DELETE(glow);
}

//
// プレイヤーを描画する。
//
HRESULT PLAYER::Draw(CAMERA* camera)
{
	if (!bCrash) {
		plane->Draw(NULL);
		propeller->Draw(NULL);

		glow->Draw(camera);
	}
	return S_OK;
}

//
// プレイヤーを更新する。
//
void PLAYER::Update(TERRAIN* terrain)
{
	UpdateMovement(terrain);
	UpdateScore(NULL);
	UpdateHealth(NULL);
	// プレイヤー向き調整
	plane->srt = srt;
	plane->srt.rot.y += D3DX_PI;
	plane->Update();

	propeller->srt.pos = srt.pos;
	propeller->srt.pos.y += 0.5f;
	propeller->srt.rot.x = srt.rot.x;

	propeller->srt.rot.y = srt.rot.y + D3DX_PI;
	propeller->srt.rot.z += fRollSpeed;
	propeller->Update();

	// カメラとモデルの調整
	camera->Update();

	// GLOW調整
	for (int i = 0; i < GLOW_NUM; i++) {
		PARTICLE* pp = &glow->particles[i];
		ELEMENTTYPE type = i ? defenseType : attackType;
		if (type != ELEMENTTYPE_NONE) {
			pp->bAlive = TRUE;
			D3DXVECTOR4 delta = D3DXVECTOR4(pos_v[i].x, pos_v[i].y, pos_v[i].z, 1.0f);
			D3DXMATRIX rot;
			D3DXMatrixRotationY(&rot, propeller->srt.rot.z / 4);
			D3DXVec4Transform(&delta, &delta, &rot);
			pp->pos = srt.pos + D3DXVECTOR3(delta.x, delta.y, delta.z);
			glow->SetDiffuse(i, ElementColor[type]);
		}
		else {
			pp->bAlive = FALSE;
		}
	}

	glow->Update();
}

//
// プレイヤーのエレメントの種類を交換する。
//
void PLAYER::SwapElements()
{
	ELEMENTTYPE temp;
	temp = attackType;
	attackType = defenseType;
	defenseType = temp;
}

//
// 弾を発射する。
//
void PLAYER::Fire(void)
{
	if (attackType != ELEMENTTYPE_NONE) {
		BULLET::Add(pGame->bullets, GM_MAX_BULLETS, this, NULL);
		DSOUND::Play(SOUND_SHOT);
	}
}

//
// ダメージを受ける。
//
void PLAYER::Damaged(BULLET * bullet)
{
	INT health = nHealth;
	INT damage = DAMAGE_TABLE[defenseType][bullet->bulletType];
	if (damage > 0) {
		health -= damage;
		health = max(0, health);

		UpdateHealth(&health);

		if (health == 0)
			Destroy();
		else 
			DSOUND::Play(SOUND_HIT);
	}
}

//
// 破壊された。
//
void PLAYER::Destroy()
{
	bCrash = TRUE;
	pGame->GameOver(id, GAME_RESULT_LOSE);
	EXPLOSION::Add(pGame->explosion, srt.pos);
}

//
// 移動状態を更新する。
//
void PLAYER::UpdateMovement(TERRAIN* terrain)
{
	BOOL	rolling = FALSE;
	BOOL	pitching = FALSE;
	FLOAT	rate = D3D::fAnimationRate;

	if (pGame->bGameOver) {
		return;
	}
	if (bControlable) {
		int padID = 0;

		if (DINPUT::padCount == 2) {
			padID = id;
		}
		else {
			padID = (id == 0);
		}

		if (DINPUT::KeyTriggered(KEYBOARDMAP[id][ACTION_ATK]) ||
			DINPUT::ButtonTriggered(padID, GAMEPADMAP[ACTION_ATK])) {
			Fire();
		}
		if (DINPUT::KeyTriggered(KEYBOARDMAP[id][ACTION_SWAP]) ||
			DINPUT::ButtonTriggered(padID, GAMEPADMAP[ACTION_SWAP])) {
			SwapElements();
		}
		if (DINPUT::KeyTriggered(KEYBOARDMAP[id][ACTION_SWITCH]) ||
			DINPUT::ButtonTriggered(padID, GAMEPADMAP[ACTION_SWITCH])) {
			// Switch camera
			bFPSCamera = !bFPSCamera;
		}
		// スピード調整
		if (DINPUT::KeyPressed(KEYBOARDMAP[id][ACTION_SPEEDUP]) && DINPUT::KeyPressed(KEYBOARDMAP[id][ACTION_SPEEDDOWN]) ||
			DINPUT::ButtonPressed(padID, GAMEPADMAP[ACTION_SPEEDUP]) && DINPUT::ButtonPressed(padID, GAMEPADMAP[ACTION_SPEEDDOWN])) {}
		else
			if (DINPUT::KeyPressed(KEYBOARDMAP[id][ACTION_SPEEDUP]) ||
				DINPUT::ButtonPressed(padID, GAMEPADMAP[ACTION_SPEEDUP])) {
			fMoveSpeed += MOVE_STEP * rate;
			if (fMoveSpeed >= MOVE_MAX_SPEED)
				fMoveSpeed = MOVE_MAX_SPEED;
		}
		else
			if (DINPUT::KeyPressed(KEYBOARDMAP[id][ACTION_SPEEDDOWN]) ||
				DINPUT::ButtonPressed(padID, GAMEPADMAP[ACTION_SPEEDDOWN])) {
			fMoveSpeed -= MOVE_STEP * rate;
			if (fMoveSpeed <= MOVE_MIN_SPEED)
				fMoveSpeed = MOVE_MIN_SPEED;
		}

		// 垂直方向調整
		if (DINPUT::KeyPressed(KEYBOARDMAP[id][ACTION_PULLUP]) && DINPUT::KeyPressed(KEYBOARDMAP[id][ACTION_PULLDOWN]) ||
			DINPUT::ButtonPressed(padID, GAMEPADMAP[ACTION_PULLUP]) && DINPUT::ButtonPressed(padID, GAMEPADMAP[ACTION_PULLDOWN])) {
		}
		else
			if (DINPUT::KeyPressed(KEYBOARDMAP[id][ACTION_PULLDOWN]) ||
				DINPUT::ButtonPressed(padID, GAMEPADMAP[ACTION_PULLDOWN])) {
			pitching = TRUE;

			srt.rot.x -= PITCH_STEP * rate;
			if (srt.rot.x <= -PITCH_MAX_ANGLE)
				srt.rot.x = -PITCH_MAX_ANGLE;
		}
		else
			if (DINPUT::KeyPressed(KEYBOARDMAP[id][ACTION_PULLUP]) ||
				DINPUT::ButtonPressed(padID, GAMEPADMAP[ACTION_PULLUP])) {
			pitching = TRUE;

			srt.rot.x += PITCH_STEP * rate;
			if (srt.rot.x >= PITCH_MAX_ANGLE)
				srt.rot.x = PITCH_MAX_ANGLE;
		}

		// 水平方向調整
		if (DINPUT::KeyPressed(KEYBOARDMAP[id][ACTION_TURNLEFT]) && DINPUT::KeyPressed(KEYBOARDMAP[id][ACTION_TURNRIGHT]) ||
			DINPUT::ButtonPressed(padID, GAMEPADMAP[ACTION_TURNLEFT]) && DINPUT::ButtonPressed(padID, GAMEPADMAP[ACTION_TURNRIGHT])) {
		}
		else
			if (DINPUT::KeyPressed(KEYBOARDMAP[id][ACTION_TURNLEFT]) ||
				DINPUT::ButtonPressed(padID, GAMEPADMAP[ACTION_TURNLEFT])) {
			rolling = TRUE;

			srt.rot.z -= ROLL_STEP * rate;
			if (srt.rot.z <= -ROLL_MAX_ANGLE)
				srt.rot.z = -ROLL_MAX_ANGLE;

			fYawSpeed -= YAW_STEP * rate;
			if (fYawSpeed <= -YAW_MAX_ANGLE)
				fYawSpeed = -YAW_MAX_ANGLE;
		}
		else
			if (DINPUT::KeyPressed(KEYBOARDMAP[id][ACTION_TURNRIGHT]) ||
				DINPUT::ButtonPressed(padID, GAMEPADMAP[ACTION_TURNRIGHT])) {
			rolling = TRUE;
			srt.rot.z += ROLL_STEP * rate;
			if (srt.rot.z >= ROLL_MAX_ANGLE)
				srt.rot.z = ROLL_MAX_ANGLE;
			
			fYawSpeed += YAW_STEP * rate;
			if (fYawSpeed >= YAW_MAX_ANGLE)
				fYawSpeed = YAW_MAX_ANGLE;
		}
	}

	if (!pitching) {
	//	if (fabsf(srt.rot.x) < 0.01f) {
	//		srt.rot.x = 0.0f;
	//	}
	//	else if (srt.rot.x > 0.0f)
	//		srt.rot.x -= PITCH_STEP * rate;
	//	else if (srt.rot.x < 0.0f)
	//		srt.rot.x += PITCH_STEP * rate;
	}
	if (!rolling) {
		fYawSpeed = 0.0f;
		if (fabsf(srt.rot.z) < 0.01f) {
			srt.rot.z = 0.0f;
		}
		else if (srt.rot.z > 0.0f)
			srt.rot.z -= ROLL_STEP * rate;
		else if (srt.rot.z < 0.0f)
			srt.rot.z += ROLL_STEP * rate;
	}
	else {
		srt.rot.y += fYawSpeed * rate;
	}
	//
	// fRollSpeed = ROLL_IDLE_SPEED + 10.0f * (fMoveSpeed - MOVE_MIN_SPEED) * YAW_STEP;
	//
	D3DXVECTOR3 dir = D3DXVECTOR3(0.0f, 0.0f, fMoveSpeed * rate);
	D3DXVECTOR4 temp;
	D3DXMATRIX mtx;

	D3DXMatrixRotationX(&mtx, -srt.rot.x);
	D3DXVec3Transform(&temp, &dir, &mtx);
	D3DXMatrixRotationY(&mtx, srt.rot.y);
	D3DXVec4Transform(&temp, &temp, &mtx);

	srt.pos += D3DXVECTOR3(temp.x, temp.y, temp.z);

	SetPlayerInBound(terrain);
	SetPlayerHeight(terrain);
}

//
// 高さをチェックする。
//
void PLAYER::SetPlayerHeight(TERRAIN* terrain)
{
	D3DXVECTOR3 pos = srt.pos;

	FLOAT height = terrain->GetGroundHeight(pos.x, pos.z);

	gx = (INT)floorf(pos.x / GM_TERRAIN_SIZE);
	gz = (INT)floorf(pos.z / GM_TERRAIN_SIZE);

	if (srt.pos.y < height) {
		srt.pos.y = height;
		Destroy();
	}
	if (srt.pos.y > GM_MAX_HEIGHT)
		srt.pos.y = GM_MAX_HEIGHT;

	fHeight = srt.pos.y - height;
}

//
// マップの外に出られないようにコントロールする。
//
void PLAYER::SetPlayerInBound(TERRAIN * terrain)
{
	FLOAT pos_max = (FLOAT)(terrain->nBlockSize * terrain->nMapSize / 2) / 2.0f;
	FLOAT pos_min = -pos_max;

	srt.pos.x = srt.pos.x > pos_max ? pos_max : srt.pos.x;
	srt.pos.z = srt.pos.z > pos_max ? pos_max : srt.pos.z;
	srt.pos.x = srt.pos.x < pos_min ? pos_min : srt.pos.x;
	srt.pos.z = srt.pos.z < pos_min ? pos_min : srt.pos.z;
}

//
// スコアを更新する。
//
void PLAYER::UpdateScore(DWORD* pNewScore)
{
	pGame->gameUI->UpdateScore(this, pNewScore);
	if (pNewScore) {
		nScore = *pNewScore;
	}
}

//
// HPを更新する。
//
void PLAYER::UpdateHealth(INT* pNewHealth)
{
	pGame->gameUI->UpdateHealth(this, pNewHealth);
	if (pNewHealth) {
		nHealth = *pNewHealth;
	}
}
