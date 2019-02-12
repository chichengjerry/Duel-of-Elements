#ifndef __PERLIN_H__
#define __PERLIN_H__

#include "main.h"
	
typedef struct PERLIN {
	static int			p[512];
	int					repeat;

	PERLIN(int repeat);

	FLOAT				OctavePerlin(FLOAT x, FLOAT y, FLOAT z, int octaves, FLOAT persistence);
	int					inc(int num);
	FLOAT				Perlin(FLOAT x, FLOAT y, FLOAT z);
} PERLIN;

#endif // !__PERLIN_H__
