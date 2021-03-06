//-----------------------------------------------
// [explosion.cpp]
// 爆発効果処理
// 作者 Chicheng Zhang
//-----------------------------------------------
#include "explosion.h"
#include "camera.h"
#include "sound.h"

//
// 静的メンバーの初期化
//

LPDIRECT3DTEXTURE9	EXPLOSION::pTex[] = { NULL };

//
// コンストラクター
//
EXPLOSION::EXPLOSION(D3DXVECTOR3 pos)
{
	particle = new EMITTER(pTex[GET_RANDOM(0, (EXPLOSIVE_IMAGE_MAX - 1))],
		1, pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), EXPLOSIVE_SIZE);

	idx = 0;
	fTime = 0.0f;

	DSOUND::Play(SOUND_EXPLOSION);
}

//
// デストラクター
//
EXPLOSION::~EXPLOSION()
{
	SAFE_DELETE(particle);
}

//
// 効果を管理リストに入れる。
//
void EXPLOSION::Add(EXPLOSION * pExplosion[], D3DXVECTOR3 pos)
{
	for (int i = 0; i < GM_MAX_EXPLOSION; i++) {
		if (!pExplosion[i]) {
			pExplosion[i] = new EXPLOSION(pos);
			break;
		}
	}
}

//
// テクスチャのロード。
//
void EXPLOSION::LoadTextures()
{
	for (int i = 0; i < EXPLOSIVE_IMAGE_MAX; i++) {
		WCHAR str[256];
		swprintf_s(str, EXPLOSIVE_IMAGE, i);
		D3D::LoadTexture(&pTex[i], str);
	}
}

//
// テクスチャの資源の解放
//
void EXPLOSION::ReleaseTextures()
{
	for (int i = 0; i < EXPLOSIVE_IMAGE_MAX; i++) {
		SAFE_RELEASE(pTex[i]);
	}
}

//
// 爆発イメージの更新
//
void EXPLOSION::Update()
{
	if (idx >= EXPLOSIVE_FRAME_TOTAL - 1) {
		idx = EXPLOSIVE_FRAME_TOTAL - 1;
		particle->bAlive = FALSE;
	}
	else {
		fTime += 64 * D3D::fAnimationRate;
		idx = (INT)floorf(fTime);

		int x = idx % EXPLOSIVE_FRAME_X;
		int y = idx / EXPLOSIVE_FRAME_Y;

		FLOAT sx = 1.0f / EXPLOSIVE_FRAME_X;
		FLOAT sy = 1.0f / EXPLOSIVE_FRAME_Y;

		D3DXVECTOR2 UV[4] = {
			D3DXVECTOR2(x * sx, y * sy),
			D3DXVECTOR2((x + 1) * sx, y * sy),
			D3DXVECTOR2(x * sx, (y + 1) *sy),
			D3DXVECTOR2((x + 1) * sx, (y + 1) * sy),
		};
		particle->SetTexture(0, UV);

		particle->Update();
	}
}

//
// 爆発効果を描画する。
//
HRESULT EXPLOSION::Draw(CAMERA* pCamera)
{
	return particle->Draw(pCamera);
}
