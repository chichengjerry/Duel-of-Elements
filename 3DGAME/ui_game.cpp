//-----------------------------------------------
// [ui_game.cpp]
// メインゲームのUI
// 作者 Chicheng Zhang
//-----------------------------------------------
#include "ui_game.h"
#include "game.h"
#include "player.h"

//
// 静的メンバーの初期化
//

LPDIRECT3DTEXTURE9	MAINGAMEUI::pTexElement = NULL;
LPDIRECT3DTEXTURE9	MAINGAMEUI::pTexStatus = NULL;
LPDIRECT3DTEXTURE9	MAINGAMEUI::pTexResult = NULL;

//
// メインゲームUIの初期化
//
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
		nScore[0] = pGame->players[0]->nScore;
		mScoreEasing[0] = EASING(BEZIER_EASE_IN_OUT);
		for (int j = 0; j < UI_MAX_DIGITS; j++) {
			rect = {
				(LONG)(UI_DIGIT_GAP + j * UI_DIGIT_GAP), UI_DIGIT_SIZE, UI_DIGIT_SIZE, UI_DIGIT_SIZE
			};

			score[0][j] = new DIGITIMAGE(0, &rect);
			score[0][j]->image->SetDiffuseColor(D3DXCOLOR(1.0f, 0.25f, 0.25f, 1.0f));
		}

		for (int i = 0; i < 2; i++) {
			// elements
			rect = {
				(LONG)(UI_ELEMENT_SIZE * 2), (LONG)(CL_HEIGHT - UI_DIGIT_SIZE * (4 - i * 2)), UI_ELEMENT_SIZE, UI_ELEMENT_SIZE
			};
			elements[0][i] = new IMAGE(pTexElement, &rect);
			elements[0][i]->nFrameTotal = 5;
			elements[0][i]->SetTexture();

			// atk/def string
			rect = {
				0, (LONG)(CL_HEIGHT - UI_DIGIT_SIZE * (4 - i * 2)), UI_TEXT_SIZE, UI_DIGIT_SIZE
			};
			string[0][i] = new IMAGE(pTexStatus, &rect);
			string[0][i]->nFrameIndex = i;
			string[0][i]->nFrameTotal = 4;
			string[0][i]->SetTexture();
		}

		// Life string
		rect = {
			0, UI_DIGIT_SIZE * 3, UI_TEXT_SIZE, UI_DIGIT_SIZE
		};
		string[0][2] = new IMAGE(pTexStatus, &rect);
		string[0][2]->nFrameIndex = 2;
		string[0][2]->nFrameTotal = 4;
		string[0][2]->SetTexture();

		// score string
		rect = {
			0, 0, UI_TEXT_SIZE, UI_DIGIT_SIZE
		};
		string[0][3] = new IMAGE(pTexStatus, &rect);
		string[0][3]->nFrameIndex = 3;
		string[0][3]->nFrameTotal = 4;
		string[0][3]->SetTexture();

		// health bar
		nHealth[0] = pGame->players[0]->nHealth;
		mHealthEasing[0] = EASING(BEZIER_EASE_IN_OUT);
		rect = {
			UI_DIGIT_SIZE, UI_DIGIT_SIZE * 5, UI_DIGIT_SIZE, UI_DIGIT_SIZE * 10
		};
		health[0] = new IMAGE(NULL, &rect);
	}

	if (pGame->players[1]) {
		// score
		nScore[1] = pGame->players[1]->nScore;
		mScoreEasing[1] = EASING(BEZIER_EASE_IN_OUT);
		for (int j = 0; j < UI_MAX_DIGITS; j++) {
			rect = {
				(LONG)(CL_WIDTH - UI_DIGIT_SIZE - UI_DIGIT_GAP * 6 + j * UI_DIGIT_GAP), UI_DIGIT_SIZE, UI_DIGIT_SIZE, UI_DIGIT_SIZE
			};

			score[1][j] = new DIGITIMAGE(0, &rect);
			score[1][j]->image->SetDiffuseColor(D3DXCOLOR(0.25f, 1.0f, 0.25f, 1.0f));
		}

		for (int j = 0; j < 2; j++) {
			// elements
			rect = {
				(LONG)(CL_WIDTH - UI_ELEMENT_SIZE * 3), (LONG)(CL_HEIGHT - UI_DIGIT_SIZE * (4 - j * 2)), UI_ELEMENT_SIZE, UI_ELEMENT_SIZE
			};
			elements[1][j] = new IMAGE(pTexElement, &rect);
			elements[1][j]->nFrameTotal = 5;
			elements[1][j]->SetTexture();

			// atk/def string
			rect = {
				(LONG)(CL_WIDTH - UI_TEXT_SIZE), (LONG)(CL_HEIGHT - UI_DIGIT_SIZE * (4 - j * 2)), UI_TEXT_SIZE, UI_DIGIT_SIZE
			};
			string[1][j] = new IMAGE(pTexStatus, &rect);
			string[1][j]->nFrameIndex = j;
			string[1][j]->nFrameTotal = 4;
			string[1][j]->SetTexture();
		}

		// Life string
		rect = {
			(LONG)(CL_WIDTH - UI_TEXT_SIZE), UI_DIGIT_SIZE * 3, UI_TEXT_SIZE, UI_DIGIT_SIZE
		};
		string[1][2] = new IMAGE(pTexStatus, &rect);
		string[1][2]->nFrameIndex = 2;
		string[1][2]->nFrameTotal = 4;
		string[1][2]->SetTexture();

		// score string
		rect = {
			(LONG)(CL_WIDTH - UI_TEXT_SIZE), 0, UI_TEXT_SIZE, UI_DIGIT_SIZE
		};
		string[1][3] = new IMAGE(pTexStatus, &rect);
		string[1][3]->nFrameIndex = 3;
		string[1][3]->nFrameTotal = 4;
		string[1][3]->SetTexture();

		// health bar
		nHealth[1] = pGame->players[1]->nHealth;
		mHealthEasing[1] = EASING(BEZIER_EASE_IN_OUT);
		rect = {
			(LONG)(CL_WIDTH - UI_DIGIT_SIZE * 2), UI_DIGIT_SIZE * 5, UI_DIGIT_SIZE, UI_DIGIT_SIZE * 10
		};
		health[1] = new IMAGE(NULL, &rect);
	}

	for (int i = 0; i < UI_TIMER_DIGITS; i++) {
		rect = { (LONG)(CL_WIDTH / 2 - 40 + i * UI_DIGIT_GAP), UI_DIGIT_SIZE, UI_DIGIT_SIZE, UI_DIGIT_SIZE };
		timer[i] = new DIGITIMAGE(0, &rect);
	}

	for (int i = 0; i < GM_MAX_PLAYER; i++) {
		rect = { (LONG)(128 + i * CL_WIDTH / 2), CL_HEIGHT / 4, 384, 64 };
		result[i] = new IMAGE(pTexResult, &rect);
;	}
}

