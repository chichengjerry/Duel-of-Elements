#include "player.h"
#include "bullet.h"
#include "game.h"
#include "camera.h"
#include "element.h"
#include "input.h"
#include "terrain.h"

PLAYER::PLAYER(MAINGAME* game, int playerId, BOOL isControlable)
{
	pGame = game;
	id = playerId;
	bControlable = isControlable;
	bFPSCamera = FALSE;
	nHealth = 100;
	fSize = PLAYER_SIZE;
	dwScore =
	dwScoreAnimate = 0;
	mScoreEasing = EASING(BEZIER_EASE_IN_OUT);
	fMoveSpeed = MOVE_MIN_SPEED;
	fRollSpeed = ROLL_MAX_ANGLE;
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
}

PLAYER::~PLAYER()
{
	SAFE_DELETE(plane);
	SAFE_DELETE(propeller);
	SAFE_DELETE(camera);
}

HRESULT PLAYER::Draw()
{
	plane->Draw(NULL);
	propeller->Draw(NULL);
	return S_OK;
}

void PLAYER::Update(TERRAIN* terrain)
{
	UpdateMovement(terrain);
	UpdateScore(NULL);
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
}

void PLAYER::SwapElements()
{
	ELEMENTTYPE temp;
	temp = attackType;
	attackType = defenseType;
	defenseType = temp;
}

void PLAYER::Fire(void)
{
	if (attackType != ELEMENTTYPE_NONE) {
		BULLET::Add(pGame->bullets, GM_MAX_BULLETS, this, NULL);
	}
}

