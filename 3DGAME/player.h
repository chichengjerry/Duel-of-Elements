//-----------------------------------------------
// [player.h]
// プレイヤー処理
// 作者 Chicheng Zhang
//-----------------------------------------------
#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "main.h"
#include "core.h"
#include "gamemain.h"
#include "easing.h"
#include "model.h"
#include "particle.h"

#define YAW_MAX_ANGLE		D3DXToRadian(90.0f)
#define YAW_STEP			D3DXToRadian(45.0f)

#define PITCH_MAX_ANGLE		D3DXToRadian(30.0f)
#define PITCH_STEP			D3DXToRadian(30.0f)

#define ROLL_MAX_ANGLE		D3DXToRadian(15.0f)
#define ROLL_STEP			D3DXToRadian(30.0f)

#define MOVE_MAX_SPEED		128.0f
#define MOVE_MIN_SPEED		0.0f
#define MOVE_STEP			64.0f

#define PLAYER_SIZE			10.0f

//
// プレイヤー情報
//

typedef struct PLAYER {
	MAINGAME*				pGame;

	MODEL*					plane;
	MODEL*					propeller;
	CAMERA*					camera;
	EMITTER*				glow;

	SRT						srt;

	DWORD					id;
	INT						gx;
	INT						gz;
	BOOL					bControlable;
	BOOL					bFPSCamera;
	BOOL					bCrash;
	INT						nHealth;
	DWORD					nScore;
	FLOAT					fSize;

	FLOAT					fMoveSpeed;
	FLOAT					fRollSpeed;
	FLOAT					fYawSpeed;
	FLOAT					fHeight;
	ELEMENTTYPE				defenseType;
	ELEMENTTYPE				attackType;

	PLAYER(MAINGAME* pGame, int playerId, BOOL bControlable);
	~PLAYER();

	HRESULT					Draw(CAMERA* camera);
	void					Update(TERRAIN* terrain);

	void					SwapElements();
	void					Fire(void);
	void					Damaged(BULLET* bullet);
	void					Destroy();
	void					UpdateMovement(TERRAIN* terrain);
	void					SetPlayerHeight(TERRAIN* terrain);
	void					SetPlayerInBound(TERRAIN* terrain);
	void					UpdateHealth(INT* pNewHealth);
	void					UpdateScore(DWORD* score);
} PLAYER;

#endif // !__PLAYER_H__
