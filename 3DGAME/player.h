/***********************************************
 * [model.h]
 * プレイヤー処理
 * GP11A341 24 張馳騁
 ***********************************************/

#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "main.h"
#include "core.h"
#include "element.h"
#include "model.h"


#define MAX_PLAYER			2
//
// プレイヤー情報
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
// プレイヤーの初期化。
//
HRESULT						player_init(PLAYER*, BOOL);

//
// プレイヤーの解放。
//
void						player_uninit(PLAYER*);

//
// プレイヤーを描画する。
//
HRESULT						player_draw(PLAYER*);

//
// プレイヤーを更新する。
//
void						player_update(PLAYER*);

//
// バーチャルテーブル
//
typedef struct VTABLE {
	HRESULT					(*init)(PLAYER*, BOOL) = player_init;
	void					(*uninit)(PLAYER*) = player_uninit;

	HRESULT					(*draw)(PLAYER*) = player_draw;
	void					(*update)(PLAYER*) = player_update;
} VTABLE;

#undef VTABLE

#endif // !__PLAYER_H__
