#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "main.h"
#include "core.h"

typedef struct LIGHT {
	D3DLIGHT9		light;

	LIGHT();
	~LIGHT();

	void			Update();
} LIGHT;

#endif // !__LIGHT_H__
