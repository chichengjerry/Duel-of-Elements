#include "player.h"
#include "game.h"
#include "input.h"

#define PLAYER_ROTATION		D3DXToRadian(1.0f)

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

	WCHAR model[1024];
	wsprintf(model, _T("data/MODEL/player%d.x"), id);
	mdl = new MODEL((LPCWSTR)model, NULL);
	
}

PLAYER::~PLAYER()
{
	delete camera;
	delete mdl;
}

HRESULT PLAYER::Draw()
{
	mdl->Draw();

	return S_OK;
}

void PLAYER::Update()
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
			srt.rot.y -= PLAYER_ROTATION;
		}
		else if (DINPUT::GetKeyboardPress(DIK_D)) {
			srt.rot.y += PLAYER_ROTATION;
		}

		else if (DINPUT::GetKeyboardPress(DIK_Z)) {
			if(camera->pitch > -90.0f)
				camera->pitch -= 1.0f;
		}
		else if (DINPUT::GetKeyboardPress(DIK_X)) {
			if (camera->pitch < -30.0f)
				camera->pitch += 1.0f;
		}
	}

	if (id == 1) {
		if (DINPUT::GetKeyboardPress(DIK_U) && DINPUT::GetKeyboardPress(DIK_J)) {}
		else if (DINPUT::GetKeyboardPress(DIK_U)) {
			dir = D3DXVECTOR3(0.0f, 0.0f, fMoveSpeed);
			D3DXVec3Transform(&temp, &dir, &mtx);
			srt.pos += D3DXVECTOR3(temp.x, temp.y, temp.z);
		}
		else if (DINPUT::GetKeyboardPress(DIK_J)) {
			dir = D3DXVECTOR3(0.0f, 0.0f, -fMoveSpeed);
			D3DXVec3Transform(&temp, &dir, &mtx);
			srt.pos += D3DXVECTOR3(temp.x, temp.y, temp.z);
		}
		if (DINPUT::GetKeyboardPress(DIK_H) && DINPUT::GetKeyboardPress(DIK_K)) {}
		else if (DINPUT::GetKeyboardPress(DIK_H)) {
			srt.rot.y -= PLAYER_ROTATION;
		}
		else if (DINPUT::GetKeyboardPress(DIK_K)) {
			srt.rot.y += PLAYER_ROTATION;
		}

		else if (DINPUT::GetKeyboardPress(DIK_N)) {
			if (camera->pitch > -90.0f)
				camera->pitch -= 1.0f;
		}
		else if (DINPUT::GetKeyboardPress(DIK_M)) {
			if (camera->pitch < -30.0f)
				camera->pitch += 1.0f;
		}
	}
	
	mdl->srt = srt;
	mdl->Update();

	camera->Update();
}
