#include "player.h"
#include "camera.h"
#include "element.h"
#include "game.h"
#include "input.h"
#include "terrian.h"

#define PLAYER_YAW			D3DXToRadian(1.0f)
#define PLAYER_ROLL			D3DXToRadian(5.0f)
#define PLAYER_STEP			D3DXToRadian(1.0f)
#define ROLL_MOVE_SPEED		D3DXToRadian(30.0f)
#define ROLL_IDLE_SPEED		D3DXToRadian(10.0f)
#define MOVE_MAX_SPEED		1.0f
#define MOVE_MIN_SPEED		0.125f
#define MOVE_STEP			0.125f

PLAYER::PLAYER(int playerId, BOOL isComputer, D3DXVECTOR3 spawnpoint)
{
	id = playerId;
	nHealth = 100;
	fMoveSpeed = MOVE_MIN_SPEED;
	fRollSpeed = ROLL_IDLE_SPEED;
	attackType = ELEMENTTYPE_NONE;
	defenseType = ELEMENTTYPE_NONE;

	isPlayer = !isComputer;

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

	WCHAR str[1024];

	swprintf_s(str, _T("player %d Axis X: %f, Y: %f, Z: %f\n Grid X: %d, Z: %d"),
		id, srt.pos.x, srt.pos.y, srt.pos.z, gx, gz);
	D3D::ShowText(str, 0, id * 36);

	return S_OK;
}

void PLAYER::Update(MAINGAME* pGame)
{
	Move(pGame);
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

void PLAYER::Move(MAINGAME * pGame)
{
	BOOL	rolling = FALSE;
	BOOL	moving = FALSE;

	fRollSpeed = ROLL_IDLE_SPEED;

	if (id == 0) {
		if (DINPUT::GetKeyboardPress(DIK_W)) {
			moving = TRUE;

			if (fMoveSpeed < MOVE_MAX_SPEED)
				fMoveSpeed += MOVE_STEP;
		}

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
		if (DINPUT::GetKeyboardPress(DIK_U)) {
			moving = TRUE;

			if (fMoveSpeed < MOVE_MAX_SPEED)
				fMoveSpeed += MOVE_STEP;
		}

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

	if (!moving) {
		if (fMoveSpeed > MOVE_MIN_SPEED)
			fMoveSpeed -= MOVE_STEP;
	}
	else {
		fRollSpeed = ROLL_MOVE_SPEED;
	}

	D3DXVECTOR3 dir;
	D3DXVECTOR4 temp;
	D3DXMATRIX mtx;

	D3DXMatrixIdentity(&mtx);
	D3DXMatrixRotationYawPitchRoll(&mtx, srt.rot.y, srt.rot.x, srt.rot.z);

	dir = D3DXVECTOR3(0.0f, 0.0f, fMoveSpeed);
	D3DXVec3Transform(&temp, &dir, &mtx);
	srt.pos += D3DXVECTOR3(temp.x, temp.y, temp.z);

	FLOAT pos_max = (FLOAT)(GM_TERRAIN_SIZE * pGame->terrain->dwMapSize - 1);

	if (srt.pos.x < GM_TERRAIN_SIZE)
		srt.pos.x = GM_TERRAIN_SIZE;
	if (srt.pos.x > pos_max)
		srt.pos.x = pos_max;

	if (srt.pos.z < GM_TERRAIN_SIZE)
		srt.pos.z = GM_TERRAIN_SIZE;
	if (srt.pos.z > pos_max)
		srt.pos.z = pos_max;

	if (!rolling) {
		if (srt.rot.z > 0.0f)
			srt.rot.z -= PLAYER_STEP;
		if (srt.rot.z < 0.0f)
			srt.rot.z += PLAYER_STEP;
	}

	SetPlayerHeight(pGame);
}

void PLAYER::SetPlayerHeight(MAINGAME * pGame)
{
	D3DXVECTOR3 pos = srt.pos;

	FLOAT height = pGame->terrain->GetGroundHeight(pos.x, pos.z);

	gx = (DWORD)floorf(pos.x / GM_TERRAIN_SIZE);
	gz = (DWORD)floorf(pos.z / GM_TERRAIN_SIZE);

	srt.pos.y = height + 20.0f;
}