//
// メインゲームUIで使ったリソースの解放
//
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

//
// メインゲームUIを更新する。
//
void MAINGAMEUI::Update()
{
	//プレイヤーごとに情報を更新する。
	for (int i = 0; i < GM_MAX_PLAYER; i++) {
		INT value = nScore[i];

		INT atk = pGame->players[i]->attackType;
		INT def = pGame->players[i]->defenseType;

		// スコア用スピリット
		for (int j = 0; j < UI_MAX_DIGITS; j++) {
			score[i][UI_MAX_DIGITS - j - 1]->SetDigit(value % 10);
			value /= 10;
		}
		// エレメントのスピリット
		if (elements[i][0]->nFrameIndex != atk) {
			elements[i][0]->nFrameIndex = atk;
			elements[i][0]->SetTexture();
		}
		if (elements[i][1]->nFrameIndex != def) {
			elements[i][1]->nFrameIndex = def;
			elements[i][1]->SetTexture();
		}

		// 生命値バーのサイズと位置
		value = nHealth[i];

		D3DRECT rect = { i ? (LONG)(CL_WIDTH - UI_DIGIT_SIZE * 2) : UI_DIGIT_SIZE,
			(LONG)(UI_DIGIT_SIZE * 5 + (100 - value) * UI_DIGIT_SIZE / 10.0f),
			UI_DIGIT_SIZE,
			(LONG)(value * UI_DIGIT_SIZE / 10.0f)
		};
		D3DXCOLOR col;
		D3DXColorLerp(&col, &D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.75f), &D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.75f), FLOAT(value / 100.0f));

		health[i]->SetVertex(&rect);
		health[i]->SetDiffuseColor(col);
		health[i]->Update();

		// ゲーム終了なら結果画面を表示する。
		if (pGame->bGameOver) {
			result[i]->nFrameIndex = pGame->winner[i];
			result[i]->nFrameTotal = GAME_RESULT_NUM;
			result[i]->SetTexture();
			result[i]->Update();
		}
	}

	// 残り時間のスピリット
	INT time = pGame->nTimeRemain / 1000;
	for (int i = 0; i < UI_TIMER_DIGITS; i++) {
		timer[UI_TIMER_DIGITS - i - 1]->SetDigit(time % 10);
		time /= 10;
	}
}

