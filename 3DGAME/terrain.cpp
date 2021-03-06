//-----------------------------------------------
// [terrain.cpp]
// 地形処理
// 作者 Chicheng Zhang
//-----------------------------------------------
#include "terrain.h"
#include "camera.h"
#include "player.h"
#include "perlin.h"

COLORSTEP COLOR_STEPS[] = {
	{ D3DXCOLOR(D3DCOLOR_ARGB(255, 0, 94, 21)), 0.3f },
	{ D3DXCOLOR(D3DCOLOR_ARGB(255, 99, 140, 11)), 0.4f },
	{ D3DXCOLOR(D3DCOLOR_ARGB(255, 106, 57, 6)), 0.5f },
	{ D3DXCOLOR(D3DCOLOR_ARGB(255, 255, 255, 255)), 0.6f },
};

//
// 地形を初期化する。
//
TERRAIN::TERRAIN(DWORD depth)
{
	nMapSize = (1 << (depth * 2));
	nHeightMapSize = nMapSize + 1;
	nBlockSize = GM_TERRAIN_SIZE;

	// パーリンノイズを使って高度図を生成する。
	pHeightMap = (BYTE*)malloc(sizeof(BYTE) * nHeightMapSize * nHeightMapSize);
	pField = new MESHFIELD(nMapSize, nMapSize, nBlockSize, nBlockSize, 0.0f, GM_MAX_HEIGHT);

	PERLIN perlin = PERLIN(0);
	DWORD seed = timeGetTime() % 255;
	FLOAT dx = 0.0f;
	FLOAT dz = 0.0f;
	COLORMAP colorMap = COLORMAP(sizeof(COLOR_STEPS) / sizeof(COLORSTEP), COLOR_STEPS);

	for (int z = 0; z < nHeightMapSize; z++) {
		for (int x = 0; x < nHeightMapSize; x++) {
			dx = (FLOAT)x / nMapSize * GM_TERRAIN_ENR + seed;
			dz = (FLOAT)z / nMapSize * GM_TERRAIN_ENR + seed;
			FLOAT height = perlin.OctavePerlin(dx, dz, 0.0f, 6, 1.0f);
			pHeightMap[z * nHeightMapSize + x] = (BYTE)(256 * height);
		}
	}

	pField->SetTexture(_T("data/TEXTURE/rock.jpg"));

	pField->SetVertex(pHeightMap, &colorMap);
}

//
// リソースの解放
//
TERRAIN::~TERRAIN()
{
	SAFE_DELETE(pHeightMap);
	SAFE_DELETE(pField);
}

//
// 地面を描画する。
//
HRESULT TERRAIN::Draw(CAMERA* camera)
{
	pField->Draw();

	return S_OK;
}

//
// 地面のデータを更新する。
//
void TERRAIN::Update()
{
	pField->Update();
}

//
// 既定の座標で地形の高さを求める。
// メッシュフィールドの方法と同じ。
//
FLOAT TERRAIN::GetGroundHeight(FLOAT x, FLOAT z) {
	return pField->GetPositionHeight(x, z);
}

//
// 座標は地形の範囲内であるかをチェックする。
// メッシュフィールドの方法と同じ。
//
BOOL TERRAIN::IsOutofBound(FLOAT x, FLOAT z)
{
	return pField->IsOutofBound(x, z);
}
