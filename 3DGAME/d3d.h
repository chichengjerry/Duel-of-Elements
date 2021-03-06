//-----------------------------------------------
// [d3d.h]
// DirectX処理
// 作者 Chicheng Zhang
//-----------------------------------------------
#ifndef __D3D_H__
#define __D3D_H__

#include "main.h"
#include "core.h"
#include "util.h"

struct D3D {
	static LPDIRECT3D9			pD3D;								// Direct3D オブジェクト
	static LPDIRECT3DDEVICE9	pDevice;							// Deviceオブジェクト(描画に必要)
	static LPD3DXFONT			pD3DXFont;							// フォントへのポインタ
	static FLOAT				fAnimationRate;
	static DWORD				dwTime;

	static HRESULT				Init(HINSTANCE hInst, HWND hWnd);
	static void					Destroy(void);
	static HRESULT				Render(void);
	static HRESULT				Update(void);

	static LPDIRECT3DDEVICE9	GetDevice(void);
	static HRESULT				LoadTexture(LPDIRECT3DTEXTURE9* ppTex, LPCWSTR path);
	static HRESULT				ShowText(LPCWSTR text, LONG left, LONG top);

	static HRESULT				EnableFog(DWORD fogColor);
	static HRESULT				DisableFog();
};

#endif // ! __D3D_H__