#include "ui_game.h"
#include "game.h"
#include "player.h"

#define IMAGE_ELEMENT		_T("data/TEXTURE/ui/elements.png")
#define IMAGE_STRING		_T("data/TEXTURE/ui/string.png")
#define IMAGE_RESULT		_T("data/TEXTURE/ui/result.png")

LPDIRECT3DTEXTURE9	MAINGAMEUI::pTexElement = NULL;
LPDIRECT3DTEXTURE9	MAINGAMEUI::pTexStatus = NULL;
LPDIRECT3DTEXTURE9	MAINGAMEUI::pTexResult = NULL;

const DWORD DIGIT_GAP = 24;
const DWORD DIGIT_SIZE = 32;
const DWORD ELEMENT_SIZE = 48;
const DWORD TEXT_SIZE = 96;

MAINGAMEUI::MAINGAMEUI(MAINGAME* game)
{
	pGame = game;

	LPDIRECT3DDEVICE9 pDevice = D3D::GetDevice();

	D3D::LoadTexture(&pTexElement, IMAGE_ELEMENT);
	D3D::LoadTexture(&pTexStatus, IMAGE_STRING);
	D3D::LoadTexture(&pTexResult, IMAGE_RESULT);

	D3DRECT rect;

	if (pGame->players[0]) {
		// score
		dwScoreAnimate[0] = pGame->players[0]->dwScore;
		mScoreEasing[0] = EASING(BEZIER_EASE_IN_OUT);
		for (int j = 0; j < UI_MAX_DIGITS; j++) {
			rect = {
				(LONG)(DIGIT_GAP + j * DIGIT_GAP), DIGIT_SIZE, DIGIT_SIZE, DIGIT_SIZE
			};

			score[0][j] = new DIGITIMAGE(0, &rect);
			score[0][j]->image->SetDiffuseColor(D3DXCOLOR(1.0f, 0.25f, 0.25f, 1.0f));
		}

		for (int i = 0; i < 2; i++) {
			// elements
			rect = {
				(LONG)(ELEMENT_SIZE * 2), (LONG)(CL_HEIGHT - DIGIT_SIZE * (4 - i * 2)), ELEMENT_SIZE, ELEMENT_SIZE
			};
			elements[0][i] = new IMAGE(pTexElement, &rect);
			elements[0][i]->nFrameTotal = 5;
			elements[0][i]->SetTexture();

			// atk/def string
			rect = {
				0, (LONG)(CL_HEIGHT - DIGIT_SIZE * (4 - i * 2)), TEXT_SIZE, DIGIT_SIZE
			};
			string[0][i] = new IMAGE(pTexStatus, &rect);
			string[0][i]->nFrameIndex = i;
			string[0][i]->nFrameTotal = 4;
			string[0][i]->SetTexture();
		}

		// Life string
		rect = {
			0, DIGIT_SIZE * 3, TEXT_SIZE, DIGIT_SIZE
		};
		string[0][2] = new IMAGE(pTexStatus, &rect);
		string[0][2]->nFrameIndex = 2;
		string[0][2]->nFrameTotal = 4;
		string[0][2]->SetTexture();

		// score string
		rect = {
			0, 0, TEXT_SIZE, DIGIT_SIZE
		};
		string[0][3] = new IMAGE(pTexStatus, &rect);
		string[0][3]->nFrameIndex = 3;
		string[0][3]->nFrameTotal = 4;
		string[0][3]->SetTexture();

		// health bar
		nHealthAnimate[0] = pGame->players[0]->nHealth;
		mHealthEasing[0] = EASING(BEZIER_EASE_IN_OUT);
		rect = {
			DIGIT_SIZE, DIGIT_SIZE * 5, DIGIT_SIZE, DIGIT_SIZE * 10
		};
		health[0] = new IMAGE(NULL, &rect);
	}

	if (pGame->players[1]) {
		// score
		dwScoreAnimate[1] = pGame->players[1]->dwScore;
		mScoreEasing[1] = EASING(BEZIER_EASE_IN_OUT);
		for (int j = 0; j < UI_MAX_DIGITS; j++) {
			rect = {
				(LONG)(CL_WIDTH - DIGIT_SIZE - DIGIT_GAP * 6 + j * DIGIT_GAP), DIGIT_SIZE, DIGIT_SIZE, DIGIT_SIZE
			};

			score[1][j] = new DIGITIMAGE(0, &rect);
			score[1][j]->image->SetDiffuseColor(D3DXCOLOR(0.25f, 1.0f, 0.25f, 1.0f));
		}

		for (int j = 0; j < 2; j++) {
			// elements
			rect = {
				(LONG)(CL_WIDTH - ELEMENT_SIZE * 3), (LONG)(CL_HEIGHT - DIGIT_SIZE * (4 - j * 2)), ELEMENT_SIZE, ELEMENT_SIZE
			};
			elements[1][j] = new IMAGE(pTexElement, &rect);
			elements[1][j]->nFrameTotal = 5;
			elements[1][j]->SetTexture();

			// atk/def string
			rect = {
				(LONG)(CL_WIDTH - TEXT_SIZE), (LONG)(CL_HEIGHT - DIGIT_SIZE * (4 - j * 2)), TEXT_SIZE, DIGIT_SIZE
			};
			string[1][j] = new IMAGE(pTexStatus, &rect);
			string[1][j]->nFrameIndex = j;
			string[1][j]->nFrameTotal = 4;
			string[1][j]->SetTexture();
		}

		// Life string
		rect = {
			(LONG)(CL_WIDTH - TEXT_SIZE), DIGIT_SIZE * 3, TEXT_SIZE, DIGIT_SIZE
		};
		string[1][2] = new IMAGE(pTexStatus, &rect);
		string[1][2]->nFrameIndex = 2;
		string[1][2]->nFrameTotal = 4;
		string[1][2]->SetTexture();

		// score string
		rect = {
			(LONG)(CL_WIDTH - TEXT_SIZE), 0, TEXT_SIZE, DIGIT_SIZE
		};
		string[1][3] = new IMAGE(pTexStatus, &rect);
		string[1][3]->nFrameIndex = 3;
		string[1][3]->nFrameTotal = 4;
		string[1][3]->SetTexture();

		// health bar
		nHealthAnimate[1] = pGame->players[1]->nHealth;
		mHealthEasing[1] = EASING(BEZIER_EASE_IN_OUT);
		rect = {
			(LONG)(CL_WIDTH - DIGIT_SIZE * 2), DIGIT_SIZE * 5, DIGIT_SIZE, DIGIT_SIZE * 10
		};
		health[1] = new IMAGE(NULL, &rect);
	}

	for (int i = 0; i < UI_TIMER_DIGITS; i++) {
		rect = { (LONG)(CL_WIDTH / 2 - 40 + i * DIGIT_GAP), DIGIT_SIZE, DIGIT_SIZE, DIGIT_SIZE };
		timer[i] = new DIGITIMAGE(0, &rect);
	}

	for (int i = 0; i < GM_MAX_PLAYER; i++) {
		rect = { (LONG)(128 + i * CL_WIDTH / 2), CL_HEIGHT / 4, 384, 64 };
		result[i] = new IMAGE(pTexResult, &rect);
;	}
}

