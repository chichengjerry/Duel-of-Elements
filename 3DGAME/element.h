/***********************************************
 * [element.h]
 * •®•Ï•·•Û•»ÑI¿Ì
 * GP11A341 24 èàÒYÚG
 ***********************************************/

#ifndef _ELEMENT_H_
#define _ELEMENT_H_

#include "main.h"
#include "core.h"
#include "model.h"

typedef BYTE ELEMENTS;

enum ELEMENT_TYPE {
	ELEMENT_EARTH,
	ELEMENT_WATER,
	ELEMENT_FIRE,
	ELEMENT_AIR
};

#define MAX_ELEMENTS			16

#define GET_ELEMENTS(e, t)		(((e) & (0x3 << (t) * 2)) >> ((t) * 2))
#define SET_ELEMENTS(e, t, n)	((e) = (e) & (0xff - (0x3 << ((t) * 2))) | ((n) << ((t) * 2)))

//
// •®•Ï•·•Û•»«ÈàÛ
//
typedef struct _element {
	ELEMENTS		elements;

	MODEL			mdl;
	SRT				srt;
} ELEMENT;

ELEMENT* create_element(void* map);

#endif // !_ELEMENT_H_
