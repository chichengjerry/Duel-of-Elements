//-----------------------------------------------
// [ui_game.h]
// メインゲームのUI
// 作者 Chicheng Zhang
//-----------------------------------------------
#ifndef __UI_GAME_H__
#define __UI_GAME_H__

#include "main.h"
#include "core.h"
#include "image.h"
#include "easing.h"
#include "gamemain.h"
#include "ui_digits.h"

#define IMAGE_ELEMENT		_T("data/TEXTURE/ui/elements.png")
#define IMAGE_STRING		_T("data/TEXTURE/ui/string.png")
#define IMAGE_RESULT		_T("data/TEXTURE/ui/result.png")

#define UI_MAX_DIGITS		6
#define UI_TIMER_DIGITS		3

#define UI_DIGIT_GAP		24
#define UI_DIGIT_SIZE		32
#define UI_ELEMENT_SIZE		48
#define UI_TEXT_SIZE		96

typedef struct MAINGAMEUI {
	static LPDIRECT3DTEXTURE9	pTexElement;
	static LPDIRECT3DTEXTURE9	pTexStatus;
	static LPDIRECT3DTEXTURE9	pTexResult;

	MAINGAME*					pGame;
	IMAGE*						elements[GM_MAX_PLAYER][2];
	IMAGE*						string[GM_MAX_PLAYER][4];

	DIGITIMAGE*					score[GM_MAX_PLAYER][UI_MAX_DIGITS];
	DWORD						nScore[GM_MAX_PLAYER];
	EASING						mScoreEasing[GM_MAX_PLAYER];
	
	IMAGE*						health[GM_MAX_PLAYER];
	INT							nHealth[GM_MAX_PLAYER];
	EASING						mHealthEasing[GM_MAX_PLAYER];

	IMAGE*						result[GM_MAX_PLAYER];
	DIGITIMAGE*					timer[UI_TIMER_DIGITS];

	MAINGAMEUI(MAINGAME* game);
	~MAINGAMEUI();

	void						Update();
	HRESULT						Draw();

	HRESULT						LoadTexture();
	
	void						UpdateHealth(PLAYER* player, INT* pNewHealth);
	void						UpdateScore(PLAYER* player, DWORD* pNewScore);

} MAINGAMEUI;

#endif // !__UI_GAME_H__
