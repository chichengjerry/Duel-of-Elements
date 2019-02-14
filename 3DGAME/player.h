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

#define YAW_MAX_ANGLE		D3DXToRadian(90.0f)
#define YAW_STEP			D3DXToRadian(90.0f)

#define PITCH_MAX_ANGLE		D3DXToRadian(30.0f)
#define PITCH_STEP			D3DXToRadian(30.0f)

#define ROLL_STEP			D3DXToRadian(30.0f)
#define ROLL_MAX_ANGLE		D3DXToRadian(15.0f)

#define MOVE_MAX_SPEED		5.0f
#define MOVE_MIN_SPEED		1.0f
#define MOVE_STEP			1.0f

#define PLAYER_SIZE			10.0f

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
	FLOAT					fSize;

	FLOAT					fMoveSpeed;
	FLOAT					fRollSpeed;
	FLOAT					fYawSpeed;
	FLOAT					fHeight;
	ELEMENTTYPE				defenseType;
	ELEMENTTYPE				attackType;

	PLAYER(MAINGAME* pGame, int playerId, BOOL bControlable);
	~PLAYER();

	HRESULT					Draw();
	void					Update(TERRAIN* terrain);

	void					SwapElements();
	void					Fire(void);
	void					UpdateMovement(TERRAIN* terrain);
	void					SetPlayerHeight(TERRAIN* terrain);
	void					SetPlayerInBound(TERRAIN* terrain);
	void					UpdateScore(DWORD* score);
} PLAYER;

#endif // !__PLAYER_H__
