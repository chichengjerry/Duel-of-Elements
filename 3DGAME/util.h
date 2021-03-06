//-----------------------------------------------
// [util.h]
// ヘルパー関数、マクロ定義
// 作者 Chicheng Zhang
//-----------------------------------------------
#ifndef __UTIL_H__
#define __UTIL_H__

#include "main.h"
#include "maths.h"

#define SAFE_RELEASE(p)			{ if(p) { (p)->Release(); p = NULL; } }
#define SAFE_DELETE(p)			{ if(p) { delete p; p = NULL; } }
#define SAFE_FREE(p)			{ if(p) { free(p); p = NULL; } }
// [a, b]の間にランダム数を取得する。
#define GET_RANDOM(a, b)		((a) + rand() / (RAND_MAX / ((b) - (a) + 1) + 1))
// [0, 1)の範囲でランダム数を生成し、それは1/nより小さいかをチェック。
#define DICE(n)					(rand() < (RAND_MAX + 1u) / (n))

#endif // !__UTIL_H__
