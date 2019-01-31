#include "player.h"
#include "game.h"
#include "input.h"

#define PLAYER_ROTATION		D3DXToRadian(5.0f)

PLAYER::PLAYER(int playerId, BOOL isComputer, D3DXVECTOR3 spawn)
{
	id = playerId;
	nHealth = 100;
	fMoveSpeed = 1.0f;
	attackType = ELEMENTTYPE_NONE;
	defenseType = ELEMENTTYPE_NONE;

	isPlayer = !isComputer;

	srt = SRT(spawn);

	camera = new CAMERA(this);

	mdl = new MODEL(_T("data/MODEL/player.x"), NULL);
}

PLAYER::~PLAYER()
{
	delete camera;
	delete mdl;
}

HRESULT PLAYER::Draw(MAINGAME* game)
{
	mdl->Draw();

	return S_OK;
}

void PLAYER::Update(MAINGAME* game)
{
	D3DXVECTOR3 dir;
	D3DXVECTOR4 temp;
	D3DXMATRIX mtx;
	D3DXMatrixIdentity(&mtx);
	D3DXMatrixRotationYawPitchRoll(&mtx, srt.rot.y, srt.rot.x, srt.rot.z);

	if (id == 0) {
		if (DINPUT::GetKeyboardPress(DIK_W) && DINPUT::GetKeyboardPress(DIK_S)) {}
		else if (DINPUT::GetKeyboardPress(DIK_W)) {
			dir = D3DXVECTOR3(0.0f, 0.0f, fMoveSpeed);
			D3DXVec3Transform(&temp, &dir, &mtx);
			srt.pos += D3DXVECTOR3(temp.x, temp.y, temp.z);
		}
		else if (DINPUT::GetKeyboardPress(DIK_S)) {
			dir = D3DXVECTOR3(0.0f, 0.0f, -fMoveSpeed);
			D3DXVec3Transform(&temp, &dir, &mtx);
			srt.pos += D3DXVECTOR3(temp.x, temp.y, temp.z);
		}
		if (DINPUT::GetKeyboardPress(DIK_A) && DINPUT::GetKeyboardPress(DIK_D)) {}
		else if (DINPUT::GetKeyboardPress(DIK_A)) {
			srt.rot.y += PLAYER_ROTATION;
		}
		else if (DINPUT::GetKeyboardPress(DIK_D)) {
			srt.rot.y -= PLAYER_ROTATION;
		}
	}

	if (id == 1) {
		if (DINPUT::GetKeyboardPress(DIK_I) && DINPUT::GetKeyboardPress(DIK_K)) {}
		else if (DINPUT::GetKeyboardPress(DIK_I)) {
			dir = D3DXVECTOR3(0.0f, 0.0f, fMoveSpeed);
			D3DXVec3Transform(&temp, &dir, &mtx);
			srt.pos += D3DXVECTOR3(temp.x, temp.y, temp.z);
		}
		else if (DINPUT::GetKeyboardPress(DIK_K)) {
			dir = D3DXVECTOR3(0.0f, 0.0f, -fMoveSpeed);
			D3DXVec3Transform(&temp, &dir, &mtx);
			srt.pos += D3DXVECTOR3(temp.x, temp.y, temp.z);
		}
		if (DINPUT::GetKeyboardPress(DIK_J) && DINPUT::GetKeyboardPress(DIK_L)) {}
		else if (DINPUT::GetKeyboardPress(DIK_J)) {
			srt.rot.y += PLAYER_ROTATION;
		}
		else if (DINPUT::GetKeyboardPress(DIK_L)) {
			srt.rot.y -= PLAYER_ROTATION;
		}
	}
	mdl->Update();
	camera->Update();
}
