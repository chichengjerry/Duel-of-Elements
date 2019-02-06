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
// x����x�W�F�Ȑ���y���v�Z����B
// �R���g���[���|�C���g��P0(0, 0)�AP1(mx1, my1)�AP2(mx2, my2)�AP3(1, 1)�l�ł���A
// ���� mx1 �y�� mx2 �͈̔͂� [0, 1] �ƂȂ�B
//
BOOL			Bezier(FLOAT* y, CONST FLOAT x, CONST FLOAT mx1, CONST FLOAT my1, CONST FLOAT mx2, CONST FLOAT my2);

#define BINARY_SUBDIVISION		FALSE	/* �ו��� */

//
// ��p�x�W�F�Ȑ��̃G�C���A�X�B
//
#define BezierEaseInOut(x, y)	Bezier((y), (x), 0.42f, 0.0f, 0.58f, 1.0f)
#define BezierEaseIn(x, y)		Bezier((y), (x), 0.42f, 0.0f, 1.0f, 1.0f)
#define BezierEaseOut(x, y)		Bezier((y), (x), 0.0f, 0.0f, 0.58f, 1.0f)
#define BezierEase(x, y)		Bezier((y), (x), 0.25f, 0.1f, 0.25f, 1.0f)
#define BezierLinear(x, y)		Bezier((y), (x), 0.0f, 0.0f, 1.0f, 1.0f)

#endif // !__TIMER_H__
