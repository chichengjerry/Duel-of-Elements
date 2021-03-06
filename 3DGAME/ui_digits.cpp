//-----------------------------------------------
// [ui_digits.cpp]
// 数字のUI設定
// 作者 Chicheng Zhang
//-----------------------------------------------
#include "ui_digits.h"

#define	IMAGE_DIGIT		_T("data/TEXTURE/ui/digits.png")

DWORD				DIGITIMAGE::count = 0;
LPDIRECT3DTEXTURE9	DIGITIMAGE::pTex = NULL;

//
// コンストラクター
//
DIGITIMAGE::DIGITIMAGE(INT digit, D3DRECT* rect)
{
	count++;
	LoadTexture();
	image = new IMAGE(pTex, rect);
	image->nFrameIndex = 0;
	image->nFrameTotal = 10;
	image->SetTexture();
	SetDigit(digit);
}

//
// デストラクター
//
DIGITIMAGE::~DIGITIMAGE()
{
	SAFE_DELETE(image);
	if (!--count) {
		SAFE_RELEASE(pTex);
	}
}

//
// テクスチャのロード
//
HRESULT DIGITIMAGE::LoadTexture()
{
	LPDIRECT3DDEVICE9 pDevice = D3D::GetDevice();
	if (!pTex) {
		if (FAILED(D3DXCreateTextureFromFile(pDevice,			// デバイスへのポインタ
			IMAGE_DIGIT,										// ファイルの名前
			&pTex)))										// 読み込むメモリー
		{
			return E_FAIL;
		}
	}
	return S_OK;
}

//
// 数字の設定
//
void DIGITIMAGE::SetDigit(INT digit)
{
	if (image->nFrameIndex != digit) {
		image->nFrameIndex = digit;
		image->SetTexture();
	}
}

//
// 数字の描画
//
HRESULT DIGITIMAGE::Draw()
{
	return image->Draw();
}

//
// 数字の更新
//
void DIGITIMAGE::Update()
{
}
