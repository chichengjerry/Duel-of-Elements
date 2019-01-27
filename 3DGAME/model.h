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
// ビルボード情報
//
#define VTABLE BILLBOARDVTBL
struct VTABLE;

typedef struct _billboard {
	LPDIRECT3DVERTEXBUFFER9	pVtx;
	LPDIRECT3DTEXTURE9		pTex;

	D3DXMATRIX				mtx;
	SRT						srt;

	//	バーチャルテーブル
	VTABLE*				lpVtbl;
} BILLBOARD;

//
// ビルボードの初期化。
//
HRESULT						billboard_init(BILLBOARD*, LPCWSTR);

//
// モデルの解放。
//
void						billboard_uninit(BILLBOARD*);

//
// ビルボードを描画する。
//
HRESULT						billboard_draw(BILLBOARD*);

//
// モデルのワールドマトリックスを取得する。
//
D3DXMATRIX*					billboard_get_matrix(BILLBOARD*);

//
// モデルのサイズ、回転、位置情報を取得する。
//
SRT*						billboard_get_srt(BILLBOARD*);

//
// モデルを更新する。
//
void						billboard_update(BILLBOARD*);

//
// バーチャルテーブル
//
typedef struct VTABLE {
	HRESULT					(*init)(BILLBOARD*, LPCWSTR) = billboard_init;
	void					(*uninit)(BILLBOARD*) = billboard_uninit;

	HRESULT					(*draw)(BILLBOARD*) = billboard_draw;
	D3DXMATRIX*				(*get_matrix)(BILLBOARD*) = billboard_get_matrix;
	SRT*					(*get_srt)(BILLBOARD*) = billboard_get_srt;
	void					(*update)(BILLBOARD*) = billboard_update;
} VTABLE;

#undef VTABLE

#define VTABLE MODELVTBL
struct VTABLE;

//
// モデル情報
//
typedef struct _model {
	LPD3DXBUFFER			pMatBuf;
	LPD3DXMESH				pMesh;
	LPDIRECT3DTEXTURE9		pTex;
	DWORD					nMatNum;

	D3DXMATRIX				mtx;
	SRT						srt;
	
	//	バーチャルテーブル
	VTABLE*					lpVtbl;
} MODEL;

//
// モデルの初期化。
//
HRESULT						model_init(MODEL*, LPCWSTR);

//
// モデルの解放。
//
void						model_uninit(MODEL*);

//
// モデルを描画する。
//
HRESULT						model_draw(MODEL*);

//
// モデルのワールドマトリックスを取得する。
//
D3DXMATRIX*					model_get_matrix(MODEL*);

//
// モデルの情報を取得する。
//
SRT*						model_get_srt(MODEL*);

//
// モデルを更新する。
//
void						model_update(MODEL*);

//
// バーチャルテーブル
//
typedef struct VTABLE {
	HRESULT					(*init)(MODEL*, LPCWSTR) = model_init;
	void					(*uninit)(MODEL*) = model_uninit;
	HRESULT					(*draw)(MODEL*) = model_draw;
	D3DXMATRIX*				(*get_matrix)(MODEL*) = model_get_matrix;
	SRT*					(*get_srt)(MODEL*) = model_get_srt;
} VTABLE;

#undef VTABLE
#endif // !__MODEL_H__
