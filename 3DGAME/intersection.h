#ifndef __INTERSECTION_H__
#define __INTERSECTION_H__

#include "main.h"

//
// ���֤������Τν�����Ӌ�㤹�롣
// a, b, c: �����Τ�픵�
// p, q: ���򾀷�
// pd: ����� pd(u, v, w) ����������ϵ
// pt: 
//
BOOL intersect_segment_against_triangle(
	D3DXVECTOR3* pd, FLOAT *pt,
	D3DXVECTOR3 a, D3DXVECTOR3 b, D3DXVECTOR3 c,
	D3DXVECTOR3 p, D3DXVECTOR3 q
);

#endif // !__INTERSECTION_H__
