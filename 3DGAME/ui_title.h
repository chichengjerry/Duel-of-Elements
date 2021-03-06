//-----------------------------------------------
// [ui_title.h]
// タイトル画面UI
// 作者 Chicheng Zhang
//-----------------------------------------------
#ifndef __UI_TITLE_H__
#define __UI_TITLE_H__

#include "main.h"
#include "core.h"
#include "d3d.h"
#include "easing.h"
#include "element.h"
#include "image.h"
#include "title.h"

#define TITLE_TEX_MAIN		_T("data/TEXTURE/ui/title.png")
#define TITLE_TEX_MENU		_T("data/TEXTURE/ui/menu.png")

typedef struct TITLEUI {
	TITLE*						pGameTitle;

	static LPDIRECT3DTEXTURE9	pTexMenu;
	static LPDIRECT3DTEXTURE9	pTexTitle;

	IMAGE*						menu[TITLEMENU_COUNT];
	IMAGE*						title;

	TITLEUI(TITLE* title);
	~TITLEUI();

	void						UpdateLogoColor();
	void						Update();
	HRESULT						Draw();
} TITLEUI;

#endif // !__UI_TITLE_H__
