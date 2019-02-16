#include "game.h"
#include "barrier.h"
#include "bullet.h"
#include "camera.h"
#include "element.h"
#include "light.h"
#include "player.h"
#include "sky.h"
#include "terrain.h"
#include "sound.h"
#include "ui_game.h"
#include "stage.h"

MAINGAME::MAINGAME(GAMESTAGE* state,INT nPlayer)
{
	srand((unsigned)time(NULL));

	pState = state;
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

	DSOUND::Play(SOUND_BGM);
}

MAINGAME::~MAINGAME()
{
	DSOUND::Stop(SOUND_BGM);

	for (int i = 0; i < GM_MAX_PLAYER; i++) {
		SAFE_DELETE(players[i]);
	}
	for (int i = 0; i < GM_MAX_ELEMENTS; i++) {
		SAFE_DELETE(elements[i]);
	}
	for (int i = 0; i < GM_MAX_BULLETS; i++) {
		SAFE_DELETE(bullets[i]);
	}
	SAFE_DELETE(barrier);
	SAFE_DELETE(sky);
	SAFE_DELETE(terrain);
	SAFE_DELETE(gameUI);
}

void MAINGAME::Update(void)
{
	sky->Update();
	barrier->Update();
	terrain->Update();

	BOOL bTerrainSound = FALSE;

	for (int i = 0; i < GM_MAX_PLAYER; i++) {
		players[i]->Update(terrain);

		// set terrain sound
		if (players[i]->fHeight < players[i]->fSize * 10) {
			bTerrainSound = TRUE;
		}
	}

	if (bTerrainSound) {
		DSOUND::Play(SOUND_TERRAIN);
	}
	else {
		DSOUND::Stop(SOUND_TERRAIN);
	}

	for (int i = 0; i < GM_MAX_ELEMENTS; i++) {
		elements[i]->Update(players);
	}

	for (int i = 0; i < GM_MAX_BULLETS; i++) {
		if (bullets[i]) {
			if (bullets[i]->bAlive)
				bullets[i]->Update(terrain);
			else
				SAFE_DELETE(bullets[i]);
		}
	}

	gameUI->Update();
}

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

		barrier->Draw(camera);

		for (int j = 0; j < GM_MAX_PLAYER; j++) {
			if(!players[i]->bFPSCamera || players[i] != players[j])
				players[j]->Draw(camera);

#if _DEBUG
			WCHAR str[1024];
			swprintf_s(str, _T(" Player %d\n X: %.1f, Y: %.1f, Z: %.1f\n Speed: %.1f Height: %.1f \n Score: %d"),
				players[j]->id,
				players[j]->srt.pos.x, players[j]->srt.pos.y, players[j]->srt.pos.z,
				players[j]->fMoveSpeed, players[j]->fHeight,
				players[j]->dwScoreAnimate);
			D3D::ShowText(str, CL_WIDTH / 2 * players[j]->id, 0);
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

		gameUI->Draw();

		pDevice->EndScene();

		pDevice->Present(&rect, &rect, NULL, NULL);
	}

	pDevice->SetViewport(&vp0);

	pDevice->BeginScene();

	return S_OK;
}
