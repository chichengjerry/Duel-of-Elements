/***********************************************
 * [util.h]
 * 統合関数
 * GP11A341 24 張馳騁
 ***********************************************/

#ifndef __UTIL_H__
#define __UTIL_H__

#include "main.h"

#define SAFE_RELEASE(p)			{ if(p) { (p)->Release(); p = NULL; } }
#define SAFE_DELETE(p)			{ if(p) { delete p; p = NULL; } }
#define FREE(p)					{ if(p) { free(p); p = NULL; } }
#define GET_RANDOM(a, b)		((a) + rand() / (RAND_MAX / ((b) - (a) + 1) + 1))
#define DICE(n)					(rand() < (RAND_MAX + 1u) / (n))

FLOAT							BarryCentric(D3DXVECTOR3 p1, D3DXVECTOR3 p2, D3DXVECTOR3 p3, D3DXVECTOR2 v);

#endif // !__UTIL_H__
