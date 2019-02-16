#include "ui_title.h"

#define TITLE_TEX_MAIN	_T("data/TEXTURE/ui/title.png")
#define TITLE_TEX_MENU	_T("data/TEXTURE/ui/menu.png")

LPDIRECT3DTEXTURE9	TITLEUI::pTexMenu = NULL;
LPDIRECT3DTEXTURE9	TITLEUI::pTexTitle = NULL;

TITLEUI::TITLEUI(TITLE * pTitle)
{
	pGameTitle = pTitle;

	D3D::LoadTexture(&pTexTitle, TITLE_TEX_MAIN);
	D3D::LoadTexture(&pTexMenu, TITLE_TEX_MENU);

	for (int i = 0; i < TITLEMENU_COUNT; i++) {
		D3DRECT rect = { CL_WIDTH / 2 - 160, CL_HEIGHT / 2 + 96 * i, 320, 64 };
		menu[i] = new IMAGE(pTexMenu, &rect);
		menu[i]->nFrameIndex = i;
		menu[i]->nFrameTotal = TITLEMENU_COUNT;
		menu[i]->SetTexture();
	}
	D3DRECT rect = { CL_WIDTH / 2 - 320, CL_HEIGHT / 8, 640, 160 };
	title = new IMAGE(pTexTitle, &rect);
}

TITLEUI::~TITLEUI()
{
	SAFE_RELEASE(pTexTitle);
	SAFE_RELEASE(pTexMenu);

	for (int i = 0; i < TITLEMENU_COUNT; i++) {
		SAFE_DELETE(menu[i]);
	}
	SAFE_DELETE(title);
}

void TITLEUI::Update()
{
	for (int i = 0; i < TITLEMENU_COUNT; i++) {
		if (pGameTitle->menuSelected == i) {
			menu[i]->SetDiffuseColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		}
		else {
			menu[i]->SetDiffuseColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
		menu[i]->Update();
	}
	title->Update();
}

HRESULT TITLEUI::Draw()
{
	for (int i = 0; i < TITLEMENU_COUNT; i++) {
		menu[i]->Draw();
	}
	title->Draw();

	return S_OK;
}
