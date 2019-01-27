/***********************************************
 * [game.h]
 * ¥²©`¥à„IÀí
 * GP11A341 24 ˆñYòG
 ***********************************************/

#include "main.h"
#include "element.h"
#include "map.h"
#include "player.h"

#ifndef __GAME_H__
#define __GAME_H__

#define GAME_INIT(p)		(p)->lpVtbl->init(p)
#define GAME_UNINIT(p)		(p)->lpVtbl->uninit(p)

#define GAME_DRAW(p)		(p)->lpVtbl->draw(p)
#define GAME_GETMATRIX(p)	(p)->lpVtbl->get_matrix(p)
#define GAME_GETSRT(p)		(p)->lpVtbl->get_srt(p)
#define GAME_UPDATE(p)		(p)->lpVtbl->update(p)

typedef struct _game {
	PLAYER					player[MAX_PLAYER];
	MAP						map;
	ELEMENT					elements[MAX_ELEMENTS];
} GAME;


#endif // !__GAME_H__
