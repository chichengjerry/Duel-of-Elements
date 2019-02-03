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

enum ELEMENTTYPE : DWORD {
	ELEMENTTYPE_NONE,
	ELEMENTTYPE_EARTH,
	ELEMENTTYPE_WATER,
	ELEMENTTYPE_FIRE,
	ELEMENTTYPE_AIR
};

//
// エレメント情報
//
struct ELEMENT {

	BOOL					isActive;
	ELEMENTTYPE				elementType;
	MODEL*					mdl;
	SRT						srt;

	ELEMENT(SRT* srt);
	~ELEMENT();
	void					Fresh(void);
	void					Update(MAINGAME* pGame);
	HRESULT					Draw();
};

#endif // !_ELEMENT_H_
