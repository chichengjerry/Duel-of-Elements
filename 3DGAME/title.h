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

typedef enum TITLEMENU {
	TITLEMENU_VERSUS,
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

	TITLE(GAMESTAGE*);
	~TITLE();

	void				Update();
	HRESULT				Draw();

	void				UpdateCamera();
} TITLE;

#endif // !__TITLE_H__
