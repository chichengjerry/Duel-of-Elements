#ifndef __UI_TUTORIAL_H__
#define __UI_TUTORIAL_H__

#include "main.h"
#include "core.h"
#include "d3d.h"
#include "image.h"
#include "title.h"

#define TUTORIAL_PAGE			2

typedef struct TUTORIALUI {
	static LPDIRECT3DTEXTURE9	pTexTutor[TUTORIAL_PAGE];
	IMAGE*						tutorial[TUTORIAL_PAGE];
	TITLE*						pTitle;
	INT							nCurrentPage;

	TUTORIALUI(TITLE* title);
	~TUTORIALUI();

	void						Update();
	void						Draw();
} TUTORIALUI;

#endif // !__UI_TUTORIAL_H__
