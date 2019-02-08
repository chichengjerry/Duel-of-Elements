#ifndef __EASING_H__
#define __EASING_H__

#include "main.h"

typedef struct EASING {
	DWORD		dwSrcVar;
	DWORD		dwDestVar;
	DWORD		dwDuration;
	DWORD		dwTimer;
	BOOL		bEasing;
	FLOAT		x2, y2, x3, y3;

	EASING();
	EASING(FLOAT x1, FLOAT y1, FLOAT x2, FLOAT y2);

	void		SetTimer(DWORD pSrcVar, DWORD pDestVar, DWORD duration);
	DWORD		GetValue();
	void		SetEase(FLOAT x2, FLOAT y2, FLOAT x3, FLOAT y3);
} EASING;

//
// 常用ベジェ曲線のエイリアス。
//
#define BEZIER_EASE_IN_OUT		0.42f, 0.0f, 0.58f, 1.0f
#define BEZIER_EASE_IN			0.42f, 0.0f, 1.0f, 1.0f
#define BEZIER_EASE_OUT			0.0f, 0.0f, 0.58f, 1.0f
#define BEZIER_EASE				0.25f, 0.1f, 0.25f, 1.0f
#define BEZIER_LINEAR			0.0f, 0.0f, 1.0f, 1.0f

#endif // !__TIMER_H__
