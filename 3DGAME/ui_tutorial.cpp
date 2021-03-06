//-----------------------------------------------
// [ui_tutorial.cpp]
// 操作説明UI
// 作者 Chicheng Zhang
//-----------------------------------------------
#include "ui_tutorial.h"

//
// 静的メンバーの初期化
//
LPDIRECT3DTEXTURE9	TUTORIALUI::pTexTutor[] = { NULL };

//
// 操作説明の初期化
//
TUTORIALUI::TUTORIALUI(TITLE * title)
{
	nCurrentPage = 0;
	D3DRECT rect = { 0, 0, CL_WIDTH, CL_HEIGHT };

	for (int i = 0; i < TUTORIAL_PAGES; i++) {
		WCHAR str[256];
		swprintf_s(str, TITLE_TEX_TUTOR, i + 1);
		D3D::LoadTexture(&pTexTutor[i], str);

		tutorial[i] = new IMAGE(pTexTutor[i], &rect);
	}
}

//
// 操作説明のリソースの解放
//
TUTORIALUI::~TUTORIALUI()
{
	for (int i = 0; i < TUTORIAL_PAGES; i++) {
		SAFE_DELETE(tutorial[i]);
	}
}

//
// 操作説明の更新
//
void TUTORIALUI::Update()
{
	// ページの切り替え
	if (DINPUT::KeyTriggered(DIK_RIGHT) || DINPUT::ButtonTriggered(BUTTON_RIGHT)) {
		nCurrentPage = (nCurrentPage + 1) % TUTORIAL_PAGES;
		DSOUND::Play(SOUND_CLICK);
	}
	if (DINPUT::KeyTriggered(DIK_LEFT) || DINPUT::ButtonTriggered(BUTTON_LEFT)) {
		nCurrentPage = (nCurrentPage + TUTORIAL_PAGES - 1) % TUTORIAL_PAGES;
		DSOUND::Play(SOUND_CLICK);
	}

	for (int i = 0; i < TUTORIAL_PAGES; i++) {
		tutorial[i]->Update();
	}
}

//
// 操作説明の描画。
// 現在は選んだページを描画するだけ。
//
void TUTORIALUI::Draw()
{
	tutorial[nCurrentPage]->Draw();
}
