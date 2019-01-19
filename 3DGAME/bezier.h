/***********************************************
 * [bezier.h]
 * ベジェ曲線による動画処理
 * GP11A341 24 張馳騁
 ***********************************************/
#ifndef __BEZIER_H__
#define __BEZIER_H__

#include "main.h"

/***********************************************
 * マクロ定義
 ***********************************************/

#define BINARY_SUBDIVISION		FALSE	/* 細分化 */

/***********************************************
 * 関数宣言
 ***********************************************/

//
// xからベジェ曲線のyを計算する。
// コントロールポイントはP0(0, 0)、P1(mx1, my1)、P2(mx2, my2)、P3(1, 1)四つであり、
// 且つ mx1 及び mx2 の範囲は [0, 1] となる。
//
static BOOL bezier_calc(FLOAT* y, CONST FLOAT x, CONST FLOAT mx1, CONST FLOAT my1, CONST FLOAT mx2, CONST FLOAT my2);

//
// 常用ベジェ曲線のエイリアス。
//

#define BEZIER_EASE_IN_OUT(t)	bezier_calc((t), 0.42f, 0.0f, 0.58f, 1.0f)
#define BEZIER_EASE_IN(t)		bezier_calc((t), 0.42f, 0.0f, 1.0f, 1.0f)
#define BEZIER_EASE_OUT(t)		bezier_calc((t), 0.0f, 0.0f, 0.58f, 1.0f)
#define BEZIER_EASE(t)			bezier_calc((t), 0.25f, 0.1f, 0.25f, 1.0f)
#define LINEAR(t)				bezier_calc((t), 0.0f, 0.0f, 1.0f, 1.0f)

#endif // !__BEZIER_H__
