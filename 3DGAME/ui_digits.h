//-----------------------------------------------
// [ui_digits.h]
// 数字のUI設定
// 作者 Chicheng Zhang
//-----------------------------------------------
#ifndef __UI_DIGITS_H__
#define __UI_DIGITS_H__

#include "main.h"
#include "image.h"

typedef struct DIGITIMAGE {
	static DWORD				count;
	static LPDIRECT3DTEXTURE9	pTex;

	IMAGE*						image;
	INT							digit;
	D3DXCOLOR					col;

	DIGITIMAGE(INT digit, D3DRECT* rect);
	~DIGITIMAGE();

	HRESULT						Draw();
	void						Update();

	HRESULT						LoadTexture();
	void						SetDigit(INT digit);
} DIGITIMAGE;


#endif // !__UI_DIGITS_H__
