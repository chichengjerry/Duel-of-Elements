/***********************************************
 * [model.h]
 * プレイヤー処理
 * GP11A341 24 張馳騁
 ***********************************************/

#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "main.h"
#include "core.h"
#include "camera.h"
#include "element.h"
#include "model.h"


#define MAX_PLAYER			2
//
// プレイヤー情報
//

typedef struct PLAYER {
	MODEL*					mdl;
	CAMERA*					camera;
	SRT						srt;

	DWORD					id;
	BOOL					isPlayer;
	DWORD					nHealth = 100;
	FLOAT					fMoveSpeed = 1.0f;
	ELEMENTTYPE				attackType = ELEMENTTYPE_NONE;
	ELEMENTTYPE				defenseType = ELEMENTTYPE_NONE;

	PLAYER(int playerId, BOOL isPlayer, D3DXVECTOR3 spawn);
	~PLAYER();
	HRESULT					Draw();
	void					Update();
} PLAYER;

#endif // !__PLAYER_H__
