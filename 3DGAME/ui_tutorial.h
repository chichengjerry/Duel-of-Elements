//-----------------------------------------------
// [ui_tutorial.h]
// 操作説明UI
// 作者 Chicheng Zhang
//-----------------------------------------------
#ifndef __UI_TUTORIAL_H__
#define __UI_TUTORIAL_H__

#include "main.h"
#include "core.h"
#include "d3d.h"
#include "image.h"
#include "title.h"
#include "sound.h"

#define TUTORIAL_PAGES			2

#define TITLE_TEX_TUTOR			_T("data/TEXTURE/ui/tutorial_%d.png")

typedef struct TUTORIALUI {
	static LPDIRECT3DTEXTURE9	pTexTutor[TUTORIAL_PAGES];
	IMAGE*						tutorial[TUTORIAL_PAGES];
	TITLE*						pTitle;
	INT							nCurrentPage;

	TUTORIALUI(TITLE* title);
	~TUTORIALUI();

	void						Update();
	void						Draw();
} TUTORIALUI;

#endif // !__UI_TUTORIAL_H__
