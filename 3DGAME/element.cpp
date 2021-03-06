//-----------------------------------------------
// [element.cpp]
// エレメントの処理
// 作者 Chicheng Zhang
//-----------------------------------------------
#include "element.h"
#include "camera.h"
#include "player.h"
#include "sound.h"
#include "terrain.h"

#define ELEMENT_ROTATION
#define ELEMENT_GLOW			_T("data/TEXTURE/glow01.jpg")
#define	ELEMENT_GLOW_SIZE		60
#define	REFRESH_RATE			5000

LPCWSTR ElementModels[4] = {
	_T("data/MODEL/hexahedron.x"),
	_T("data/MODEL/icosahedron.x"),
	_T("data/MODEL/tetrahedron.x"),
	_T("data/MODEL/octahedron.x")
};

//
// エレメントとグローの色
//
D3DXCOLOR ElementColor[] = {
	D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
	D3DXCOLOR(0.0f, 0.5f, 0.0f, 1.0f),
	D3DXCOLOR(0.1f, 0.625f, 0.625f, 1.0f),
	D3DXCOLOR(1.0f, 0.5f, 0.25f, 1.0f),
	D3DXCOLOR(0.75f, 1.0f, 1.0f, 1.0f)
};

//
// 相互作用によってダメージの設定。
//
int DAMAGE_TABLE[ELEMENTTYPE_NUM][ELEMENTTYPE_NUM] = {
	/* def\atk */ /* NONE  */ /* EARTH */ /* WATER */ /* FIRE  */ /* AIR   */
	/* NONE  */	{		0	,		10	,		10	,		10	,		10	 },
	/* EARTH */	{		0	,		0	,		5	,		15	,		20	 },
	/* WATER */	{		0	,		5	,		0	,		20	,		15	 },
	/* FIRE  */	{		0	,		15	,		20	,		0	,		5	 },
	/* AIR   */	{		0	,		20	,		15	,		5	,		0	 },
};

//
// 相互作用によって得られるスコアの設定。
//
int SCORE_TABLE[ELEMENTTYPE_NUM][ELEMENTTYPE_NUM] = {
	/* cur\get */ /* NONE  */ /* EARTH */ /* WATER */ /* FIRE  */ /* AIR   */
	/* NONE  */	{		0	,		50	,		50	,		50	,		50	 },
	/* EARTH */	{		0	,		0	,		100	,		100	,		300	 },
	/* WATER */	{		0	,		100	,		0	,		300	,		100	 },
	/* FIRE  */	{		0	,		100	,		300	,		0	,		100	 },
	/* AIR   */	{		0	,		300	,		100	,		100	,		0	 },
};

inline void SetElement(ELEMENT* el) {

	el->elementType = (ELEMENTTYPE)GET_RANDOM(ELEMENTTYPE_EARTH, ELEMENTTYPE_AIR);
	el->SetPosition(NULL);

	el->mdl = new MODEL(ElementModels[el->elementType - 1], NULL);
	el->emitter = new EMITTER(el->pTex, 1, el->srt.pos,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		ElementColor[el->elementType],
		ELEMENT_GLOW_SIZE);
};

//
// 静的メンバーの初期化
//
LPDIRECT3DTEXTURE9	ELEMENT::pTex = NULL;

//
// コンストラクター
//
ELEMENT::ELEMENT(TERRAIN* pTerrain)
{
	mdl = NULL;
	emitter = NULL;
	dwRefreshTimer = NULL;
	terrain = pTerrain;

	SetElement(this);
}

//
// デストラクター
//
ELEMENT::~ELEMENT()
{
	SAFE_DELETE(mdl);
	SAFE_DELETE(emitter);
}

//
// テスクチャをロードする。
//
void ELEMENT::LoadTexture()
{
	D3D::LoadTexture(&pTex, ELEMENT_GLOW);
}

//
// テクスチャの資源を解放する。
//
void ELEMENT::ReleaseTexture()
{
	SAFE_RELEASE(pTex);
}

//
// 新しいエレメントを設置する。
//
void ELEMENT::RefreshElement(DWORD mHealthEasing)
{
	if (mHealthEasing) {
		dwRefreshTimer = timeGetTime();
		SAFE_DELETE(mdl);
		SAFE_DELETE(emitter);
	}
	else {
		DWORD newTimer = timeGetTime();

		if (newTimer - dwRefreshTimer >= REFRESH_RATE) {
			SetElement(this);
			// Refresh Element
			dwRefreshTimer = NULL;
		}
	}
}

//
// エレメントのポジションを設置する。
//
void ELEMENT::SetPosition(D3DXVECTOR3 * pos)
{
	if (pos) {
		srt = *pos;
	}
	else {
		FLOAT spawnX = (GET_RANDOM(terrain->nMapSize / 4, terrain->nMapSize * 3 / 4) - terrain->nMapSize / 2.0f) * terrain->nBlockSize;
		FLOAT spawnZ = (GET_RANDOM(terrain->nMapSize / 4, terrain->nMapSize * 3 / 4) - terrain->nMapSize / 2.0f) * terrain->nBlockSize;
		FLOAT spawnY = terrain->GetGroundHeight(spawnX, spawnZ);
		//spawnY = (GM_MAX_HEIGHT - spawnY) * elementType * 0.2f + spawnY;
		
		spawnY = GET_RANDOM(spawnY * 1.25f, GM_MAX_HEIGHT);
		srt = SRT(D3DXVECTOR3(spawnX, spawnY, spawnZ));
	}
}

//
// エレメントの状態を更新する。
//
void ELEMENT::Update(PLAYER* player[])
{
	if (dwRefreshTimer) {
		RefreshElement(NULL);
	}
	else {
		for (int i = 0; i < GM_MAX_PLAYER; i++) {
			D3DXVECTOR3 dist = player[i]->srt.pos - srt.pos;
			if (D3DXVec3LengthSq(&dist) < ELEMENT_GLOW_SIZE * ELEMENT_GLOW_SIZE / 4) {
				
				DWORD newScore = player[i]->nScore + SCORE_TABLE[player[i]->defenseType][elementType];

				if (player[i]->defenseType + elementType == ELEMENTTYPE_NUM)
					player[i]->defenseType = ELEMENTTYPE_NONE;
				else
					player[i]->attackType = elementType;

				player[i]->UpdateScore(&newScore);

				// Pickup
				DSOUND::Play(SOUND_PICKUP);
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

//
// エレメントを描画する。
//
HRESULT ELEMENT::Draw(CAMERA* camera)
{
	D3DXVECTOR3 dist = camera->v_pos - srt.pos;
	if (!dwRefreshTimer && D3DXVec3LengthSq(&dist) < GR_VISIBLE_DIST) {
		emitter->Draw(camera);
		mdl->Draw(NULL);
	}

	return S_OK;
}
