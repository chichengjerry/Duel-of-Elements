//-----------------------------------------------
// [game.cpp]
// ゲーム処理
// 作者 Chicheng Zhang
//-----------------------------------------------
#include "game.h"
#include "barrier.h"
#include "bullet.h"
#include "camera.h"
#include "element.h"
#include "explosion.h"
#include "light.h"
#include "player.h"
#include "sky.h"
#include "terrain.h"
#include "sound.h"
#include "ui_game.h"
#include "stage.h"

//
// コンストラクター
//
MAINGAME::MAINGAME(GAMESTAGE* state,INT nPlayer)
{
	srand((unsigned)time(NULL));
	bGameOver = FALSE;
	winner[0] = winner[1] = GAME_RESULT_WIN;

	pState = state;

	//Init Elements
	ELEMENT::LoadTexture();
	EXPLOSION::LoadTextures();
	sky = new SKY();
	terrain = new TERRAIN(GM_TERRIAN_LEVEL);
	barrier = new BARRIER(terrain);
	for (int i = 0; i < GM_MAX_PLAYER; i++) {
		players[i] = new PLAYER(this, i, (BOOL)(i < nPlayer));
	}

	for (int i = 0; i < GM_MAX_ELEMENTS; i++){
		elements[i] = new ELEMENT(terrain);
	}
	gameUI = new MAINGAMEUI(this);

	// set timer
	nTimeRemain = GAME_TIME * 1000;
	nTimeLast = 0;

	DSOUND::Play(SOUND_BGM_GAME);
}

//
// デストラクター
//
MAINGAME::~MAINGAME()
{
	DSOUND::Stop(SOUND_BGM_GAME);

	for (int i = 0; i < GM_MAX_PLAYER; i++) {
		SAFE_DELETE(players[i]);
	}
	for (int i = 0; i < GM_MAX_ELEMENTS; i++) {
		SAFE_DELETE(elements[i]);
	}
	for (int i = 0; i < GM_MAX_BULLETS; i++) {
		SAFE_DELETE(bullets[i]);
	}
	for (int i = 0; i < GM_MAX_EXPLOSION; i++) {
		SAFE_DELETE(explosion[i]);
	}

	SAFE_DELETE(barrier);
	SAFE_DELETE(sky);
	SAFE_DELETE(terrain);
	SAFE_DELETE(gameUI);
	EXPLOSION::ReleaseTextures();
	ELEMENT::ReleaseTexture();
}

//
// ゲーム内容を更新する。
//
void MAINGAME::Update(void)
{
	BOOL bTerrainSound = FALSE;

	if (!bGameOver) {
		if (nTimeLast)
			nTimeRemain -= (D3D::dwTime - nTimeLast);

		nTimeLast = D3D::dwTime;

		if (nTimeRemain < 0) {
			nTimeRemain = 0;
			if (players[0]->nScore > players[1]->nScore) {
				GameOver(1, GAME_RESULT_LOSE);
			}
			else if (players[0]->nScore < players[1]->nScore) {
				GameOver(0, GAME_RESULT_LOSE);
			}
			else {
				GameOver(0, GAME_RESULT_DRAW);
				GameOver(1, GAME_RESULT_DRAW);
			}
		}
		sky->Update();
		barrier->Update();
		terrain->Update();

		PlayerCollisionCheck();
	}
	else {
		DSOUND::Stop(SOUND_TERRAIN);

		if (DINPUT::KeyTriggered(DIK_RETURN) || DINPUT::ButtonTriggered(BUTTON_START)) {
			pState->SetStage(STAGE_TITLE);
		}
	}

	for (int i = 0; i < GM_MAX_PLAYER; i++) {
		players[i]->Update(terrain);

		// set terrain sound
		if (players[i]->fHeight < players[i]->fSize * 10) {
			bTerrainSound = TRUE;
		}

		for (int j = 0; j < GM_MAX_BULLETS; j++) {
			if (bullets[j] && bullets[j]->HitPlayer(players[i])) {

				players[i]->Damaged(bullets[j]);

				// prevent further damage
				bullets[j]->bulletType = ELEMENTTYPE_NONE;
			}
		}
	}
	if (!bGameOver && bTerrainSound)
		DSOUND::Play(SOUND_TERRAIN);
	else
		DSOUND::Stop(SOUND_TERRAIN);

	for (int i = 0; i < GM_MAX_ELEMENTS; i++) {
		elements[i]->Update(players);
	}

	for (int i = 0; i < GM_MAX_BULLETS; i++) {
		if (bullets[i]) {
			if (bullets[i]->bAlive) {
				bullets[i]->Update(terrain);
			}
			else
				SAFE_DELETE(bullets[i]);
		}
	}

	for (int i = 0; i < GM_MAX_EXPLOSION; i++) {
		if (explosion[i]) {
			if (explosion[i]->particle->bAlive) {
				explosion[i]->Update();
			}
			else
				SAFE_DELETE(explosion[i]);
		}
	}

	gameUI->Update();
}

