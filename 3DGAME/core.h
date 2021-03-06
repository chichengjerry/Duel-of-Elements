//-----------------------------------------------
// [core.h]
// 汎用情報の定義
// 作者 Chicheng Zhang
//-----------------------------------------------
#ifndef __CORE_H__
#define __CORE_H__

#include "main.h"

#define CL_FULLSCREEN			FALSE
#define CL_HEIGHT				720
#define CL_WIDTH				1280

#define	NUM_VERTEX				4
#define	NUM_POLYGON				2

// ２Ｄポリゴン頂点フォーマット( 頂点座標[2D] / 反射光 / テクスチャ座標 )
#define	FVF_VERTEX_2D		(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

typedef struct VERTEX_2D {
	D3DXVECTOR3				vtx;		// 頂点座標
	FLOAT					rhw;		// テクスチャのパースペクティブコレクト用
	D3DCOLOR				dif;		// 反射光
	D3DXVECTOR2				tex;		// テクスチャ座標
} VERTEX_2D;

// ３Ｄポリゴン頂点フォーマット( 頂点座標[3D] / 法線 / 反射光 / テクスチャ座標 )
#define	FVF_VERTEX_3D		(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

typedef struct VERTEX_3D {
	D3DXVECTOR3				vtx;		// 頂点座標
	D3DXVECTOR3				nor;		// 法線ベクトル
	D3DCOLOR				dif;		// 反射光
	D3DXVECTOR2				tex;		// テクスチャ座標
} VERTEX_3D;

//
// サイズ、回転、位置情報（Scale Rotation Translation）
//
typedef struct SRT {
	D3DXVECTOR3				scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	D3DXVECTOR3				rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3				pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	SRT()
	{
		scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	SRT(D3DXVECTOR3 translate)
	{
		scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pos = translate;
	};
} SRT;

#endif // !__CORE_H__
