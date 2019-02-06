#include "terrian.h"
#include "player.h"

TERRAIN::TERRAIN(DWORD depth)
{
	dwMapSize = (1 << (depth * 2));
	dwHeightMapSize = dwMapSize + 1;

	pHeightMap = (BYTE*)malloc(sizeof(BYTE) * dwHeightMapSize * dwHeightMapSize);
	memset(pHeightMap, 0, sizeof(BYTE) * dwHeightMapSize * dwHeightMapSize);

	BYTE* leftTop = pHeightMap;
	BYTE* rightTop = pHeightMap + (dwHeightMapSize - 1);
	BYTE* leftBottom = pHeightMap + dwHeightMapSize * (dwHeightMapSize - 1);
	BYTE* rightBottom = pHeightMap + (dwHeightMapSize * dwHeightMapSize - 1);

	*leftTop = 
	*rightTop =
	*leftBottom = 
	*rightBottom = 128;

	GenerateHeightMap(leftTop, rightTop, leftBottom, rightBottom, depth * 2);

	SetVertex();
}

TERRAIN::~TERRAIN()
{
	SAFE_DELETE(pHeightMap);
	DWORD count = dwMapSize * dwMapSize;
	for (DWORD i = 0; i < count; i++) {
		SAFE_DELETE(ppGround[i]);
	}
}

HRESULT TERRAIN::SetVertex()
{
	ppGround = (GROUND**)malloc(sizeof(GROUND*) * dwMapSize * dwMapSize);

	struct _vtx {
		D3DXVECTOR3 vtx;
		D3DXVECTOR3 nor;
	};

	_vtx* vtx = (_vtx*)malloc(sizeof(_vtx) * dwHeightMapSize * dwHeightMapSize);
	
#define NEIGHBOUR_VTX	6
	struct posfix {
		DWORD dx;
		DWORD dz;
	} pos[NEIGHBOUR_VTX] = {
		{0, 1},
		{-1, 0},
		{0, -1},
		{1, 1}
	};

	// Gernerate vertex
	for (DWORD z = 0; z < dwHeightMapSize; z++) {
		for (DWORD x = 0; x < dwHeightMapSize; x++) {
			vtx[z * dwHeightMapSize + x].vtx = GM_TERRAIN_SIZE * D3DXVECTOR3((FLOAT)x, GM_TERRAIN_MAG * ((FLOAT)GetPoint(x, z) - 128.0f), (FLOAT)z);
		}
	}
	//Gernerate normal
	for (DWORD z = 0; z < dwHeightMapSize; z++) {
		for (DWORD x = 0; x < dwHeightMapSize; x++) {
		
			/*{
				D3DXVECTOR3 origin = vtx[z * dwHeightMapSize + x].vtx;
				D3DXVECTOR3 nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

				for (int i = 0; i < NEIGHBOUR_VTX; i++) {
					D3DXVECTOR3 va, vb, vn;
					int pa = (z + pos[i].dz) * dwHeightMapSize + (x + pos[i].dx);
					int pb = (z + pos[(i + 1) % NEIGHBOUR_VTX].dz) * dwHeightMapSize + pos[(i + 1) % NEIGHBOUR_VTX].dx;
					if (pa < 0 || pa > dwHeightMapSize * dwHeightMapSize - 1) {}
					else if (pb < 0 || pb > dwHeightMapSize * dwHeightMapSize - 1) {}
					else {
						va = vtx[pa].vtx - origin;
						vb = vtx[pb].vtx - origin;
						D3DXVec3Cross(&vn, &va, &vb);
						D3DXVec3Normalize(&vn, &vn);
						nor += vn;
					}
				}

				D3DXVec3Normalize(&nor, &nor);

				vtx[z * dwHeightMapSize + x].nor = nor;
			}*/
			{
				#define	GetVtx(x, z)	((x) < 0 || (z) < 0 || (x) > dwHeightMapSize - 1 || (z) > dwHeightMapSize - 1) ? 0 : vtx[(z) * dwHeightMapSize + (x)].vtx.y
				FLOAT hL = GetVtx(x - 1, z);
				FLOAT hR = GetVtx(x + 1, z);
				FLOAT hD = GetVtx(x, z - 1);
				FLOAT hU = GetVtx(x, z + 1);

				D3DXVECTOR3 nor;
				// deduce terrain normal
				nor.x = hL - hR;
				nor.z= hD - hU;
				nor.y = 2.0;
				D3DXVec3Normalize(&nor, &nor);

				vtx[z * dwHeightMapSize + x].nor = nor;
			}
		}
	}

	for (DWORD z = 0; z < dwMapSize; z++) {
		for (DWORD x = 0; x < dwMapSize; x++) {
			D3DXVECTOR3 vec[4], nor[4];
			_vtx group[4];
			if ((x + z) & 1) {
				//
				//		x, z + 1		x + 1, z + 1
				//					N
				//		x, z			x + 1, z
				//
				group[0] = vtx[(z + 1) * dwHeightMapSize + x + 1];
				group[1] = vtx[z * dwHeightMapSize + x + 1];
				group[2] = vtx[(z + 1) * dwHeightMapSize + x];
				group[3] = vtx[z * dwHeightMapSize + x];
			}
			else {
				//
				//		x, z + 1		x + 1, z + 1
				//					Z
				//		x, z			x + 1, z
				//
				group[0] = vtx[(z + 1) * dwHeightMapSize + x];
				group[1] = vtx[(z + 1) * dwHeightMapSize + x + 1];
				group[2] = vtx[z * dwHeightMapSize + x];
				group[3] = vtx[z * dwHeightMapSize + x + 1];
			}

			for (int i = 0; i < 4; i++) {
				vec[i] = group[i].vtx;
				nor[i] = group[i].nor;
			}

			ppGround[z * dwMapSize + x] = new GROUND(vec, nor);
		}
	}

	delete vtx;

	return S_OK;
}

