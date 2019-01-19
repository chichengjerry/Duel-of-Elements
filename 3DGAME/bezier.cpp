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

/***********************************************
 * インプリメンテーション
 ***********************************************/

#define FACTOR_A(a, b)			(1.0f - 3.0f * (b) + 3.0f * (a))
#define FACTOR_B(a, b)			(3.0f * (b) - 6.0f * (a))
#define FACTOR_C(a)				(3.0f * (a))

//
// ベジェ曲線関数。
//
static FLOAT bezier(FLOAT aT, FLOAT aA1, FLOAT aA2) {
	return ((FACTOR_A(aA1, aA2) * aT + FACTOR_B(aA1, aA2)) * aT + FACTOR_C(aA1)) * aT;
}

//
// ベジェ曲線の微分関数。
//
static FLOAT bezier_slope(FLOAT aT, FLOAT aA1, FLOAT aA2) {
	return 3.0f * FACTOR_A(aA1, aA2) * aT * aT + 2.0f * FACTOR_B(aA1, aA2) * aT + FACTOR_C(aA1);
}

#undef FACTOR_A
#undef FACTOR_B
#undef FACTOR_C

#if BINARY_SUBDIVISION
//
// 細分化。
//
FLOAT subdivide(FLOAT aX, FLOAT aA, FLOAT aB, FLOAT mx1, FLOAT mx2) {
	FLOAT x, t;
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

//
// ニュートン法で接近値を求める。
// 
static FLOAT newton_raphson_iterate(FLOAT ax, FLOAT aT, FLOAT mx1, FLOAT mx2) {
	// イタレーション
	for (int i = 0; i < NEWTON_ITERATIONS; i++) {
		FLOAT s = bezier_slope(aT, mx1, mx2);
		if (fabsf(s) < SLOPE_PRECISION) {
			return aT;
		}
		aT -= (bezier(aT, mx1, mx2) - ax) / s;
	}
	return aT;
}

static BOOL bezier_calc(FLOAT* y, CONST FLOAT x, CONST FLOAT mx1, CONST FLOAT my1, CONST FLOAT mx2, CONST FLOAT my2) {
	
	//早めに関数を完成させる
	if (!(0.0f <= mx1 && mx1 <= 1.0f && 0.0f <= mx2 && mx2 <= 1.0f)) {
		// 入力値に違いがある
		return FALSE;
	}
	if (fabsf(x) < SLOPE_PRECISION) {
		// 始点
		*y = 0.0f;
		return TRUE;
	}
	if (fabsf(x - 1.0f) < SLOPE_PRECISION) {
		// 終点
		*y = 1.0f;
		return TRUE;
	}
	if (mx1 == my1 && mx2 == my2) {
		// リニアの場合
		*y = x;
		return TRUE;
	}

	FLOAT sample[KSPLINE_TABLE_SIZE];
	for (int i = 0; i < KSPLINE_TABLE_SIZE; i++) {
		sample[i] = bezier(i * KSAMPLE_STEP_SIZE, mx1, mx2);
	}

	FLOAT aX = x;
	FLOAT interval_start = 0.0f;
	FLOAT d, s, t;
	int current;

	for (current = 1; current != KSAMPLE_STEP_SIZE - 1 && sample[current] <= x; current++) {
		interval_start += KSAMPLE_STEP_SIZE;
	}

	current--;

	d = (aX - sample[current]) / (sample[current + 1] - sample[current]);
	t = interval_start + d * KSAMPLE_STEP_SIZE;
	s = bezier_slope(t, mx1, mx2);

	if (s >= SLOPE_PRECISION) {
		t = newton_raphson_iterate(aX, t, mx1, mx2);
	}

#if BINARY_SUBDIVISION
	else if (s != 0.0f) {
		t = subdivide(aX, interval_start, interval_start + KSAMPLE_STEP_SIZE, mx1, mx2);
	}
#endif // BINARY_SUBDIVISION

	*y = bezier(t, my1, my2);

	return TRUE;
}