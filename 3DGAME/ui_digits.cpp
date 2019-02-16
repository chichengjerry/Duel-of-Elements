#include "ui_digits.h"

#define	IMAGE_DIGIT		_T("data/TEXTURE/ui/digits.png")

DWORD				DIGITIMAGE::count = 0;
LPDIRECT3DTEXTURE9	DIGITIMAGE::pTex = NULL;

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

DIGITIMAGE::~DIGITIMAGE()
{
	SAFE_DELETE(image);
	if (!--count) {
		SAFE_RELEASE(pTex);
	}
}

HRESULT DIGITIMAGE::LoadTexture()
{
	LPDIRECT3DDEVICE9 pDevice = D3D::GetDevice();
	if (!pTex) {
		if (FAILED(D3DXCreateTextureFromFile(pDevice,			// �f�o�C�X�ւ̃|�C���^
			IMAGE_DIGIT,										// �t�@�C���̖��O
			&pTex)))										// �ǂݍ��ރ������[
		{
			return E_FAIL;
		}
	}
	return S_OK;
}

void DIGITIMAGE::SetDigit(INT digit)
{
	if (image->nFrameIndex != digit) {
		image->nFrameIndex = digit;
		image->SetTexture();
	}
}

HRESULT DIGITIMAGE::Draw()
{
	return image->Draw();
}

void DIGITIMAGE::Update()
{
}
