/***********************************************
 * [game.h]
 * •≤©`•‡ÑI¿Ì
 * GP11A341 24 èàÒYÚG
 ***********************************************/

#include "main.h"
#include "core.h"
#include "camera.h"
#include "element.h"
#include "map.h"
#include "player.h"

#ifndef __GAME_H__
#define __GAME_H__

typedef struct MAINGAME {
	ELEMENT					elements[MAX_ELEMENTS];
	MAP						map;
	PLAYER					player[MAX_PLAYER];
	CAMERAS					cameras;
} MAINGAME;


#endif // !__GAME_H__
