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
	const FLOAT ELEMENT_SIZE = 48.0f;

	LPDIRECT3DDEVICE9 pDevice = D3D::GetDevice();

	if (!pTexElement) {
		if (FAILED(D3DXCreateTextureFromFile(pDevice,			// �f�o�C�X�ւ̃|�C���^
			IMAGE_ELEMENT,										// �t�@�C���̖��O
			&pTexElement)))										// �ǂݍ��ރ������[
		{
			return;
		}
	}

	if (pGame->players[0]) {
		D3DXVECTOR3 vtx[4];

		for (int j = 0; j < UI_MAX_DIGITS; j++) {
			
			// score
			vtx[0] = D3DXVECTOR3(DIGIT_GAP + j * DIGIT_GAP, DIGIT_GAP, 0.0f);
			vtx[1] = D3DXVECTOR3(DIGIT_GAP + j * DIGIT_GAP + DIGIT_SIZE, DIGIT_GAP, 0.0f);
			vtx[2] = D3DXVECTOR3(DIGIT_GAP + j * DIGIT_GAP, DIGIT_GAP + DIGIT_SIZE, 0.0f);
			vtx[3] = D3DXVECTOR3(DIGIT_GAP + j * DIGIT_GAP + DIGIT_SIZE, DIGIT_GAP + DIGIT_SIZE, 0.0f);

			score[0][j] = new DIGITIMAGE(0, vtx);
			score[0][j]->image->SetDiffuseColor(&D3DXCOLOR(1.0f, 0.25f, 0.25f, 1.0f));
		}
		for (int j = 0; j < 2; j++) {
			// elements
			vtx[0] = D3DXVECTOR3(ELEMENT_SIZE * 2, CL_HEIGHT - ELEMENT_SIZE * (4 - j * 2), 0.0f);
			vtx[1] = D3DXVECTOR3(ELEMENT_SIZE * 3, CL_HEIGHT - ELEMENT_SIZE * (4 - j * 2), 0.0f);
			vtx[2] = D3DXVECTOR3(ELEMENT_SIZE * 2, CL_HEIGHT - ELEMENT_SIZE * (3 - j * 2), 0.0f);
			vtx[3] = D3DXVECTOR3(ELEMENT_SIZE * 3, CL_HEIGHT - ELEMENT_SIZE * (3 - j * 2), 0.0f);
			elements[0][j] = new IMAGE(pTexElement, vtx);
			elements[0][j]->nFrameTotal = 5;
			elements[0][j]->SetTexture();
		}
	}

	if (pGame->players[1]) {
		D3DXVECTOR3 vtx[4];

		for (int j = 0; j < UI_MAX_DIGITS; j++) {
			// score
			D3DXVECTOR3 offset = D3DXVECTOR3(CL_WIDTH - DIGIT_SIZE - DIGIT_GAP * 6, 0.0f, 0.0f);
			vtx[0] = offset + D3DXVECTOR3(j * DIGIT_GAP, DIGIT_GAP, 0.0f);
			vtx[1] = offset + D3DXVECTOR3(j * DIGIT_GAP + DIGIT_SIZE, DIGIT_GAP, 0.0f);
			vtx[2] = offset + D3DXVECTOR3(j * DIGIT_GAP, DIGIT_GAP + DIGIT_SIZE, 0.0f);
			vtx[3] = offset + D3DXVECTOR3(j * DIGIT_GAP + DIGIT_SIZE, DIGIT_GAP + DIGIT_SIZE, 0.0f);
			score[1][j] = new DIGITIMAGE(0, vtx);
			score[1][j]->image->SetDiffuseColor(&D3DXCOLOR(0.25f, 1.0f, 0.25f, 1.0f));

		}
		for (int j = 0; j < 2; j++) {
			// elements
			vtx[0] = D3DXVECTOR3(CL_WIDTH - ELEMENT_SIZE * 3, CL_HEIGHT - ELEMENT_SIZE * (4 - j * 2), 0.0f);
			vtx[1] = D3DXVECTOR3(CL_WIDTH - ELEMENT_SIZE * 2, CL_HEIGHT - ELEMENT_SIZE * (4 - j * 2), 0.0f);
			vtx[2] = D3DXVECTOR3(CL_WIDTH - ELEMENT_SIZE * 3, CL_HEIGHT - ELEMENT_SIZE * (3 - j * 2), 0.0f);
			vtx[3] = D3DXVECTOR3(CL_WIDTH - ELEMENT_SIZE * 2, CL_HEIGHT - ELEMENT_SIZE * (3 - j * 2), 0.0f);
			elements[1][j] = new IMAGE(pTexElement, vtx);
			elements[1][j]->nFrameTotal = 5;
			elements[1][j]->SetTexture();
		}
	}
	
}

MAINGAMEUI::~MAINGAMEUI()
{
	SAFE_RELEASE(pTexElement);
	SAFE_RELEASE(pTexStatus);
	SAFE_RELEASE(pTexStatus);

	for (int i = 0; i < UI_MAX_DIGITS; i++) {
		SAFE_DELETE(score[0][i]);
		SAFE_DELETE(score[1][i]);
	}
	for (int i = 0; i < GM_MAX_PLAYER; i++) {
		SAFE_DELETE(elements[i][0]);
		SAFE_DELETE(elements[i][1]);
	}
}

void MAINGAMEUI::Update()
{
	for (int i = 0; i < GM_MAX_PLAYER; i++) {
		INT value = pGame->players[i]->dwScoreAnimate;

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
	}

	pDevice->Present(NULL, NULL, NULL, NULL);

	pDevice->EndScene();

	pDevice->BeginScene();

	return S_OK;
}

HRESULT MAINGAMEUI::LoadTexture()
{
	LPDIRECT3DDEVICE9 pDevice = D3D::GetDevice();

	if (FAILED(D3DXCreateTextureFromFile(pDevice,			// �f�o�C�X�ւ̃|�C���^
		IMAGE_ELEMENT,										// �t�@�C���̖��O
		&pTexElement)))										// �ǂݍ��ރ������[
	{
		return E_FAIL;
	}
	if (FAILED(D3DXCreateTextureFromFile(pDevice,			// �f�o�C�X�ւ̃|�C���^
		IMAGE_STATUS,										// �t�@�C���̖��O
		&pTexStatus)))										// �ǂݍ��ރ������[
	{
		return E_FAIL;
	}

	return S_OK;
}