MAINGAMEUI::~MAINGAMEUI()
{
	SAFE_RELEASE(pTexElement);
	SAFE_RELEASE(pTexStatus);
	SAFE_RELEASE(pTexResult);

	for (int i = 0; i < UI_MAX_DIGITS; i++) {
		SAFE_DELETE(score[0][i]);
		SAFE_DELETE(score[1][i]);
	}
	for (int i = 0; i < GM_MAX_PLAYER; i++) {
		SAFE_DELETE(elements[i][0]);
		SAFE_DELETE(elements[i][1]);
		SAFE_DELETE(health[i]);
		SAFE_DELETE(string[i][0]);
		SAFE_DELETE(string[i][1]);
		SAFE_DELETE(string[i][2]);
		SAFE_DELETE(string[i][3]);

		SAFE_DELETE(result[i]);
	}
	for (int i = 0; i < UI_TIMER_DIGITS; i++) {
		SAFE_DELETE(timer[i]);
	}
}

void MAINGAMEUI::Update()
{
	for (int i = 0; i < GM_MAX_PLAYER; i++) {
		INT value = dwScoreAnimate[i];

		INT atk = pGame->players[i]->attackType;
		INT def = pGame->players[i]->defenseType;

		for (int j = 0; j < UI_MAX_DIGITS; j++) {
			score[i][UI_MAX_DIGITS - j - 1]->SetDigit(value % 10);
			value /= 10;
		}

		if (elements[i][0]->nFrameIndex != atk) {
			elements[i][0]->nFrameIndex = atk;
			elements[i][0]->SetTexture();
		}
		if (elements[i][1]->nFrameIndex != def) {
			elements[i][1]->nFrameIndex = def;
			elements[i][1]->SetTexture();
		}

		value = nHealthAnimate[i];

		D3DRECT rect = { i ? (LONG)(CL_WIDTH - DIGIT_SIZE * 2) : DIGIT_SIZE,
			(LONG)(DIGIT_SIZE * 5 + (100 - value) * DIGIT_SIZE / 10.0f),
			DIGIT_SIZE,
			(LONG)(value * DIGIT_SIZE / 10.0f)
		};
		D3DXCOLOR col;
		D3DXColorLerp(&col, &D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.75f), &D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.75f), FLOAT(value / 100.0f));

		health[i]->SetVertex(&rect);
		health[i]->SetDiffuseColor(col);

		health[i]->Update();

		if (pGame->bGameOver) {
			result[i]->nFrameIndex = pGame->winner[i];
			result[i]->nFrameTotal = GAME_RESULT_NUM;
			result[i]->SetTexture();
			result[i]->Update();
		}
	}

	INT time = pGame->nTimeRemain / 1000;
	for (int i = 0; i < UI_TIMER_DIGITS; i++) {
		timer[UI_TIMER_DIGITS - i - 1]->SetDigit(time % 10);
		time /= 10;
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
		elements[i][0]->Draw();
		elements[i][1]->Draw();

		string[i][0]->Draw();
		string[i][1]->Draw();
		string[i][2]->Draw();
		string[i][3]->Draw();

		health[i]->Draw();

		if (pGame->bGameOver) {
			result[i]->Draw();
		}
#if _DEBUG
		WCHAR str[256];
		swprintf_s(str, _T("Health %d"), nHealthAnimate[i]);
		D3D::ShowText(str, i * CL_WIDTH / 2, 0);
#endif
	}
	for (int i = 0; i < UI_TIMER_DIGITS; i++) {
		timer[i]->Draw();
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
		IMAGE_STRING,										// ファイルの名前
		&pTexStatus)))										// 読み込むメモリー
	{
		return E_FAIL;
	}

	return S_OK;
}

void MAINGAMEUI::UpdateHealth(PLAYER * player, INT * pNewHealth)
{
	int id = player->id;
	if (pNewHealth) {
		mHealthEasing[id].SetTimer(player->nHealth, *pNewHealth, 500);
		player->nHealth = *pNewHealth;
	}
	else {
		if (mHealthEasing[id].bEasing) {
			INT r = mHealthEasing[id].GetValue();
			nHealthAnimate[id] = r;
		}
		else
			nHealthAnimate[id] = player->nHealth;
	}
}

void MAINGAMEUI::UpdateScore(PLAYER* player, DWORD* pNewScore)
{
	int id = player->id;
	if (pNewScore) {
		mScoreEasing[id].SetTimer(player->dwScore, *pNewScore, 1000);
		player->dwScore = *pNewScore;
	}
	else {
		if (mScoreEasing[id].bEasing)
			dwScoreAnimate[id] = mScoreEasing[id].GetValue();
		else
			dwScoreAnimate[id] = player->dwScore;
	}
}

