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
typedef struct _srt {
	D3DXVECTOR3				pos;
	D3DXVECTOR3				rot;
	D3DXVECTOR3				scl;
} SRT;

void						srt_init(SRT*);

//
// �v�����`�Щ`�饤��
//

#define GAME_INIT(p)		(p)->lpVtbl->init(p)
#define GAME_UNINIT(p)		(p)->lpVtbl->uninit(p)

#define GAME_DRAW(p)		(p)->lpVtbl->draw(p)
#define GAME_GETMATRIX(p)	(p)->lpVtbl->get_matrix(p)
#define GAME_GETSRT(p)		(p)->lpVtbl->get_srt(p)
#define GAME_UPDATE(p)		(p)->lpVtbl->update(p)

//
// �ӥ�ܩ`�����
//
typedef struct _billboard {
	LPDIRECT3DVERTEXBUFFER9	pVtx;
	LPDIRECT3DTEXTURE9		pTex;

	D3DXMATRIX				mtx;
	SRT						srt;

	//	�Щ`�����Ʃ`�֥�
	void*					lpVtbl;
} BILLBOARD;

HRESULT						billboard_init(BILLBOARD*, LPCWSTR);
void						billboard_uninit(BILLBOARD*);

HRESULT						billboard_draw(BILLBOARD*);
D3DXMATRIX*					billboard_get_matrix(BILLBOARD*);
SRT*						billboard_get_srt(BILLBOARD*);

//
// ��ǥ����
//
typedef struct _mdl {
	LPD3DXBUFFER			pMatBuf;
	LPD3DXMESH				pMesh;
	LPDIRECT3DTEXTURE9		pTex;
	DWORD					nMatNum;

	D3DXMATRIX				mtx;
	SRT						srt;
	
	//	�Щ`�����Ʃ`�֥�
	void*					lpVtbl;
} MODEL;

HRESULT						model_init(MODEL*, LPCWSTR);
void						model_uninit(MODEL*);

HRESULT						model_draw(MODEL*);
D3DXMATRIX*					model_get_matrix(MODEL*);
SRT*						model_get_srt(MODEL*);
void						model_update(MODEL*);

#endif // !__MODEL_H__
