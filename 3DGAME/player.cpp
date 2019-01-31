#include "player.h"
#include "game.h"
#include "input.h"

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
	camera->SetCamera();
	game->map->Draw();
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
			dir = D3DXVECTOR3(fMoveSpeed, 0.0f, 0.0f);
			D3DXVec3Transform(&temp, &dir, &mtx);
			srt.pos += D3DXVECTOR3(temp.x, temp.y, temp.z);
		}
		else if (DINPUT::GetKeyboardPress(DIK_S)) {
			dir = D3DXVECTOR3(-fMoveSpeed, 0.0f, 0.0f);
			D3DXVec3Transform(&temp, &dir, &mtx);
			srt.pos += D3DXVECTOR3(temp.x, temp.y, temp.z);
		}
	}

	mdl->Update();
	camera->Update();
}
