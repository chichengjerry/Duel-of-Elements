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

//
// �ץ쥤��`���
//
typedef struct _player {
	BOOL			isComputer;
	DWORD			nHealth = 100;
	FLOAT			fMoveSpeed = 1.0f;
	ELEMENTS		elements = 0;
	MODEL			mdl;
	SRT				srt;

	void*			lpVtbl;
} PLAYER;

#endif // !__PLAYER_H__
