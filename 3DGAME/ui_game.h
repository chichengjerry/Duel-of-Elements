#ifndef __UI_GAME_H__
#define __UI_GAME_H__

#include "main.h"
#include "core.h"
#include "image.h"
#include "gamemain.h"
#include "game.h"

#define UI_MAX_DIGITS			6

typedef struct MAINGAMEUI {
	static LPDIRECT3DTEXTURE9	pTexElement;
	static LPDIRECT3DTEXTURE9	pTexStatus;

	MAINGAME*					pGame;
	DIGITIMAGE*					score[GM_MAX_PLAYER][UI_MAX_DIGITS];

	MAINGAMEUI(MAINGAME* game);
	~MAINGAMEUI();

	void						Update();
	HRESULT						Draw();

	HRESULT						LoadTexture();
	void						SetScore();


} MAINGAMEUI;

#endif // !__UI_GAME_H__
