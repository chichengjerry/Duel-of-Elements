#include "ui_game.h"
#include "game.h"
#include "player.h"

#define IMAGE_ELEMENT		_T("data/TEXTURE/ui/elements.png")
#define IMAGE_STATUS		_T("")

LPDIRECT3DTEXTURE9	MAINGAMEUI::pTexElement = NULL;
LPDIRECT3DTEXTURE9	MAINGAMEUI::pTexStatus = NULL;

MAINGAMEUI::MAINGAMEUI(MAINGAME* game)
{
	pGame = game;

	const FLOAT DIGIT_GAP = 24.0f;
	const FLOAT DIGIT_SIZE = 32.0f;

	if (pGame->players[0]) {
		for (int j = 0; j < UI_MAX_DIGITS; j++) {
			D3DXVECTOR3 vtx[4];

			// score
			vtx[0] = D3DXVECTOR3(j * DIGIT_GAP, DIGIT_GAP, 0.0f);
			vtx[1] = D3DXVECTOR3(j * DIGIT_GAP + DIGIT_SIZE, DIGIT_GAP, 0.0f);
			vtx[2] = D3DXVECTOR3(j * DIGIT_GAP, DIGIT_GAP + DIGIT_SIZE, 0.0f);
			vtx[3] = D3DXVECTOR3(j * DIGIT_GAP + DIGIT_SIZE, DIGIT_GAP + DIGIT_SIZE, 0.0f);

			score[0][j] = new DIGITIMAGE(0, vtx);
			score[0][j]->image->SetDiffuseColor(&D3DXCOLOR(1.0f, 0.25f, 0.25f, 1.0f));

		}
	}

	if (pGame->players[1]) {
		for (int j = 0; j < UI_MAX_DIGITS; j++) {
			D3DXVECTOR3 vtx[4];

			// score
			D3DXVECTOR3 offset = D3DXVECTOR3(CL_WIDTH - DIGIT_SIZE * 6 + (DIGIT_SIZE - DIGIT_GAP) * 5.0f, 0.0f, 0.0f);
			vtx[0] = offset + D3DXVECTOR3(j * DIGIT_GAP, DIGIT_GAP, 0.0f);
			vtx[1] = offset + D3DXVECTOR3(j * DIGIT_GAP + DIGIT_SIZE, DIGIT_GAP, 0.0f);
			vtx[2] = offset + D3DXVECTOR3(j * DIGIT_GAP, DIGIT_GAP + DIGIT_SIZE, 0.0f);
			vtx[3] = offset + D3DXVECTOR3(j * DIGIT_GAP + DIGIT_SIZE, DIGIT_GAP + DIGIT_SIZE, 0.0f);
			score[1][j] = new DIGITIMAGE(0, vtx);
			score[1][j]->image->SetDiffuseColor(&D3DXCOLOR(0.25f, 1.0f, 0.25f, 1.0f));

			//elements
			// vtx[0] = D3DXVECTOR3()
			// vtx[1] = D3DXVECTOR3()
			// vtx[2] = D3DXVECTOR3()
			// vtx[3] = D3DXVECTOR3()
			// elements[1][0] = new IMAGE(pTexElement, )
		}
	}
	
}

MAINGAMEUI::~MAINGAMEUI()
{
	SAFE_RELEASE(pTexElement);
	SAFE_RELEASE(pTexStatus);
}

void MAINGAMEUI::Update()
{
	for (int i = 0; i < GM_MAX_PLAYER; i++) {
		INT value = pGame->players[i]->dwScoreAnimate;

		for (int j = 0; j < UI_MAX_DIGITS; j++) {
			score[i][UI_MAX_DIGITS - j - 1]->SetDigit(value % 10);
			value /= 10;
		}
	}
}

HRESULT MAINGAMEUI::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = D3D::GetDevice();

	pDevice->EndScene();

	pDevice->BeginScene();

	for (int i = 0; i < GM_MAX_PLAYER; i++) {
		for (int j = 0; j < UI_MAX_DIGITS; j++) {
			score[i][j]->Draw();
		}
	}

	pDevice->Present(NULL, NULL, NULL, NULL);

	pDevice->EndScene();

	pDevice->BeginScene();

	return S_OK;
}

HRESULT MAINGAMEUI::LoadTexture()
{
	LPDIRECT3DDEVICE9 pDevice = D3D::GetDevice();

	if (FAILED(D3DXCreateTextureFromFile(pDevice,			// デバイスへのポインタ
		IMAGE_ELEMENT,										// ファイルの名前
		&pTexElement)))										// 読み込むメモリー
	{
		return E_FAIL;
	}
	if (FAILED(D3DXCreateTextureFromFile(pDevice,			// デバイスへのポインタ
		IMAGE_STATUS,										// ファイルの名前
		&pTexStatus)))										// 読み込むメモリー
	{
		return E_FAIL;
	}

	return S_OK;
}
