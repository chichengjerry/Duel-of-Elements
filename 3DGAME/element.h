/***********************************************
 * [element.h]
 * エレメント処理
 * GP11A341 24 張馳騁
 ***********************************************/

#ifndef _ELEMENT_H_
#define _ELEMENT_H_

#include "main.h"
#include "core.h"
#include "gamemain.h"
#include "model.h"
#include "particle.h"

enum ELEMENTTYPE : DWORD {
	ELEMENTTYPE_NONE,
	ELEMENTTYPE_EARTH,
	ELEMENTTYPE_WATER,
	ELEMENTTYPE_FIRE,
	ELEMENTTYPE_AIR,

	ELEMENTTYPE_NUM
};

extern D3DXCOLOR ElementColor[];
extern int DAMAGE_TABLE[ELEMENTTYPE_NUM][ELEMENTTYPE_NUM];
extern int SCORE_TABLE[ELEMENTTYPE_NUM][ELEMENTTYPE_NUM];
//
// エレメント情報
//
struct ELEMENT {

	BOOL					isActive;
	ELEMENTTYPE				elementType;
	SRT						srt;
	DWORD					dwRefreshTimer;
	EMITTER*				emitter;
	MODEL*					mdl;
	TERRAIN*				terrain;

	ELEMENT(TERRAIN* terrain);
	~ELEMENT();

	void					RefreshElement(DWORD time);
	void					SetPosition(D3DXVECTOR3* pos);
	void					Update(PLAYER* player[]);
	HRESULT					Draw(CAMERA* camera);
};

#endif // !_ELEMENT_H_
