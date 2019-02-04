/***********************************************
 * [game.h]
 * ゲーム処理
 * GP11A341 24 張馳騁
 ***********************************************/

#ifndef __GAME_H__
#define __GAME_H__

#include "main.h"
#include "core.h"
#include "gamemain.h"

struct MAINGAME {
	PLAYER*					player[GM_MAX_PLAYER];
	ELEMENT*				elements[GM_MAX_ELEMENTS];
	LIGHT*					light;
	SKY*					sky;
	TERRAIN*				terrain;

	MAINGAME(INT nPlayer);
	~MAINGAME();

	void					Update(void);
	HRESULT					Draw(void);
};

#endif // !__GAME_H__
