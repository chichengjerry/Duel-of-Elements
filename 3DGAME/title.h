//-----------------------------------------------
// [title.h]
// タイトル処理
// 作者 Chicheng Zhang
//-----------------------------------------------
#ifndef __TITLE_H__
#define __TITLE_H__

#include "main.h"
#include "core.h"
#include "d3d.h"
#include "camera.h"
#include "sky.h"
#include "terrain.h"

struct GAMESTAGE;
struct TITLEUI;
struct TUTORIALUI;

typedef enum TITLEMENU {
	TITLEMENU_VERSUS,
	TITLEMENU_TUTORIAL,
	TITLEMENU_EXIT,

	TITLEMENU_COUNT
} TITLEMENU;

typedef struct TITLE{
	GAMESTAGE*			pState;
	INT					menuSelected;

	D3DXVECTOR3			rot;
	CAMERA*				camera;
	TERRAIN*			terrain;
	SKY*				sky;
	TITLEUI*			titleUI;
	TUTORIALUI*			tutorialUI;

	TITLE(GAMESTAGE*);
	~TITLE();

	void				Update();
	HRESULT				Draw();

	void				UpdateCamera();
} TITLE;

#endif // !__TITLE_H__
