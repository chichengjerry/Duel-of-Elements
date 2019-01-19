/***********************************************
 * [model.h]
 * モデル処理
 * GP11A341 24 張馳騁
 ***********************************************/

#ifndef __MODEL_H__
#define __MODEL_H__

#include "main.h"
#include "core.h"
#include "util.h"

#define	NUM_VERTEX			4		// 頂点数
#define	NUM_POLYGON			2		// ポリゴン数

// ２Ｄポリゴン頂点フォーマット( 頂点座標[2D] / 反射光 / テクスチャ座標 )
#define	FVF_VERTEX_2D		(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

typedef struct _vertex_2d {
	D3DXVECTOR3				vtx;		// 頂点座標
	FLOAT					rhw;		// テクスチャのパースペクティブコレクト用
	D3DCOLOR				dif;		// 反射光
	D3DXVECTOR2				tex;		// テクスチャ座標
} VERTEX_2D;

// ３Ｄポリゴン頂点フォーマット( 頂点座標[3D] / 法線 / 反射光 / テクスチャ座標 )
#define	FVF_VERTEX_3D		(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

typedef struct _vertex_3d {
	D3DXVECTOR3				vtx;		// 頂点座標
	D3DXVECTOR3				nor;		// 法線ベクトル
	D3DCOLOR				dif;		// 反射光
	D3DXVECTOR2				tex;		// テクスチャ座標
} VERTEX_3D;

//
// 位置、サイズ、回転情報
//
typedef struct _srt {
	D3DXVECTOR3				pos;
	D3DXVECTOR3				rot;
	D3DXVECTOR3				scl;
} SRT;

void						srt_init(SRT*);

//
// 関数オーバーライド
//

#define GAME_INIT(p)		(p)->lpVtbl->init(p)
#define GAME_UNINIT(p)		(p)->lpVtbl->uninit(p)

#define GAME_DRAW(p)		(p)->lpVtbl->draw(p)
#define GAME_GETMATRIX(p)	(p)->lpVtbl->get_matrix(p)
#define GAME_GETSRT(p)		(p)->lpVtbl->get_srt(p)
#define GAME_UPDATE(p)		(p)->lpVtbl->update(p)

//
// ビルボード情報
//
typedef struct _billboard {
	LPDIRECT3DVERTEXBUFFER9	pVtx;
	LPDIRECT3DTEXTURE9		pTex;

	D3DXMATRIX				mtx;
	SRT						srt;

	//	バーチャルテーブル
	void*					lpVtbl;
} BILLBOARD;

HRESULT						billboard_init(BILLBOARD*, LPCWSTR);
void						billboard_uninit(BILLBOARD*);

HRESULT						billboard_draw(BILLBOARD*);
D3DXMATRIX*					billboard_get_matrix(BILLBOARD*);
SRT*						billboard_get_srt(BILLBOARD*);

//
// モデル情報
//
typedef struct _mdl {
	LPD3DXBUFFER			pMatBuf;
	LPD3DXMESH				pMesh;
	LPDIRECT3DTEXTURE9		pTex;
	DWORD					nMatNum;

	D3DXMATRIX				mtx;
	SRT						srt;
	
	//	バーチャルテーブル
	void*					lpVtbl;
} MODEL;

HRESULT						model_init(MODEL*, LPCWSTR);
void						model_uninit(MODEL*);

HRESULT						model_draw(MODEL*);
D3DXMATRIX*					model_get_matrix(MODEL*);
SRT*						model_get_srt(MODEL*);
void						model_update(MODEL*);

#endif // !__MODEL_H__
