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
// �ӥ�ܩ`�����
//
#define VTABLE BILLBOARDVTBL
struct VTABLE;

typedef struct _billboard {
	LPDIRECT3DVERTEXBUFFER9	pVtx;
	LPDIRECT3DTEXTURE9		pTex;

	D3DXMATRIX				mtx;
	SRT						srt;

	//	�Щ`�����Ʃ`�֥�
	VTABLE*				lpVtbl;
} BILLBOARD;

//
// �ӥ�ܩ`�ɤγ��ڻ���
//
HRESULT						billboard_init(BILLBOARD*, LPCWSTR);

//
// ��ǥ�ν�š�
//
void						billboard_uninit(BILLBOARD*);

//
// �ӥ�ܩ`�ɤ��軭���롣
//
HRESULT						billboard_draw(BILLBOARD*);

//
// ��ǥ�Υ�`��ɥޥȥ�å�����ȡ�ä��롣
//
D3DXMATRIX*					billboard_get_matrix(BILLBOARD*);

//
// ��ǥ�Υ���������ܞ��λ������ȡ�ä��롣
//
SRT*						billboard_get_srt(BILLBOARD*);

//
// ��ǥ����¤��롣
//
void						billboard_update(BILLBOARD*);

//
// �Щ`�����Ʃ`�֥�
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
// ��ǥ����
//
typedef struct _model {
	LPD3DXBUFFER			pMatBuf;
	LPD3DXMESH				pMesh;
	LPDIRECT3DTEXTURE9		pTex;
	DWORD					nMatNum;

	D3DXMATRIX				mtx;
	SRT						srt;
	
	//	�Щ`�����Ʃ`�֥�
	VTABLE*					lpVtbl;
} MODEL;

//
// ��ǥ�γ��ڻ���
//
HRESULT						model_init(MODEL*, LPCWSTR);

//
// ��ǥ�ν�š�
//
void						model_uninit(MODEL*);

//
// ��ǥ���軭���롣
//
HRESULT						model_draw(MODEL*);

//
// ��ǥ�Υ�`��ɥޥȥ�å�����ȡ�ä��롣
//
D3DXMATRIX*					model_get_matrix(MODEL*);

//
// ��ǥ������ȡ�ä��롣
//
SRT*						model_get_srt(MODEL*);

//
// ��ǥ����¤��롣
//
void						model_update(MODEL*);

//
// �Щ`�����Ʃ`�֥�
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
