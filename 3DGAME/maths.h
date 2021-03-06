//-----------------------------------------------
// [maths.h]
// 数学相関関数
// 作者 Chicheng Zhang
//-----------------------------------------------
#ifndef __MATHS_H__
#define __MATHS_H__

#include "main.h"

FLOAT			BarryCentric(D3DXVECTOR3 p1, D3DXVECTOR3 p2, D3DXVECTOR3 p3, D3DXVECTOR2 v);
BOOL			IntersectRaySphere(D3DXVECTOR3* pOut, D3DXVECTOR3 p, D3DXVECTOR3 d, D3DXVECTOR3 sc, FLOAT r);

#endif // !__MATHS_H__
