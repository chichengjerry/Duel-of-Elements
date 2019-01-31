/***********************************************
 * [model.h]
 * �ץ쥤��`�I��
 * GP11A341 24 ���Y�G
 ***********************************************/

#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "main.h"
#include "core.h"
#include "element.h"
#include "model.h"


#define MAX_PLAYER			2
//
// �ץ쥤��`���
//

typedef struct PLAYER {
	DWORD					id;
	BOOL					isComputer;
	DWORD					nHealth = 100;
	FLOAT					fMoveSpeed = 1.0f;
	ELEMENTTYPE				atk = ELEMENTTYPE_NONE;
	ELEMENTTYPE				def = ELEMENTTYPE_NONE;
	MODEL*					mdl;
	SRT						srt;

	PLAYER(BOOL isComputer, SRT* spawn);
	~PLAYER();
	HRESULT					Draw();
	void					Update();
} PLAYER;

#endif // !__PLAYER_H__
