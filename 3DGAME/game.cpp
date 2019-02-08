#include "game.h"
#include "camera.h"
#include "element.h"
#include "light.h"
#include "player.h"
#include "sky.h"
#include "terrain.h"

MAINGAME::MAINGAME(INT nPlayer)
{
	srand((unsigned)time(NULL));

	light = new LIGHT();
	sky = new SKY();
	terrain = new TERRAIN(GM_TERRIAN_LEVEL);

	for (int i = 0; i < GM_MAX_PLAYER; i++) {
		FLOAT x = (FLOAT)(GET_RANDOM(1, terrain->dwMapSize - 1) * GM_TERRAIN_SIZE);
		FLOAT z = (FLOAT)(GET_RANDOM(1, terrain->dwMapSize - 1) * GM_TERRAIN_SIZE);
		D3DXVECTOR3 pos = D3DXVECTOR3(x, (FLOAT)(GET_RANDOM(GM_MAX_HEIGHT / 2, GM_MAX_HEIGHT)), z);
		
		player[i] = new PLAYER(i, (BOOL)(i < nPlayer), pos);
	}
	for (int i = 0; i < GM_MAX_ELEMENTS; i++){
		elements[i] = new ELEMENT(terrain);
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
	light->Update();
	terrain->Update();

	for (int i = 0; i < GM_MAX_PLAYER; i++) {
		player[i]->Update(terrain);
	}

	for (int i = 0; i < GM_MAX_ELEMENTS; i++) {
		elements[i]->Update(player);
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

		terrain->Draw(camera);

		for (int j = 0; j < GM_MAX_PLAYER; j++) {
			if(!player[i]->bFPSCamera || player[i] != player[j])
				player[j]->Draw();

#if _DEBUG
			WCHAR str[1024];
			swprintf_s(str, _T(" Player %d\n X: %f, Y: %f, Z: %f\n Speed: %f Height: %f \n Score: %d"),
				player[j]->id,
				player[j]->srt.pos.x, player[j]->srt.pos.y, player[j]->srt.pos.z,
				player[j]->fMoveSpeed, player[j]->fHeight,
				player[j]->dwScoreAnimate);
			D3D::ShowText(str, CL_WIDTH / 2 * player[j]->id, 0);
#endif // _DEBUG
		}

		for (int j = 0; j < GM_MAX_ELEMENTS; j++) {
			elements[j]->Draw(player[i]->camera);
		}

		pDevice->EndScene();

		pDevice->Present(&rect, &rect, NULL, NULL);
	}

	pDevice->SetViewport(&vp0);

	pDevice->BeginScene();

	return S_OK;
}
