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

TERRAIN::TERRAIN(DWORD depth)
{
	nMapSize = (1 << (depth * 2));
	nHeightMapSize = nMapSize + 1;
	nBlockSize = GM_TERRAIN_SIZE;

	pHeightMap = (BYTE*)malloc(sizeof(BYTE) * nHeightMapSize * nHeightMapSize);
	pField = new MESHFIELD(nMapSize, nMapSize, nBlockSize, nBlockSize, 0.0f, GM_MAX_HEIGHT);

	PERLIN perlin = PERLIN(0);
	DWORD seed = timeGetTime() % 255;
	FLOAT xf = 0.0f;
	FLOAT zf = 0.0f;
	COLORMAP colorMap = COLORMAP(sizeof(COLOR_STEPS) / sizeof(COLORSTEP), COLOR_STEPS);

	for (int z = 0; z < nHeightMapSize; z++) {
		for (int x = 0; x < nHeightMapSize; x++) {
			xf = (FLOAT)x / nMapSize * GM_TERRAIN_ENR + seed;
			zf = (FLOAT)z / nMapSize * GM_TERRAIN_ENR + seed;
			FLOAT height = perlin.OctavePerlin(xf, zf, 0.0f, 6, 1.0f);
			pHeightMap[z * nHeightMapSize + x] = (BYTE)(256 * height);
		}
	}

	// pField->SetTexture(_T("data/TEXTURE/rock.png"));
	pField->SetVertex(pHeightMap, &colorMap);
}

TERRAIN::~TERRAIN()
{
	SAFE_DELETE(pHeightMap);
	SAFE_DELETE(pField);
}

HRESULT TERRAIN::Draw(CAMERA* camera)
{
	D3D::EnableFog(D3DCOLOR_ARGB(255, 122, 145, 188));

	pField->Draw();

	D3D::DisableFog();

	return S_OK;
}

void TERRAIN::Update()
{
	pField->Update();
}

BOOL TERRAIN::OutOfBound(FLOAT x, FLOAT z)
{
	return pField->OutOfBound(x, z);
}

FLOAT TERRAIN::GetGroundHeight(FLOAT x, FLOAT z) {
	return pField->GetPositionHeight(x, z);
}
