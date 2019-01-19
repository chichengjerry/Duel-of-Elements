/***********************************************
 * [d3d.h]
 * DirectX処理
 * GP11A341 24 張馳騁
 ***********************************************/
#ifndef __D3D_H__
#define __D3D_H__

#include "main.h"
#include "core.h"

/***********************************************
 * マクロ定義
 ***********************************************/

#define SAFE_RELEASE(p)			if(p){ (p)->Release(); (p) = NULL; }

struct D3DVTBL;

typedef struct _D3D {
	LPDIRECT3D9					pD3D = NULL;			// Direct3D オブジェクト
	LPDIRECT3DDEVICE9			pDevice = NULL;	// Deviceオブジェクト(描画に必要)

	D3DVTBL*					lpVtbl = NULL;
} D3D, *LPD3D;

/***********************************************
 * 関数宣言
 ***********************************************/

void							D3DDestroy(LPD3D);
LPDIRECT3DDEVICE9				D3DGetDevice(LPD3D);
HRESULT							D3DInit(LPD3D, HINSTANCE, HWND);
HRESULT							D3DRender(LPD3D);
void							D3DUpdate(LPD3D);

struct D3DVTBL {
	void						(*Destroy)(LPD3D) = D3DDestroy;
	LPDIRECT3DDEVICE9			(*GetDevice)(LPD3D) = D3DGetDevice;
	HRESULT						(*Init)(LPD3D, HINSTANCE, HWND) = D3DInit;
	HRESULT						(*Render)(LPD3D) = D3DRender;
	void						(*Update)(LPD3D) = D3DUpdate;
};

#define	D3dDestroy(p)			(p)->lpVtbl->Destroy(p)
#define	D3dGetDevice(p)			(p)->lpVtbl->GetDevice(p)
#define	D3dRender(p)			(p)->lpVtbl->Render(p)
#define	D3dUpdate(p)			(p)->lpVtbl->Update(p)

#endif // ! __D3D_H__