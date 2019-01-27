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
#define VTABLE PLAYERVTBL
struct VTABLE;

typedef struct _player {
	BOOL					isComputer;
	DWORD					nHealth = 100;
	FLOAT					fMoveSpeed = 1.0f;
	ELEMENTS				elements = 0;
	MODEL					mdl;
	SRT						srt;

	VTABLE*					lpVtbl;
} PLAYER;

//
// �ץ쥤��`�γ��ڻ���
//
HRESULT						player_init(PLAYER*, BOOL);

//
// �ץ쥤��`�ν�š�
//
void						player_uninit(PLAYER*);

//
// �ץ쥤��`���軭���롣
//
HRESULT						player_draw(PLAYER*);

//
// �ץ쥤��`����¤��롣
//
void						player_update(PLAYER*);

//
// �Щ`�����Ʃ`�֥�
//
typedef struct VTABLE {
	HRESULT					(*init)(PLAYER*, BOOL) = player_init;
	void					(*uninit)(PLAYER*) = player_uninit;

	HRESULT					(*draw)(PLAYER*) = player_draw;
	void					(*update)(PLAYER*) = player_update;
} VTABLE;

#undef VTABLE

#endif // !__PLAYER_H__
