/***********************************************
 * [camera.h]
 * •´•·•ÈÑI¿Ì
 * GP11A341 24 èàÒYÚG
 ***********************************************/

#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "main.h"
#include "core.h"
#include "d3d.h"
#include "player.h"

typedef struct CAMERA {
	PLAYER*					p_player;
	D3DXVECTOR3				v_pos;
	D3DXVECTOR3				v_tar;
	D3DXVECTOR3				v_up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DVIEWPORT9			viewport;
	D3DXMATRIX				m_view;
	D3DXMATRIX				m_proj;

	CAMERA(D3DXVECTOR3 pos, PLAYER* player);
	~CAMERA();

	D3DXMATRIX*				GetView(void);
	void					SetCamera(void);
	void					Update(void);
} CAMERA;

typedef struct CAMERAS {
	MAINGAME*				p_game;
	CAMERA*					camera[MAX_PLAYER];
	DWORD					current_camera;

	CAMERAS(MAINGAME* pGame);
} CAMERAS;

#endif // !__CAMERA_H__
