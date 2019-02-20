#include "ui_tutorial.h"
#include "input.h"
#include "sound.h"

#define TITLE_TEX_TUTOR	_T("data/TEXTURE/ui/tutorial_%d.png")

LPDIRECT3DTEXTURE9	TUTORIALUI::pTexTutor[] = { NULL };

TUTORIALUI::TUTORIALUI(TITLE * title)
{
	nCurrentPage = 0;
	D3DRECT rect = { 0, 0, CL_WIDTH, CL_HEIGHT };

	for (int i = 0; i < TUTORIAL_PAGE; i++) {
		WCHAR str[256];
		swprintf_s(str, TITLE_TEX_TUTOR, i + 1);
		D3D::LoadTexture(&pTexTutor[i], str);

		tutorial[i] = new IMAGE(pTexTutor[i], &rect);
	}
}

TUTORIALUI::~TUTORIALUI()
{
	for (int i = 0; i < TUTORIAL_PAGE; i++) {
		SAFE_DELETE(tutorial[i]);
	}
}

void TUTORIALUI::Update()
{
	if (DINPUT::KeyTriggered(DIK_RIGHT) || DINPUT::ButtonTriggered(BUTTON_RIGHT)) {
		nCurrentPage = (nCurrentPage + 1) % TUTORIAL_PAGE;
		DSOUND::Play(SOUND_CLICK);
	}
	if (DINPUT::KeyTriggered(DIK_LEFT) || DINPUT::ButtonTriggered(BUTTON_LEFT)) {
		nCurrentPage = (nCurrentPage + TUTORIAL_PAGE - 1) % TUTORIAL_PAGE;
		DSOUND::Play(SOUND_CLICK);
	}

	for (int i = 0; i < TUTORIAL_PAGE; i++) {
		tutorial[i]->Update();
	}
}

void TUTORIALUI::Draw()
{
	tutorial[nCurrentPage]->Draw();
}
