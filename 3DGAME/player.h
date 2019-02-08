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
	FLOAT					fHeight;
	ELEMENTTYPE				elementType;

	PLAYER(int playerId, BOOL bControlable, D3DXVECTOR3 spawn);
	~PLAYER();

	HRESULT					Draw();
	void					Update(TERRAIN* terrtain);

	void					UpdateMovement(TERRAIN* terrtain);
	void					SetPlayerHeight(TERRAIN* terrtain);
	void					UpdateScore(DWORD* score);
} PLAYER;

#endif // !__PLAYER_H__
