#include "easing.h"

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

FLOAT			percent(FLOAT start, FLOAT end, FLOAT now);
//
// xからベジェ曲線のyを計算する。
// コントロールポイントはP0(0, 0)、P1(mx1, my1)、P2(mx2, my2)、P3(1, 1)四つであり、
// 且つ mx1 及び mx2 の範囲は [0, 1] となる。
//
BOOL			Bezier(FLOAT* y, CONST FLOAT x, CONST FLOAT mx1, CONST FLOAT my1, CONST FLOAT mx2, CONST FLOAT my2);

#define BINARY_SUBDIVISION		FALSE	/* 細分化 */


 /***********************************************
  * インプリメンテーション
  ***********************************************/

EASING::EASING()
{
	bEasing = FALSE;
	SetEase(BEZIER_LINEAR);
}

EASING::EASING(FLOAT x1, FLOAT y1, FLOAT x2, FLOAT y2)
{
	bEasing = FALSE;
	SetEase(x1, y1, x2, y2);
}

void EASING::SetTimer(DWORD srcVal, DWORD destVal , DWORD duration)
{
	dwTimer = timeGetTime();
	dwDuration = duration;
	dwSrcVar = srcVal;
	dwDestVar = destVal;
	bEasing = TRUE;
}

DWORD EASING::GetValue()
{
	DWORD elapsed = timeGetTime() - dwTimer;
	FLOAT delta;
	Bezier(&delta, percent(0, dwDuration, elapsed), x2, y2, x3, y3);

	if (delta == 1.0f)
		bEasing = FALSE;

	return dwSrcVar + (DWORD)(delta * (dwDestVar - dwSrcVar));
}

void EASING::SetEase(FLOAT x2, FLOAT y2, FLOAT x3, FLOAT y3)
{
	this->x2 = x2;
	this->y2 = y2;
	this->x3 = x3;
	this->y3 = y3;
}

FLOAT percent(FLOAT start, FLOAT end, FLOAT t)
{
	if (t <= start || start >= end)
		return 0.0f;
	if (t >= end)
		return 1.0f;

	return (t - start) / (end - start);
}

#define FACTOR_A(a, b)			(1.0f - 3.0f * (b) + 3.0f * (a))
#define FACTOR_B(a, b)			(3.0f * (b) - 6.0f * (a))
#define FACTOR_C(a)				(3.0f * (a))

//
// ベジェ曲線関数。
//
FLOAT bezier(FLOAT aT, FLOAT aA1, FLOAT aA2) {
	return ((FACTOR_A(aA1, aA2) * aT + FACTOR_B(aA1, aA2)) * aT + FACTOR_C(aA1)) * aT;
}

//
// ベジェ曲線の微分関数。
//
FLOAT bezier_slope(FLOAT aT, FLOAT aA1, FLOAT aA2) {
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
FLOAT newton_raphson_iterate(FLOAT ax, FLOAT aT, FLOAT mx1, FLOAT mx2) {
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

BOOL Bezier(FLOAT* y, CONST FLOAT x, CONST FLOAT mx1, CONST FLOAT my1, CONST FLOAT mx2, CONST FLOAT my2) {

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