/***********************************************
 * [model.h]
 * ��ǥ�I��
 * GP11A341 24 ���Y�G
 ***********************************************/

#ifndef __MODEL_H__
#define __MODEL_H__

#include "main.h"
#include "core.h"
#include "util.h"

//
// �ݥꥴ�����
//
typedef struct POLYGON {
	LPDIRECT3DVERTEXBUFFER9	pVtx;
	LPDIRECT3DTEXTURE9		pTex;
	
	D3DXCOLOR				col;
	D3DXVECTOR3				vtx[NUM_VERTEX];
	D3DXMATRIX				mtx;
	SRT						srt;

	BOOL					isBillboard;

	POLYGON(LPCWSTR src, D3DXVECTOR3 vertices[]);
	~POLYGON();

	HRESULT					SetVertex(LPDIRECT3DDEVICE9 pDevice);
	HRESULT					Draw(CAMERA* pCamera);
	D3DXMATRIX*				GetMatrix();
	SRT*					GetSrt();
	void					Update();
} POLYGON;

//
// ��ǥ����
//
typedef struct MODEL {
	DWORD					nMatNum;
	LPD3DXBUFFER			pMatBuf;
	LPD3DXMESH				pMesh;
	LPDIRECT3DTEXTURE9		pTex;

	D3DXMATRIX				mtx;
	SRT						srt;
	D3DXCOLOR				col;

	MODEL(LPCWSTR modelSrc, LPCWSTR texSrc);
	~MODEL();

	HRESULT					LoadModel(LPCWSTR modelSrc, LPCWSTR texSrc);
	HRESULT					Draw();
	D3DXMATRIX*				GetMatrix();
	SRT*					GetSrt();
	void					Update();
} MODEL;

#endif // !__MODEL_H__
