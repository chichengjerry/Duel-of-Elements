#ifndef __INTERSECTION_H__
#define __INTERSECTION_H__

#include "main.h"

//
// 線分と三角形の交差点を計算する。
// a, b, c: 三角形の頂点
// p, q: 有向線分
// pd: 交差点 pd(u, v, w) の重心座標系
// pt: 
//
BOOL intersect_segment_against_triangle(
	D3DXVECTOR3* pd, FLOAT *pt,
	D3DXVECTOR3 a, D3DXVECTOR3 b, D3DXVECTOR3 c,
	D3DXVECTOR3 p, D3DXVECTOR3 q
);

#endif // !__INTERSECTION_H__
