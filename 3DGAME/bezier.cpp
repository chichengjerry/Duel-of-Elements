#include "bezier.h"

/***********************************************
 * �}�N����`
 ***********************************************/

#define NEWTON_ITERATIONS		4
#define SLOPE_PRECISION			0.0001f
#define KSPLINE_TABLE_SIZE		11
#define KSAMPLE_STEP_SIZE		(1.0f / (KSPLINE_TABLE_SIZE - 1.0f))

#if BINARY_SUBDIVISION
#define SUBDIVISION_PRECISION	0.0000001f
#define SUBDIVISION_ITERATIONS	10
#endif // BINARY_SUBDIVISION

#define FA(aA1, aA2)			(1.0f - 3.0f * aA2 + 3.0f * aA1)
#define FB(aA1, aA2)			(3.0f * aA2 - 6.0f * aA1)
#define FC(aA1)					(3.0f * aA1)

/***********************************************
 * �C���v�������e�[�V����
 ***********************************************/

/**
 �x�W�F�Ȑ��֐��B
 */
float bezier_func(float aT, float aA1, float aA2) {
	return ((FA(aA1, aA2) * aT + FB(aA1, aA2)) * aT + FC(aA1)) * aT;
}

/**
 �x�W�F�Ȑ��̔����֐��B
 */
float bezier_slope_func(float aT, float aA1, float aA2) {
	return 3.0f * FA(aA1, aA2) * aT * aT + 2.0f * FB(aA1, aA2) * aT + FC(aA1);
}

#if BINARY_SUBDIVISION
/**
 �ו�������B
 */
float binary_subdivide(float aX, float aA, float aB, float mx1, float mx2) {
	float x, t;
	int i = 0;

	do {
		t = aA + (aB - aA) / 2.0f;
		x = calc_bezier(t, mx1, mx2) - aX;

		if (x > 0.0f)
			aB = t;
		else
			aA = t;

	} while (fabsf(x) > SUBDIVISION_PRECISION && ++i < SUBDIVISION_ITERATIONS);

	return t;
}
#endif // BINARY_SUBDIVISION

/**
 �j���[�g���E���t�\���@�Őڋߒl�����߂�B
 */ 
float newton_raphson_iterate(float ax, float aT, float mx1, float mx2) {
	// �C�^���[�V����
	for (int i = 0; i < NEWTON_ITERATIONS; i++) {
		float s = bezier_slope_func(aT, mx1, mx2);
		if (fabsf(s) < SLOPE_PRECISION) {
			return aT;
		}
		aT -= (bezier_func(aT, mx1, mx2) - ax) / s;
	}
	return aT;
}

/**
 �x�W�F�Ȑ��̌v�Z����B
 �R���g���[���|�C���g��P0(0, 0)�AP1(mx1, my1)�AP2(mx2, my2)�AP3(1, 1)�l�ł���A
 ���� mx1 �y�� mx2 �͈̔͂� [0, 1] �ƂȂ�B
 */
float bezier(float x, float mx1, float my1, float mx2, float my2) {
	if (!(0.0f <= mx1 && mx1 <= 1.0f && 0.0f <= mx2 && mx2 <= 1.0f)) {
		// ���͒l�ɈႢ������
		return -1.0f;
	}
	if (fabsf(x) < SLOPE_PRECISION) {
		// �n�_
		return 0.0f;
	}
	if (fabsf(x - 1.0f) < SLOPE_PRECISION) {
		// �I�_
		return 1.0f;
	}
	if (mx1 == my1 && mx2 == my2) {
		// ���j�A�̏ꍇ
		return x;
	}

	float sample_values[KSPLINE_TABLE_SIZE];
	for (int i = 0; i < KSPLINE_TABLE_SIZE; i++) {
		sample_values[i] = bezier_func(i * KSAMPLE_STEP_SIZE, mx1, mx2);
	}

	float aX = x;
	float interval_start = 0.0f;
	float d, s, t;
	int current_sample = 1;

	for (; current_sample != KSAMPLE_STEP_SIZE - 1 && sample_values[current_sample] <= x; current_sample++) {
		interval_start += KSAMPLE_STEP_SIZE;
	}
	current_sample--;

	d = (aX - sample_values[current_sample]) / (sample_values[current_sample + 1] - sample_values[current_sample]);
	t = interval_start + d * KSAMPLE_STEP_SIZE;
	s = bezier_slope_func(t, mx1, mx2);
	if (s >= SLOPE_PRECISION) {
		t = newton_raphson_iterate(aX, t, mx1, mx2);
	}

#if BINARY_SUBDIVISION
	else if (s != 0.0f) {
		t = binary_subdivide(aX, interval_start, interval_start + KSAMPLE_STEP_SIZE, mx1, mx2);
	}
#endif // BINARY_SUBDIVISION

	return bezier_func(t, my1, my2);
}