//
// ゲーム内容を描画する。
//
HRESULT MAINGAME::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = D3D::GetDevice();
	D3DVIEWPORT9 vp0;

	pDevice->GetViewport(&vp0);
	pDevice->EndScene();

	for (int i = 0; i < GM_MAX_PLAYER; i++) {
		CAMERA* camera = players[i]->camera;

		camera->SetCamera();

		D3DVIEWPORT9 vp = camera->viewport;

		RECT rect = { (LONG)vp.X, (LONG)vp.Y, (LONG)(vp.X + vp.Width), (LONG)(vp.Y + vp.Height) };

		pDevice->Clear(0, (D3DRECT*)&rect, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(255, 255, 255, 255), 1.0f, 0);

		pDevice->BeginScene();

		pDevice->SetViewport(&vp);

		sky->Draw(camera);

		D3D::EnableFog(sky->light->GetFogColor());
		terrain->Draw(camera);
		D3D::DisableFog();

		for (int j = 0; j < GM_MAX_PLAYER; j++) {
			if(!players[i]->bFPSCamera || players[i] != players[j])
				players[j]->Draw(camera);

#if _DEBUG
			WCHAR str[1024];
			swprintf_s(str, _T(" Player %d\n X: %.1f, Y: %.1f, Z: %.1f\n Speed: %.1f Height: %.1f \n Health: %d"),
				players[j]->id,
				players[j]->srt.pos.x, players[j]->srt.pos.y, players[j]->srt.pos.z,
				players[j]->fMoveSpeed, players[j]->fHeight,
				players[j]->nHealth);
			D3D::ShowText(str, CL_WIDTH / 2 * players[j]->id, 128);
#endif // _DEBUG
		}

		for (int j = 0; j < GM_MAX_ELEMENTS; j++) {
			elements[j]->Draw(camera);
		}

		for (int j = 0; j < GM_MAX_BULLETS; j++) {
			if (bullets[j]) {
				bullets[j]->Draw(camera);
			}
		}

		for (int j = 0; j < GM_MAX_EXPLOSION; j++) {
			if (explosion[j]) {
				explosion[j]->Draw(camera);
			}
		}

		barrier->Draw(camera);

		gameUI->Draw();

		pDevice->EndScene();

		pDevice->Present(&rect, &rect, NULL, NULL);
	}

	pDevice->SetViewport(&vp0);

	pDevice->BeginScene();

	return S_OK;
}

//
// ゲーム終了で、勝負を設定する。。
//
void MAINGAME::GameOver(int playerId, BOOL win)
{
	winner[playerId] = win;
	bGameOver = TRUE;
}

//
// プレイヤー同士がぶつかったかどうかをチェックする。
//
void MAINGAME::PlayerCollisionCheck()
{
	D3DXVECTOR3 d = players[0]->srt.pos - players[1]->srt.pos;
	FLOAT s = players[0]->fSize + players[1]->fSize;
	
	if (D3DXVec3LengthSq(&d) <= (s * s)) {
		players[0]->Destroy();
		players[1]->Destroy();
		players[0]->nHealth = 0;
		players[1]->nHealth = 0;
	}
}
