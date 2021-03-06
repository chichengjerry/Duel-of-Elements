//-----------------------------------------------
// [ui_title.cpp]
// タイトル画面UI
// 作者 Chicheng Zhang
//-----------------------------------------------
#include "ui_title.h"
#include "title.h"

//
// 静的メンバーの初期化
//
LPDIRECT3DTEXTURE9	TITLEUI::pTexMenu = NULL;
LPDIRECT3DTEXTURE9	TITLEUI::pTexTitle = NULL;

//
// タイトル画面の初期化
//
TITLEUI::TITLEUI(TITLE * pTitle)
{
	pGameTitle = pTitle;

	D3D::LoadTexture(&pTexTitle, TITLE_TEX_MAIN);
	D3D::LoadTexture(&pTexMenu, TITLE_TEX_MENU);
	D3DRECT rect;

	for (int i = 0; i < TITLEMENU_COUNT; i++) {
		rect = { CL_WIDTH / 2 - 160, CL_HEIGHT / 2 + 96 * i, 384, 64 };
		menu[i] = new IMAGE(pTexMenu, &rect);
		menu[i]->nFrameIndex = i;
		menu[i]->nFrameTotal = TITLEMENU_COUNT;
		menu[i]->SetTexture();
	}
	rect = { CL_WIDTH / 2 - 640, CL_HEIGHT / 8, 1280, 320 };
	title = new IMAGE(pTexTitle, &rect);
}

//
// タイトル画面のリソースの解放
//
TITLEUI::~TITLEUI()
{
	SAFE_RELEASE(pTexTitle);
	SAFE_RELEASE(pTexMenu);

	SAFE_DELETE(title);
	for (int i = 0; i < TITLEMENU_COUNT; i++) {
		SAFE_DELETE(menu[i]);
	}
}

//
// タイトルロゴに動的な色を付ける
//
void TITLEUI::UpdateLogoColor()
{
	VERTEX_2D* pTempVtx;
	DWORD t = timeGetTime();
	FLOAT p = (t % 4000) / 1000.0f;
	INT n = (INT)floorf(p);
	p -= n;

	title->pVtx->Lock(0, sizeof(VERTEX_2D) * 4, (void**)&pTempVtx, 0);
	
	// 疑似的反時計回りで移っている
	D3DXCOLOR col;
	int seq[4] = { 0, 1, 3, 2 };
	for (int i = 0; i < 4; i++) {
		D3DXColorLerp(&col, &ElementColor[seq[(n + i) % 4] + 1], &ElementColor[seq[(n + i + 1) % 4] + 1], p);
		pTempVtx[seq[i]].dif = col;
	}
	
	title->pVtx->Unlock();

	title->Update();
}

//
// タイトルUIを更新する。
//
void TITLEUI::Update()
{
	// メニュー選択
	for (int i = 0; i < TITLEMENU_COUNT; i++) {
		if (pGameTitle->menuSelected == i) {
			menu[i]->SetDiffuseColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		}
		else {
			menu[i]->SetDiffuseColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
		menu[i]->Update();
	}

	UpdateLogoColor();
}

//
// タイトルUIを描画する。
//
HRESULT TITLEUI::Draw()
{
	for (int i = 0; i < TITLEMENU_COUNT; i++) {
		menu[i]->Draw();
	}
	title->Draw();

	return S_OK;
}