//
// メインゲームUIを描画する。
//
HRESULT MAINGAMEUI::Draw()
{
	//プレイヤーごとに情報を描画する。
	for (int i = 0; i < GM_MAX_PLAYER; i++) {
		// スコア描画
		for (int j = 0; j < UI_MAX_DIGITS; j++) {
			score[i][j]->Draw();
		}
		// エレメント状態描画
		elements[i][0]->Draw();
		elements[i][1]->Draw();

		// テキスト描画
		string[i][0]->Draw();
		string[i][1]->Draw();
		string[i][2]->Draw();
		string[i][3]->Draw();

		// 生命値バー描画
		health[i]->Draw();
#if _DEBUG
		WCHAR str[256];
		swprintf_s(str, _T("Health %d"), nHealth[i]);
		D3D::ShowText(str, i * CL_WIDTH / 2, 0);
#endif

		// 結果画面描画
		if (pGame->bGameOver) {
			result[i]->Draw();
		}
	}

	// 残り時間描画
	for (int i = 0; i < UI_TIMER_DIGITS; i++) {
		timer[i]->Draw();
	}

	return S_OK;
}

//
// メインゲームUIで使うテクスチャをロードする。
//
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

//
// メインゲームUIでのプレイヤーの生命値を更新する。
// 新しい値が入れるたびに、動画で差異の部分を変更する。
//
void MAINGAMEUI::UpdateHealth(PLAYER * player, INT * pNewHealth)
{
	int id = player->id;
	if (pNewHealth) {
		mHealthEasing[id].SetTimer(player->nHealth, *pNewHealth, 500);
	}
	else {
		if (mHealthEasing[id].bEasing) nHealth[id] = mHealthEasing[id].GetValue();
		else nHealth[id] = player->nHealth;
	}
}

//
// メインゲームUIでのプレイヤーのスコアを更新する。
// 新しい値が入れるたびに、動画で差異の部分を変更する。
//
void MAINGAMEUI::UpdateScore(PLAYER* player, DWORD* pNewScore)
{
	int id = player->id;
	if (pNewScore) {
		mScoreEasing[id].SetTimer(player->nScore, *pNewScore, 1000);
	}
	else {
		if (mScoreEasing[id].bEasing) nScore[id] = mScoreEasing[id].GetValue();
		else nScore[id] = player->nScore;
	}
}