HRESULT TERRAIN::Draw(PLAYER* player)
{
	DWORD distance;

	for (DWORD z = 0; z < dwMapSize; z++) {
		for (DWORD x = 0; x < dwMapSize; x++) {
			distance = (player->gx - x) * (player->gx - x) + (player->gz - z) * (player->gz - z);
			
			if (distance * GM_TERRAIN_SIZE * GM_TERRAIN_SIZE < GR_FAR_Z * GR_FAR_Z / 4) {
				ppGround[z * dwMapSize + x]->Draw();
			}
		}
	}
	return S_OK;
}

void TERRAIN::Update()
{
	DWORD count = dwMapSize * dwMapSize;
	for (DWORD i = 0; i < count; i++) {
		ppGround[i]->Update();
	}
}

BYTE * TERRAIN::GetCenter(BYTE * leftTop, BYTE * rightBottom)
{
	return leftTop + ((rightBottom - leftTop) / 2);
}

BYTE TERRAIN::GetPoint(INT x, INT y)
{
	return pHeightMap[dwHeightMapSize * y + x];
}

FLOAT TERRAIN::GetGroundHeight(FLOAT x, FLOAT z)
{
	FLOAT height = 0.0f;

	x /= GM_TERRAIN_SIZE;
	z /= GM_TERRAIN_SIZE;

	DWORD gx = (DWORD)floorf(x);
	DWORD gz = (DWORD)floorf(z);

	if (gx < 0 || gz < 0) {
		return height;
	}
	x -= gx;
	z -= gz;
	if(gx > dwMapSize - 1)
		gx = dwMapSize - 1;
	if(gz > dwMapSize - 1)
		gz = dwMapSize - 1;

	POLYGON* polygon = ppGround[gz * dwMapSize + gx]->polygon;
	VERTEX_3D* pTempVtx;
	FLOAT terrainHeight[4];

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	polygon->pVtx->Lock(0, 0, (void**)&pTempVtx, 0);

	// 頂点座標の取得
	if ((gx + gz) & 1) {
		terrainHeight[0] = pTempVtx[2].vtx.y;
		terrainHeight[1] = pTempVtx[0].vtx.y;
		terrainHeight[2] = pTempVtx[3].vtx.y;
		terrainHeight[3] = pTempVtx[1].vtx.y;

		if (x > 1.0f - z) {
			height = BarryCentric(
				D3DXVECTOR3(1.0f, terrainHeight[1], 0.0f),
				D3DXVECTOR3(1.0f, terrainHeight[3], 1.0f),
				D3DXVECTOR3(0.0f, terrainHeight[0], 0.0f),
				D3DXVECTOR2(x, 1.0f - z));
		}
		else {
			height = BarryCentric(
				D3DXVECTOR3(1.0f, terrainHeight[3], 1.0f),
				D3DXVECTOR3(0.0f, terrainHeight[2], 1.0f),
				D3DXVECTOR3(0.0f, terrainHeight[0], 0.0f),
				D3DXVECTOR2(x, 1.0f - z));
		}
	}
	else {
		// 頂点座標の取得
		terrainHeight[0] = pTempVtx[0].vtx.y;
		terrainHeight[1] = pTempVtx[1].vtx.y;
		terrainHeight[2] = pTempVtx[2].vtx.y;
		terrainHeight[3] = pTempVtx[3].vtx.y;

		if (x < z) {
			height = BarryCentric(
				D3DXVECTOR3(0.0f, terrainHeight[0], 0.0f),
				D3DXVECTOR3(1.0f, terrainHeight[1], 0.0f),
				D3DXVECTOR3(0.0f, terrainHeight[2], 1.0f),
				D3DXVECTOR2(x, 1.0f - z));
		}
		else {
			height = BarryCentric(
				D3DXVECTOR3(1.0f, terrainHeight[1], 0.0f),
				D3DXVECTOR3(1.0f, terrainHeight[3], 1.0f),
				D3DXVECTOR3(0.0f, terrainHeight[2], 1.0f),
				D3DXVECTOR2(x, 1.0f - z));
		}
	}
	polygon->pVtx->Unlock();

	return height;
}

