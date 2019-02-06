#ifndef __EASING_H__
#define __EASING_H__

#include "main.h"

typedef struct EASING {
	DWORD		dwSrcVar;
	DWORD		dwDestVar;
	DWORD		dwDuration;
	DWORD		dwTimer;

	void		SetTimer(DWORD pSrcVar, DWORD pDestVar, DWORD duration);
	DWORD		GetValue();
} EASING;

//
// xからベジェ曲線のyを計算する。
// コントロールポイントはP0(0, 0)、P1(mx1, my1)、P2(mx2, my2)、P3(1, 1)四つであり、
// 且つ mx1 及び mx2 の範囲は [0, 1] となる。
//
BOOL			Bezier(FLOAT* y, CONST FLOAT x, CONST FLOAT mx1, CONST FLOAT my1, CONST FLOAT mx2, CONST FLOAT my2);

#define BINARY_SUBDIVISION		FALSE	/* 細分化 */

//
// 常用ベジェ曲線のエイリアス。
//
#define BezierEaseInOut(x, y)	Bezier((y), (x), 0.42f, 0.0f, 0.58f, 1.0f)
#define BezierEaseIn(x, y)		Bezier((y), (x), 0.42f, 0.0f, 1.0f, 1.0f)
#define BezierEaseOut(x, y)		Bezier((y), (x), 0.0f, 0.0f, 0.58f, 1.0f)
#define BezierEase(x, y)		Bezier((y), (x), 0.25f, 0.1f, 0.25f, 1.0f)
#define BezierLinear(x, y)		Bezier((y), (x), 0.0f, 0.0f, 1.0f, 1.0f)

#endif // !__TIMER_H__
