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
typedef struct SRT {
	D3DXVECTOR3				pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3				rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3				scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
} SRT;

//
// ポリゴン情報
//
typedef struct POLYGON {
	LPDIRECT3DVERTEXBUFFER9	pVtx;
	LPDIRECT3DTEXTURE9		pTex;

	D3DXMATRIX				mtx;
	SRT						srt;

	BOOL					isBillboard;

	POLYGON(LPCWSTR src, SRT srt, BOOL isBillboard);
	~POLYGON();

	HRESULT					Draw(CAMERA* pCamera);
	D3DXMATRIX*				GetMatrix();
	SRT*					GetSrt();
	void					Update();
} POLYGON;

//
// モデル情報
//
typedef struct MODEL {
	LPD3DXBUFFER			pMatBuf;
	LPD3DXMESH				pMesh;
	LPDIRECT3DTEXTURE9		pTex;
	DWORD					nMatNum;

	D3DXMATRIX				mtx;
	SRT						srt;

	MODEL(LPCWSTR src, SRT srt);
	~MODEL();

	HRESULT					Draw(CAMERA* pCamera);
	D3DXMATRIX*				GetMatrix();
	SRT*					GetSrt();
	void					Update();
} MODEL;

#endif // !__MODEL_H__
