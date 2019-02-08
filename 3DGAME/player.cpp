#include "player.h"
#include "camera.h"
#include "element.h"
#include "input.h"
#include "terrain.h"

#define PLAYER_YAW			D3DXToRadian(1.0f)
#define PLAYER_ROLL			D3DXToRadian(15.0f)
#define PLAYER_STEP			D3DXToRadian(1.0f)
#define ROLL_MOVE_SPEED		D3DXToRadian(30.0f)
#define ROLL_IDLE_SPEED		D3DXToRadian(10.0f)

#define MOVE_MAX_SPEED		1.0f
#define MOVE_MIN_SPEED		0.0f
#define MOVE_STEP			0.125f

PLAYER::PLAYER(int playerId, BOOL isControlable, D3DXVECTOR3 spawnpoint)
{
	id = playerId;
	bControlable = isControlable;
	bFPSCamera = FALSE;
	nHealth = 100;
	dwScore =
	dwScoreAnimate = 0;
	mScoreEasing = EASING(BEZIER_EASE_IN_OUT);
	fMoveSpeed = MOVE_MIN_SPEED;
	fRollSpeed = ROLL_MOVE_SPEED;
	elementType = ELEMENTTYPE_NONE;
	srt = SRT(spawnpoint);
	camera = new CAMERA(this);

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

void PLAYER::UpdateMovement(TERRAIN* terrain)
{
	BOOL	rolling = FALSE;
	BOOL	pitching = FALSE;

	if (bControlable) {

		if (id == 0) {
			if (DINPUT::GetKeyboardTrigger(DIK_Q)) {
				bFPSCamera = !bFPSCamera;
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

				if (srt.rot.x > -PLAYER_ROLL)
					srt.rot.x -= PLAYER_STEP;
			}
			else if (DINPUT::GetKeyboardPress(DIK_S)) {
				pitching = TRUE;

				if (srt.rot.x < PLAYER_ROLL)
					srt.rot.x += PLAYER_STEP;
			}

			// 水平方向調整
			if (DINPUT::GetKeyboardPress(DIK_A) && DINPUT::GetKeyboardPress(DIK_D)) {}
			else if (DINPUT::GetKeyboardPress(DIK_A)) {
				rolling = TRUE;
				if (srt.rot.z > -PLAYER_ROLL)
					srt.rot.z -= PLAYER_STEP;

				srt.rot.y -= PLAYER_YAW;
			}
			else if (DINPUT::GetKeyboardPress(DIK_D)) {
				rolling = TRUE;
				if (srt.rot.z < PLAYER_ROLL)
					srt.rot.z += PLAYER_STEP;

				srt.rot.y += PLAYER_YAW;
			}
		}

		if (id == 1) {
			if (DINPUT::GetKeyboardTrigger(DIK_Y)) {
				bFPSCamera = !bFPSCamera;
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
				if (srt.rot.x > -PLAYER_ROLL)
					srt.rot.x -= PLAYER_STEP;
			}
			else if (DINPUT::GetKeyboardPress(DIK_J)) {
				pitching = TRUE;
				if (srt.rot.x < PLAYER_ROLL)
					srt.rot.x += PLAYER_STEP;
			}

			// 水平方向調整
			if (DINPUT::GetKeyboardPress(DIK_H) && DINPUT::GetKeyboardPress(DIK_K)) {}
			else if (DINPUT::GetKeyboardPress(DIK_H)) {
				rolling = TRUE;
				if (srt.rot.z > -PLAYER_ROLL)
					srt.rot.z -= PLAYER_STEP;

				srt.rot.y -= PLAYER_YAW;
			}
			else if (DINPUT::GetKeyboardPress(DIK_K)) {
				rolling = TRUE;
				if (srt.rot.z < PLAYER_ROLL)
					srt.rot.z += PLAYER_STEP;

				srt.rot.y += PLAYER_YAW;
			}

		}

	}

	if (!pitching) {
		if (fabsf(srt.rot.x) < FLT_EPSILON) {
			srt.rot.x = 0.0f;
		}
		else if (srt.rot.x > 0.0f)
			srt.rot.x -= PLAYER_STEP;
		else if (srt.rot.x < 0.0f)
			srt.rot.x += PLAYER_STEP;
	}
	if (!rolling) {
		if (fabsf(srt.rot.z) < FLT_EPSILON) {
			srt.rot.z = 0.0f;
		}
		else if (srt.rot.z > 0.0f)
			srt.rot.z -= PLAYER_STEP;
		else if (srt.rot.z < 0.0f)
			srt.rot.z += PLAYER_STEP;
	}
	//
	// fRollSpeed = ROLL_IDLE_SPEED + 10.0f * (fMoveSpeed - MOVE_MIN_SPEED) * PLAYER_STEP;
	//
	D3DXVECTOR3 dir = D3DXVECTOR3(0.0f, 0.0f, fMoveSpeed);
	D3DXVECTOR4 temp;
	D3DXMATRIX mtx;

	D3DXMatrixRotationX(&mtx, -srt.rot.x);
	D3DXVec3Transform(&temp, &dir, &mtx);
	D3DXMatrixRotationY(&mtx, srt.rot.y);
	D3DXVec4Transform(&temp, &temp, &mtx);

	srt.pos += D3DXVECTOR3(temp.x, temp.y, temp.z);

	FLOAT pos_max = (FLOAT)(GM_TERRAIN_SIZE * terrain->dwMapSize);

	srt.pos.x = srt.pos.x > pos_max ? pos_max : srt.pos.x;
	srt.pos.z = srt.pos.z > pos_max ? pos_max : srt.pos.z;
	srt.pos.x = srt.pos.x < 0.0f ? 0.0f : srt.pos.x;
	srt.pos.z = srt.pos.z < 0.0f ? 0.0f : srt.pos.z;

	SetPlayerHeight(terrain);
}

void PLAYER::SetPlayerHeight(TERRAIN* terrain)
{
	D3DXVECTOR3 pos = srt.pos;

	FLOAT height = terrain->GetGroundHeight(pos.x, pos.z);

	gx = floorf(pos.x / GM_TERRAIN_SIZE);
	gz = floorf(pos.z / GM_TERRAIN_SIZE);

	if(srt.pos.y < height)
		srt.pos.y = height;
	if (srt.pos.y > GM_MAX_HEIGHT)
		srt.pos.y = GM_MAX_HEIGHT;

	fHeight = srt.pos.y - height;
}

void PLAYER::UpdateScore(DWORD* pNewScore)
{
	if (pNewScore) {
		mScoreEasing.SetTimer(dwScore, *pNewScore, 2000);
		dwScore = *pNewScore;
	}
	else {
		if (mScoreEasing.bEasing)
			dwScoreAnimate = mScoreEasing.GetValue();
		else
			dwScoreAnimate = dwScore;
	}
}