BYTE TERRAIN::GetAverage(BYTE* p1, BYTE* p2, BYTE* p3, BYTE* p4, INT depth)
{
	int range = 128 >> (8 - depth) || 1;

	INT r = GET_RANDOM(-range, range - 1);

	return (*p1 + *p2 + *p3 + *p4) / 4 + r;
}

void TERRAIN::GenerateHeightMap(BYTE* leftTop, BYTE* rightTop, BYTE* leftBottom, BYTE* rightBottom, INT depth)
{
	if (depth < 0)
		return;

	BYTE* center = GetCenter(leftTop, rightBottom);
	*center = GetAverage(leftTop, rightTop, leftBottom, rightBottom, depth);

	BYTE* centerTop = GetCenter(leftTop, rightTop);
	*centerTop = GetAverage(leftTop, rightTop, center, center, depth);

	BYTE* centerLeft = GetCenter(leftTop, leftBottom);
	*centerLeft = GetAverage(leftTop, leftBottom, center, center, depth);

	BYTE* centerRight = GetCenter(rightTop, rightBottom);
	*centerRight = GetAverage(rightTop, rightBottom, center, center, depth);

	BYTE* centerBottom = GetCenter(leftBottom, rightBottom);
	*centerBottom = GetAverage(leftBottom, rightBottom, center, center, depth);

	//LeftTop
	GenerateHeightMap(leftTop, centerTop, centerLeft, center, depth - 1);

	//RightTop
	GenerateHeightMap(centerTop, rightTop, center, centerRight, depth - 1);

	//LeftBottom
	GenerateHeightMap(centerLeft, center, leftBottom, centerBottom, depth - 1);

	//RightBottom
	GenerateHeightMap(center, centerRight, centerBottom, rightBottom, depth - 1);
}
