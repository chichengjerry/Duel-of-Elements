//-----------------------------------------------
// [game.h]
// ゲーム処理
// 作者 Chicheng Zhang
//-----------------------------------------------
#ifndef __GAME_H__
#define __GAME_H__

#include "main.h"
#include "core.h"
#include "gamemain.h"

enum GAME_RESULT {
	GAME_RESULT_WIN,
	GAME_RESULT_LOSE,
	GAME_RESULT_DRAW,

	GAME_RESULT_NUM
};

struct GAMESTAGE;

struct MAINGAME {
	GAMESTAGE*				pState;
	MAINGAMEUI*				gameUI;
	BARRIER*				barrier = NULL;
	BULLET*					bullets[GM_MAX_BULLETS] = { NULL };
	EXPLOSION*				explosion[GM_MAX_BULLETS] = { NULL };
	PLAYER*					players[GM_MAX_PLAYER] = { NULL };
	ELEMENT*				elements[GM_MAX_ELEMENTS] = { NULL };
	SKY*					sky;
	TERRAIN*				terrain;
	INT						winner[GM_MAX_PLAYER];
	BOOL					bGameOver;
	INT						nTimeRemain;
	INT						nTimeLast;

	MAINGAME(GAMESTAGE* state, INT nPlayer);
	~MAINGAME();

	void					Update(void);
	HRESULT					Draw(void);

	void					GameOver(int playerId, BOOL win);
	void					PlayerCollisionCheck();
};

#endif // !__GAME_H__
