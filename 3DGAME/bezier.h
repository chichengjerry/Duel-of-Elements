/***********************************************
 * [bezier.heap]
 * ベジェ曲線による動画処理
 * GP11A341 24 張馳騁
 ***********************************************/
#ifndef _BEZIER_H_
#define _BEZIER_H_

#include "main.h"

/***********************************************
 * マクロ定義
 ***********************************************/

#define BINARY_SUBDIVISION		0	/* 細分化 */

/***********************************************
 * 関数宣言
 ***********************************************/

/**
 ベジェ曲線の計算する。
 コントロールポイントはP0(0, 0)、P1(mx1, my1)、P2(mx2, my2)、P3(1, 1)四つであり、
 且つ mx1 及び mx2 の範囲は [0, 1] となる。
 */
float bezier(float x, float mx1, float my1, float mx2, float my2);

/**
 常用ベジェ曲線のエイリアス。
 */

#define EASE_IN_OUT(t)			bezier((t), 0.42f, 0.0f, 0.58f, 1.0f)
#define EASE_IN(t)				bezier((t), 0.42f, 0.0f, 1.0f, 1.0f)
#define EASE_OUT(t)				bezier((t), 0.0f, 0.0f, 0.58f, 1.0f)
#define EASE(t)					bezier((t), 0.25f, 0.1f, 0.25f, 1.0f)
#define LINEAR(t)				bezier((t), 0.0f, 0.0f, 1.0f, 1.0f)

#endif // !_BEZIER_H_
