#include "element.h"
#include "camera.h"
#include "player.h"
#include "terrain.h"

#define ELEMENT_ROTATION
#define ELEMENT_GLOW			_T("data/TEXTURE/glow01.jpg")
#define	ELEMENT_GLOW_SIZE		40
#define	REFRESH_RATE			5000

LPCWSTR ElementModels[4] = {
	_T("data/MODEL/hexahedron.x"),
	_T("data/MODEL/icosahedron.x"),
	_T("data/MODEL/tetrahedron.x"),
	_T("data/MODEL/octahedron.x")
};

D3DXCOLOR ElementColor[] = {
	D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
	D3DXCOLOR(0.0f, 0.5f, 0.0f, 1.0f),
	D3DXCOLOR(0.1f, 0.625f, 0.625f, 1.0f),
	D3DXCOLOR(1.0f, 0.5f, 0.25f, 1.0f),
	D3DXCOLOR(0.75f, 1.0f, 1.0f, 1.0f)
};

ELEMENT::ELEMENT(TERRAIN* pTerrain)
{
	mdl = NULL;
	emitter = NULL;
	dwRefreshTimer = NULL;
	terrain = pTerrain;

	elementType = (ELEMENTTYPE)GET_RANDOM(ELEMENTTYPE_EARTH, ELEMENTTYPE_AIR);
	SetPosition(NULL);

	mdl = new MODEL(ElementModels[elementType - 1], NULL);
	emitter = new EMITTER(ELEMENT_GLOW, 1, srt.pos,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		ElementColor[elementType],
		ELEMENT_GLOW_SIZE);
}

ELEMENT::~ELEMENT()
{
	SAFE_DELETE(mdl);
	SAFE_DELETE(emitter);
}

void ELEMENT::RefreshElement(DWORD mScoreEasing)
{
	if (mScoreEasing) {
		dwRefreshTimer = timeGetTime();
		SAFE_DELETE(mdl);
		SAFE_DELETE(emitter);
	}
	else {
		DWORD newTimer = timeGetTime();

		if (newTimer - dwRefreshTimer >= REFRESH_RATE) {
			dwRefreshTimer = NULL;
			// Refresh Element
			elementType = (ELEMENTTYPE)GET_RANDOM(ELEMENTTYPE_EARTH, ELEMENTTYPE_AIR);
			SetPosition(NULL);

			mdl = new MODEL(ElementModels[elementType - 1], NULL);
			emitter = new EMITTER(ELEMENT_GLOW, 1, srt.pos,
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				ElementColor[elementType],
				ELEMENT_GLOW_SIZE);
		}
	}
}

void ELEMENT::SetPosition(D3DXVECTOR3 * pos)
{
	if (pos) {
		srt = *pos;
	}
	else {
		FLOAT spawnX = (GET_RANDOM(terrain->nMapSize / 4, terrain->nMapSize * 3 / 4) - terrain->nMapSize / 2.0f) * terrain->nBlockSize;
		FLOAT spawnZ = (GET_RANDOM(terrain->nMapSize / 4, terrain->nMapSize * 3 / 4) - terrain->nMapSize / 2.0f) * terrain->nBlockSize;
		FLOAT spawnY = terrain->GetGroundHeight(spawnX, spawnZ);
		spawnY = (GM_MAX_HEIGHT - spawnY) * elementType * 0.2f + spawnY;
		srt = SRT(D3DXVECTOR3(spawnX, spawnY, spawnZ));
	}
}

void ELEMENT::Update(PLAYER* player[])
{
	if (dwRefreshTimer) {
		RefreshElement(NULL);
	}
	else {
		for (int i = 0; i < GM_MAX_PLAYER; i++) {
			D3DXVECTOR3 dist = player[i]->srt.pos - srt.pos;
			if (D3DXVec3LengthSq(&dist) < ELEMENT_GLOW_SIZE * ELEMENT_GLOW_SIZE / 4) {
				DWORD newScore = player[i]->dwScore + 100;
				player[i]->elementType = elementType;
				player[i]->UpdateScore(&newScore);

				RefreshElement(timeGetTime());
				break;
			}
			else {
				srt.rot.y += D3DXToRadian(1.0f);
				srt.rot.x += D3DXToRadian(1.0f);
				srt.rot.z += D3DXToRadian(1.0f);
				mdl->srt = srt;
				mdl->Update();
				emitter->Update();
			}
		}
	}
}

HRESULT ELEMENT::Draw(CAMERA* camera)
{
	D3DXVECTOR3 dist = camera->v_pos - srt.pos;
	if (!dwRefreshTimer && D3DXVec3LengthSq(&dist) < GR_VISIBLE_DIST) {
		emitter->Draw(camera);
		mdl->Draw(NULL);
	}

	return S_OK;
}
