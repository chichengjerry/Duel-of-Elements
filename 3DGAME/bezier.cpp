#include "bezier.h"

/***********************************************
 * マクロ定義
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
 * インプリメンテーション
 ***********************************************/

/**
 ベジェ曲線関数。
 */
float bezier_func(float aT, float aA1, float aA2) {
	return ((FA(aA1, aA2) * aT + FB(aA1, aA2)) * aT + FC(aA1)) * aT;
}

/**
 ベジェ曲線の微分関数。
 */
float bezier_slope_func(float aT, float aA1, float aA2) {
	return 3.0f * FA(aA1, aA2) * aT * aT + 2.0f * FB(aA1, aA2) * aT + FC(aA1);
}

#if BINARY_SUBDIVISION
/**
 細分化する。
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
 ニュートン・ラフソン法で接近値を求める。
 */ 
float newton_raphson_iterate(float ax, float aT, float mx1, float mx2) {
	// イタレーション
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
 ベジェ曲線の計算する。
 コントロールポイントはP0(0, 0)、P1(mx1, my1)、P2(mx2, my2)、P3(1, 1)四つであり、
 且つ mx1 及び mx2 の範囲は [0, 1] となる。
 */
float bezier(float x, float mx1, float my1, float mx2, float my2) {
	if (!(0.0f <= mx1 && mx1 <= 1.0f && 0.0f <= mx2 && mx2 <= 1.0f)) {
		// 入力値に違いがある
		return -1.0f;
	}
	if (fabsf(x) < SLOPE_PRECISION) {
		// 始点
		return 0.0f;
	}
	if (fabsf(x - 1.0f) < SLOPE_PRECISION) {
		// 終点
		return 1.0f;
	}
	if (mx1 == my1 && mx2 == my2) {
		// リニアの場合
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