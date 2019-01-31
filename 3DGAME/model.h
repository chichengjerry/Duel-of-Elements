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

#define	NUM_VERTEX			4		// 픵���
#define	NUM_POLYGON			2		// �ݥꥴ����

// ���ĥݥꥴ��픵�ե��`�ޥå�( 픵�����[2D] / ����� / �ƥ����������� )
#define	FVF_VERTEX_2D		(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

typedef struct _vertex_2d {
	D3DXVECTOR3				vtx;		// 픵�����
	FLOAT					rhw;		// �ƥ�������Υѩ`���ڥ��ƥ��֥��쥯����
	D3DCOLOR				dif;		// �����
	D3DXVECTOR2				tex;		// �ƥ�����������
} VERTEX_2D;

// ���ĥݥꥴ��픵�ե��`�ޥå�( 픵�����[3D] / ���� / ����� / �ƥ����������� )
#define	FVF_VERTEX_3D		(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

typedef struct _vertex_3d {
	D3DXVECTOR3				vtx;		// 픵�����
	D3DXVECTOR3				nor;		// �����٥��ȥ�
	D3DCOLOR				dif;		// �����
	D3DXVECTOR2				tex;		// �ƥ�����������
} VERTEX_3D;

//
// λ�á�����������ܞ���
//
typedef struct SRT {
	D3DXVECTOR3				pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3				rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3				scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
} SRT;

//
// �ݥꥴ�����
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
// ��ǥ����
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
