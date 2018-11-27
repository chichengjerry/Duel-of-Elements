/***********************************************
 * [bezier.heap]
 * �x�W�F�Ȑ��ɂ�铮�揈��
 * GP11A341 24 ���y�t
 ***********************************************/
#ifndef _BEZIER_H_
#define _BEZIER_H_

#include "main.h"

/***********************************************
 * �}�N����`
 ***********************************************/

#define BINARY_SUBDIVISION		0	/* �ו��� */

/***********************************************
 * �֐��錾
 ***********************************************/

/**
 �x�W�F�Ȑ��̌v�Z����B
 �R���g���[���|�C���g��P0(0, 0)�AP1(mx1, my1)�AP2(mx2, my2)�AP3(1, 1)�l�ł���A
 ���� mx1 �y�� mx2 �͈̔͂� [0, 1] �ƂȂ�B
 */
float bezier(float x, float mx1, float my1, float mx2, float my2);

/**
 ��p�x�W�F�Ȑ��̃G�C���A�X�B
 */

#define EASE_IN_OUT(t)			bezier((t), 0.42f, 0.0f, 0.58f, 1.0f)
#define EASE_IN(t)				bezier((t), 0.42f, 0.0f, 1.0f, 1.0f)
#define EASE_OUT(t)				bezier((t), 0.0f, 0.0f, 0.58f, 1.0f)
#define EASE(t)					bezier((t), 0.25f, 0.1f, 0.25f, 1.0f)
#define LINEAR(t)				bezier((t), 0.0f, 0.0f, 1.0f, 1.0f)

#endif // !_BEZIER_H_
