#include "game.h"
#include "camera.h"
#include "element.h"
#include "light.h"
#include "player.h"
#include "sky.h"
#include "terrian.h"

#define GM_TERRIAN_LEVEL		3

MAINGAME::MAINGAME(INT nPlayer)
{
	srand((unsigned)time(NULL));

	light = new LIGHT();
	sky = new SKY();
	terrain = new TERRAIN(GM_TERRIAN_LEVEL);

	for (int i = 0; i < GM_MAX_ELEMENTS; i++){

		FLOAT x = (FLOAT)(GET_RANDOM(1, terrain->dwMapSize - 1) * GM_TERRAIN_SIZE);
		FLOAT z = (FLOAT)(GET_RANDOM(1, terrain->dwMapSize - 1) * GM_TERRAIN_SIZE);
		FLOAT y = terrain->GetGroundHeight(x, z) + 20.0f;

		SRT srt = SRT(D3DXVECTOR3(x, y, z));

		elements[i] = new ELEMENT(&srt);
	}

	for (int i = 0; i < GM_MAX_PLAYER; i++) {
		
		FLOAT x = (FLOAT)(GET_RANDOM(1, terrain->dwMapSize - 1) * GM_TERRAIN_SIZE);
		FLOAT z = (FLOAT)(GET_RANDOM(1, terrain->dwMapSize - 1) * GM_TERRAIN_SIZE);
		D3DXVECTOR3 pos = D3DXVECTOR3(x, 0.0f, z);
		
		player[i] = new PLAYER(i, (BOOL)(i < nPlayer), pos);
	}
}

MAINGAME::~MAINGAME()
{
	for (int i = 0; i < GM_MAX_PLAYER; i++) {
		SAFE_DELETE(player[i]);
	}
	for (int i = 0; i < GM_MAX_ELEMENTS; i++) {
		SAFE_DELETE(elements[i]);
	}

	SAFE_DELETE(light);
	SAFE_DELETE(sky);
	SAFE_DELETE(terrain);
}

void MAINGAME::Update(void)
{
	sky->Update();
	terrain->Update();

	for (int i = 0; i < GM_MAX_PLAYER; i++) {
		player[i]->Update(this);
	}

	for (int i = 0; i < GM_MAX_ELEMENTS; i++) {
		elements[i]->Update(this);
	}
}

HRESULT MAINGAME::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = D3D::GetDevice();
	D3DVIEWPORT9 vp0;

	pDevice->GetViewport(&vp0);
	pDevice->EndScene();

	for (int i = 0; i < GM_MAX_PLAYER; i++) {
		CAMERA* camera = player[i]->camera;

		camera->SetCamera();

		D3DVIEWPORT9 vp = camera->viewport;

		pDevice->SetViewport(&vp);

		RECT rect = { (LONG)vp.X, (LONG)vp.Y, (LONG)(vp.X + vp.Width), (LONG)(vp.Y + vp.Height) };

		pDevice->Clear(0, (D3DRECT*)&rect, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(255, 255, 255, 255), 1.0f, 0);

		pDevice->BeginScene();

		sky->Draw(camera);

		terrain->Draw(player[i]);

		for (int j = 0; j < GM_MAX_PLAYER; j++) {
			player[j]->Draw();
		}

		for (int i = 0; i < GM_MAX_ELEMENTS; i++) {
			elements[i]->Draw();
		}

		pDevice->EndScene();

		pDevice->Present(&rect, &rect, NULL, NULL);
	}

	pDevice->SetViewport(&vp0);

	pDevice->BeginScene();

	return S_OK;
}