void PLAYER::UpdateMovement(TERRAIN* terrain)
{
	BOOL	rolling = FALSE;
	BOOL	pitching = FALSE;

	if (bControlable) {

		if (id == 0) {
			if (DINPUT::GetKeyboardTrigger(DIK_Q)) {
				bFPSCamera = !bFPSCamera;
			}
			if (DINPUT::GetKeyboardTrigger(DIK_E)) {
				Fire();
			}
			if (DINPUT::GetKeyboardTrigger(DIK_C)) {
				SwapElements();
			}
			// スピード調整
			if (DINPUT::GetKeyboardPress(DIK_Z) && DINPUT::GetKeyboardPress(DIK_X)) {}
			else if (DINPUT::GetKeyboardPress(DIK_Z)) {
				if (fMoveSpeed < MOVE_MAX_SPEED)
					fMoveSpeed += MOVE_STEP;
			}
			else if (DINPUT::GetKeyboardPress(DIK_X)) {
				if (fMoveSpeed > MOVE_MIN_SPEED)
					fMoveSpeed -= MOVE_STEP;
			}

			// 垂直方向調整
			if (DINPUT::GetKeyboardPress(DIK_W) && DINPUT::GetKeyboardPress(DIK_S)) {}
			else if (DINPUT::GetKeyboardPress(DIK_W)) {
				pitching = TRUE;
				if (srt.rot.x > -PITCH_MAX_ANGLE)
					srt.rot.x -= PITCH_STEP;
			}
			else if (DINPUT::GetKeyboardPress(DIK_S)) {
				pitching = TRUE;
				if (srt.rot.x < PITCH_MAX_ANGLE)
					srt.rot.x += PITCH_STEP;
			}

			// 水平方向調整
			if (DINPUT::GetKeyboardPress(DIK_A) && DINPUT::GetKeyboardPress(DIK_D)) {}
			else if (DINPUT::GetKeyboardPress(DIK_A)) {
				rolling = TRUE;
				if (srt.rot.z > -ROLL_MAX_ANGLE)
					srt.rot.z -= ROLL_STEP;

				if (fYawSpeed > -YAW_MAX_ANGLE)
					fYawSpeed -= YAW_STEP;
			}
			else if (DINPUT::GetKeyboardPress(DIK_D)) {
				rolling = TRUE;
				if (srt.rot.z < ROLL_MAX_ANGLE)
					srt.rot.z += ROLL_STEP;

				if (fYawSpeed < YAW_MAX_ANGLE)
					fYawSpeed += YAW_STEP;
			}
		}

		if (id == 1) {
			if (DINPUT::GetKeyboardTrigger(DIK_Y)) {
				bFPSCamera = !bFPSCamera;
			}
			if (DINPUT::GetKeyboardTrigger(DIK_I)) {
				Fire();
			}
			if (DINPUT::GetKeyboardTrigger(DIK_COMMA)) {
				SwapElements();
			}
			if (DINPUT::GetKeyboardPress(DIK_N) && DINPUT::GetKeyboardPress(DIK_M)) {}
			else if (DINPUT::GetKeyboardPress(DIK_N)) {
				if (fMoveSpeed < MOVE_MAX_SPEED)
					fMoveSpeed += MOVE_STEP;
			}
			else if (DINPUT::GetKeyboardPress(DIK_M)) {
				if (fMoveSpeed > MOVE_MIN_SPEED)
					fMoveSpeed -= MOVE_STEP;
			}

			// 垂直方向調整
			if (DINPUT::GetKeyboardPress(DIK_U) && DINPUT::GetKeyboardPress(DIK_J)) {}
			else if (DINPUT::GetKeyboardPress(DIK_U)) {
				pitching = TRUE;
				if (srt.rot.x > -PITCH_MAX_ANGLE)
					srt.rot.x -= PITCH_STEP;
			}
			else if (DINPUT::GetKeyboardPress(DIK_J)) {
				pitching = TRUE;
				if (srt.rot.x < PITCH_MAX_ANGLE)
					srt.rot.x += PITCH_STEP;
			}

			// 水平方向調整
			if (DINPUT::GetKeyboardPress(DIK_H) && DINPUT::GetKeyboardPress(DIK_K)) {}
			else if (DINPUT::GetKeyboardPress(DIK_H)) {
				rolling = TRUE;
				if (srt.rot.z > -ROLL_MAX_ANGLE)
					srt.rot.z -= ROLL_STEP;

				if (fYawSpeed > -YAW_MAX_ANGLE)
					fYawSpeed -= YAW_STEP;
			}
			else if (DINPUT::GetKeyboardPress(DIK_K)) {
				rolling = TRUE;
				if (srt.rot.z < ROLL_MAX_ANGLE)
					srt.rot.z += ROLL_STEP;

				if (fYawSpeed < YAW_MAX_ANGLE)
					fYawSpeed += YAW_STEP;
			}

		}

	}

	if (!pitching) {
		if (fabsf(srt.rot.x) < FLT_EPSILON) {
			srt.rot.x = 0.0f;
		}
		else if (srt.rot.x > 0.0f)
			srt.rot.x -= PITCH_STEP;
		else if (srt.rot.x < 0.0f)
			srt.rot.x += PITCH_STEP;
	}
	if (!rolling) {
		fYawSpeed = 0.0f;
		if (fabsf(srt.rot.z) < FLT_EPSILON) {
			srt.rot.z = 0.0f;
		}
		else if (srt.rot.z > 0.0f)
			srt.rot.z -= ROLL_STEP;
		else if (srt.rot.z < 0.0f)
			srt.rot.z += ROLL_STEP;
	}
	else {
		srt.rot.y += fYawSpeed;
	}
	//
	// fRollSpeed = ROLL_IDLE_SPEED + 10.0f * (fMoveSpeed - MOVE_MIN_SPEED) * YAW_STEP;
	//
	D3DXVECTOR3 dir = D3DXVECTOR3(0.0f, 0.0f, fMoveSpeed);
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

void PLAYER::SetPlayerHeight(TERRAIN* terrain)
{
	D3DXVECTOR3 pos = srt.pos;

	FLOAT height = terrain->GetGroundHeight(pos.x, pos.z);

	gx = (INT)floorf(pos.x / GM_TERRAIN_SIZE);
	gz = (INT)floorf(pos.z / GM_TERRAIN_SIZE);

	if(srt.pos.y < height + fSize)
		srt.pos.y = height + fSize;
	if (srt.pos.y > GM_MAX_HEIGHT)
		srt.pos.y = GM_MAX_HEIGHT;

	fHeight = srt.pos.y - height;
}

void PLAYER::SetPlayerInBound(TERRAIN * terrain)
{
	FLOAT pos_max = (FLOAT)(terrain->nBlockSize * terrain->nMapSize / 2) / 2.0f;
	FLOAT pos_min = -pos_max;

	srt.pos.x = srt.pos.x > pos_max ? pos_max : srt.pos.x;
	srt.pos.z = srt.pos.z > pos_max ? pos_max : srt.pos.z;
	srt.pos.x = srt.pos.x < pos_min ? pos_min : srt.pos.x;
	srt.pos.z = srt.pos.z < pos_min ? pos_min : srt.pos.z;
}

void PLAYER::UpdateScore(DWORD* pNewScore)
{
	if (pNewScore) {
		mScoreEasing.SetTimer(dwScore, *pNewScore, 1000);
		dwScore = *pNewScore;
	}
	else {
		if (mScoreEasing.bEasing)
			dwScoreAnimate = mScoreEasing.GetValue();
		else
			dwScoreAnimate = dwScore;
	}
}
