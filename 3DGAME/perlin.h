//-----------------------------------------------
// [perlin.h]
// ノイズ生成
// 作者 Chicheng Zhang
// 参考資料：https://gist.github.com/Flafla2/f0260a861be0ebdeef76
//-----------------------------------------------
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
