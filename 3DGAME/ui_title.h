#ifndef __UI_TITLE_H__
#define __UI_TITLE_H__

#include "main.h"
#include "core.h"
#include "d3d.h"
#include "easing.h"
#include "element.h"
#include "image.h"
#include "title.h"

typedef struct TITLEUI {
	TITLE*						pGameTitle;

	static LPDIRECT3DTEXTURE9	pTexMenu;
	static LPDIRECT3DTEXTURE9	pTexTitle;

	IMAGE*						menu[TITLEMENU_COUNT];
	IMAGE*						title;

	TITLEUI(TITLE* title);
	~TITLEUI();

	void						SetTitleColor();
	void						Update();
	HRESULT						Draw();
} TITLEUI;

#endif // !__UI_TITLE_H__
