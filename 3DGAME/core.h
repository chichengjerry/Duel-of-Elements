/***********************************************
 * [main.h]
 * メイン処理
 * GP11A341 24 張馳騁
 ***********************************************/
#ifndef __CORE_H__
#define __CORE_H__

 /***********************************************
  * インクルードファイル
  ***********************************************/
#include "main.h"

/***********************************************
 * マクロ定義
 ***********************************************/

#define CL_DEVMODE				FALSE

#define CL_HUD					TRUE
#define CL_FULLSCREEN			FALSE
#define CL_HEIGHT				720
#define CL_WIDTH				1280

//	#define CM_CHEAT				FALSE
//	#define CM_GODMODE				FALSE	// GM_CHEATがTRUEのみ有効
//	#define CM_SHOWBB				FALSE	// GM_CHEATがTRUEのみ有効
//	#define CM_SKY					TRUE	// GM_CHEATがTRUEのみ変更できる

#define GM_GRAVITY				800
#define GM_FOG					1000

#define GR_FAR_Z				1024
#define GR_NEAR_Z				1
#define GR_MAXPARTICLES			256
#define GR_VIEWANGLE			60
#define GR_VSYNC				FALSE
#define GR_SHOWFPS				FALSE

struct CAMERA;
struct PLAYER;
struct MAINGAME;

#define	NUM_VERTEX			4		// 頂点数
#define	NUM_POLYGON			2		// ポリゴン数

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
// 位置、サイズ、回転情報
//
typedef struct SRT {
	D3DXVECTOR3				scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	D3DXVECTOR3				rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3				pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	SRT();
	SRT(D3DXVECTOR3 pos);
} SRT;


#endif // !__CORE_H__
