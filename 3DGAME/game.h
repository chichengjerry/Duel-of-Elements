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

struct GAMESTAGE;

struct MAINGAME {
	GAMESTAGE*				pState;
	MAINGAMEUI*				gameUI;
	BARRIER*				barrier = NULL;
	BULLET*					bullets[GM_MAX_BULLETS] = { NULL };
	PLAYER*					players[GM_MAX_PLAYER] = { NULL };
	ELEMENT*				elements[GM_MAX_ELEMENTS] = { NULL };
	SKY*					sky;
	TERRAIN*				terrain;

	MAINGAME(GAMESTAGE* state, INT nPlayer);
	~MAINGAME();

	void					Update(void);
	HRESULT					Draw(void);
};

#endif // !__GAME_H__
