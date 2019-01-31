#include "game.h"

#define MAP_SIZE	25

#define ELEMENT_SPAWN	0x0010
#define PLAYER_SPAWN	0x0100
#define MAX_SPAWN		4

#define EL	ELEMENT_SPAWN | 1
#define PL	PLAYER_SPAWN | 1

DWORD mapData[MAP_SIZE][MAP_SIZE] = {
	{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ -1,  1,  1,  1,  1,  1,  1,  1,  1,  1, -1, -1, -1, -1, -1,  1,  1,  1,  1,  1,  1,  1,  1,  1, -1 },
	{ -1,  1, PL,  1,  1,  1,  1,  1, EL,  1, -1, -1, -1, -1, -1,  1, EL,  1,  1,  1,  1,  1, PL,  1, -1 },
	{ -1,  1,  1,  1,  1,  1,  1,  1,  1,  1, -1, -1, -1, -1, -1,  1,  1,  1,  1,  1,  1,  1,  1,  1, -1 },
	{ -1,  1,  1,  1, -1, -1, -1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1, -1, -1, -1,  1,  1,  1, -1 },
	{ -1,  1,  1,  1, -1, -1, -1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1, -1, -1, -1,  1,  1,  1, -1 },
	{ -1,  1,  1,  1, -1, -1, -1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1, -1, -1, -1,  1,  1,  1, -1 },
	{ -1,  1,  1,  1,  1,  1,  1,  1,  1,  1, -1, -1, -1, -1, -1,  1,  1,  1,  1,  1,  1,  1,  1,  1, -1 },
	{ -1,  1, EL,  1,  1,  1,  1,  1, EL,  1, -1, -1, -1, -1, -1,  1, EL,  1,  1,  1,  1,  1, EL,  1, -1 },
	{ -1,  1,  1,  1,  1,  1,  1,  1,  1,  1, -1, -1, -1, -1, -1,  1,  1,  1,  1,  1,  1,  1,  1,  1, -1 },
	{ -1, -1, -1, -1,  1,  1,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1,  1,  1, -1, -1, -1, -1 },
	{ -1, -1, -1, -1,  1,  1,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1,  1,  1, -1, -1, -1, -1 },
	{ -1, -1, -1, -1,  1,  1,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1,  1,  1, -1, -1, -1, -1 },
	{ -1, -1, -1, -1,  1,  1,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1,  1,  1, -1, -1, -1, -1 },
	{ -1, -1, -1, -1,  1,  1,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1,  1,  1, -1, -1, -1, -1 },
	{ -1,  1,  1,  1,  1,  1,  1,  1,  1,  1, -1, -1, -1, -1, -1,  1,  1,  1,  1,  1,  1,  1,  1,  1, -1 },
	{ -1,  1, EL,  1,  1,  1,  1,  1, EL,  1, -1, -1, -1, -1, -1,  1,  1, EL,  1,  1,  1,  1, EL,  1, -1 },
	{ -1,  1,  1,  1,  1,  1,  1,  1,  1,  1, -1, -1, -1, -1, -1,  1,  1,  1,  1,  1,  1,  1,  1,  1, -1 },
	{ -1,  1,  1,  1, -1, -1, -1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1, -1, -1, -1,  1,  1,  1, -1 },
	{ -1,  1,  1,  1, -1, -1, -1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1, -1, -1, -1,  1,  1,  1, -1 },
	{ -1,  1,  1,  1, -1, -1, -1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1, -1, -1, -1,  1,  1,  1, -1 },
	{ -1,  1,  1,  1,  1,  1,  1,  1,  1,  1, -1, -1, -1, -1, -1,  1,  1,  1,  1,  1,  1,  1,  1,  1, -1 },
	{ -1,  1, PL,  1,  1,  1,  1,  1, EL,  1, -1, -1, -1, -1, -1,  1,  1, EL,  1,  1,  1,  1, PL,  1, -1 },
	{ -1,  1,  1,  1,  1,  1,  1,  1,  1,  1, -1, -1, -1, -1, -1,  1,  1,  1,  1,  1,  1,  1,  1,  1, -1 },
	{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }
};

#undef EL
#undef PL

typedef struct POS {
	int x;
	int z;
} POS;

MAINGAME::MAINGAME(INT nPlayer)
{
	srand((unsigned)time(NULL));

	map = new MAP(MAP_SIZE, MAP_SIZE, (DWORD*)mapData);
	light = new LIGHT();

	POS spawnpoint[MAX_SPAWN] = {};
	D3DXVECTOR3 offset = D3DXVECTOR3(-(FLOAT)(MAP_SIZE * NODE_LENGTH) / 2.0f, 0.0f, -(FLOAT)(MAP_SIZE * NODE_LENGTH) / 2.0f);

	for (int spawned = 0, z = 0; z < MAP_SIZE; z++) {
		for (int x = 0; x < MAP_SIZE; x++) {
			if (PLAYER_SPAWN & mapData[z][x] && spawned < MAX_SPAWN) {
				spawnpoint[spawned++] = { x, z };
			}
			SRT srt = SRT();

			D3DXVECTOR3 vtx[4] = {
				D3DXVECTOR3(x * NODE_LENGTH,		0.0f, z * NODE_LENGTH) + offset,
				D3DXVECTOR3((x + 1) * NODE_LENGTH,	0.0f, z * NODE_LENGTH) + offset,
				D3DXVECTOR3(x * NODE_LENGTH,		0.0f, (z + 1) * NODE_LENGTH) + offset,
				D3DXVECTOR3((x + 1) * NODE_LENGTH,	0.0f, (z + 1) * NODE_LENGTH) + offset,
			};

			map->ground[z * MAP_SIZE + x] = new GROUND(vtx);
		}
	}

	int r;

	r = GET_RANDOM(0, MAX_SPAWN - 1);

	for (int i = 0; i < MAX_PLAYER; i++) {
		
		D3DXVECTOR3 pos = D3DXVECTOR3(spawnpoint[r].x * (NODE_LENGTH / 2), 0.0f, spawnpoint[r].z * (NODE_LENGTH / 2)) + offset;
		
		player[i] = new PLAYER(i, (BOOL)(i < nPlayer), pos);
		
		r = (r + 2) % MAX_SPAWN;
	}
}

MAINGAME::~MAINGAME()
{
	for (int i = 0; i < MAX_PLAYER; i++) {
		SAFE_DELETE(player[i]);
	}
	SAFE_DELETE(map);
	SAFE_DELETE(light);
}

void MAINGAME::Update(void)
{
	map->Update();
	for (int i = 0; i < MAX_PLAYER; i++) {
		player[i]->Update(this);
	}
}

HRESULT MAINGAME::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = D3D::GetDevice();
	pDevice->EndScene();
	for (int i = 0; i < MAX_PLAYER; i++) {
		player[i]->camera->SetCamera();
		D3DVIEWPORT9 vp = player[i]->camera->viewport;
		D3DRECT rect = { vp.X, vp.Y, vp.X + vp.Width, vp.Y + vp.Height };

		pDevice->Clear(0, &rect, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

		pDevice->BeginScene();
		player[i]->Draw(this);
		map->Draw();
		pDevice->EndScene();
	}
	pDevice->BeginScene();

	return S_OK;
}
