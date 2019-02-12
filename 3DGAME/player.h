/***********************************************
 * [model.h]
 * プレイヤー処理
 * GP11A341 24 張馳騁
 ***********************************************/

#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "main.h"
#include "core.h"
#include "gamemain.h"
#include "model.h"
#include "easing.h"

//
// プレイヤー情報
//

typedef struct PLAYER {
	MAINGAME*				pGame;
	MODEL*					plane;
	MODEL*					propeller;
	CAMERA*					camera;
	SRT						srt;

	DWORD					id;
	INT						gx;
	INT						gz;
	BOOL					bControlable;
	BOOL					bFPSCamera;
	INT						nHealth;
	DWORD					dwScore;
	DWORD					dwScoreAnimate;
	EASING					mScoreEasing;

	FLOAT					fMoveSpeed;
	FLOAT					fRollSpeed;
	FLOAT					fYawSpeed;
	FLOAT					fHeight;
	ELEMENTTYPE				elementType;

	PLAYER(MAINGAME* pGame, int playerId, BOOL bControlable);
	~PLAYER();

	HRESULT					Draw();
	void					Update(TERRAIN* terrain);

	void					Fire(void);
	void					UpdateMovement(TERRAIN* terrain);
	void					SetPlayerHeight(TERRAIN* terrain);
	void					SetPlayerInBound(TERRAIN* terrain);
	void					UpdateScore(DWORD* score);
} PLAYER;

#endif // !__PLAYER_H__
