/***********************************************
 * [game.h]
 * ゲーム処理
 * GP11A341 24 張馳騁
 ***********************************************/

#ifndef __GAME_H__
#define __GAME_H__

#include "main.h"
#include "core.h"
#include "camera.h"
#include "element.h"
#include "ground.h"
#include "map.h"
#include "light.h"
#include "player.h"
#include "sky.h"

typedef struct MAINGAME {
	//ELEMENT*				elements[MAX_ELEMENTS];
	MAP*					map;
	PLAYER*					player[MAX_PLAYER];
	LIGHT*					light;
	SKY*					sky;

	MAINGAME(INT nPlayer);
	~MAINGAME();

	void					Update(void);
	HRESULT					Draw(void);
} MAINGAME;

#endif // !__GAME_H__
