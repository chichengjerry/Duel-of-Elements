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

//
// プレイヤー情報
//

typedef struct PLAYER {
	MODEL*					plane;
	MODEL*					propeller;
	CAMERA*					camera;
	SRT						srt;

	DWORD					id;
	DWORD					gx;
	DWORD					gz;
	BOOL					isPlayer;
	DWORD					nHealth;
	FLOAT					fMoveSpeed;
	FLOAT					fRollSpeed;
	ELEMENTTYPE				attackType;
	ELEMENTTYPE				defenseType;

	PLAYER(int playerId, BOOL isPlayer, D3DXVECTOR3 spawn);
	~PLAYER();

	HRESULT					Draw();
	void					Update(TERRAIN* terrtain);

	void					Move(TERRAIN* terrtain);
	void					SetPlayerHeight(TERRAIN* terrtain);
} PLAYER;

#endif // !__PLAYER_H__
