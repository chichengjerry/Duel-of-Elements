#include "element.h"
#include "player.h"

#define ELEMENT_ROTATION
#define ELEMENT_GLOW			_T("data/TEXTURE/glow01.jpg")
#define	ELEMENT_GLOW_SIZE		20
#define	REFRESH_RATE			5000

LPCWSTR ElementModels[4] = {
	_T("data/MODEL/hexahedron.x"),
	_T("data/MODEL/icosahedron.x"),
	_T("data/MODEL/tetrahedron.x"),
	_T("data/MODEL/octahedron.x")
};

D3DXCOLOR ElementColor[4] = {
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

	FLOAT x = (FLOAT)(GET_RANDOM(1, terrain->dwMapSize - 1) * GM_TERRAIN_SIZE);
	FLOAT z = (FLOAT)(GET_RANDOM(1, terrain->dwMapSize - 1) * GM_TERRAIN_SIZE);
	srt = SRT(D3DXVECTOR3(x, terrain->GetGroundHeight(x, z) + elementType * 20.0f, z));

	mdl = new MODEL(ElementModels[elementType - 1], NULL);
	emitter = new EMITTER(ELEMENT_GLOW, 1, srt.pos,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		ElementColor[elementType - 1],
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
			FLOAT x = (FLOAT)(GET_RANDOM(1, terrain->dwMapSize - 1) * GM_TERRAIN_SIZE);
			FLOAT z = (FLOAT)(GET_RANDOM(1, terrain->dwMapSize - 1) * GM_TERRAIN_SIZE);

			srt = SRT(D3DXVECTOR3(x, terrain->GetGroundHeight(x, z) + elementType * 20.0f, z));

			mdl = new MODEL(ElementModels[elementType - 1], NULL);
			emitter = new EMITTER(ELEMENT_GLOW, 1, srt.pos,
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				ElementColor[elementType - 1],
				ELEMENT_GLOW_SIZE);
		}
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
				player[i]->attackType = elementType;
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

HRESULT ELEMENT::Draw(PLAYER* player)
{
	D3DXVECTOR3 dist = player->srt.pos - srt.pos;
	if (!dwRefreshTimer && D3DXVec3LengthSq(&dist) < GR_FAR_Z * GR_FAR_Z) {
		emitter->Draw(player->camera);
		mdl->Draw(NULL);
	}

	return S_OK;
}
