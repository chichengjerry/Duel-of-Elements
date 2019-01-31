#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "main.h"

typedef struct LIGHT {
	D3DLIGHT9		light[3];

	LIGHT();
	~LIGHT();
} LIGHT;

#endif // !__LIGHT_H__
