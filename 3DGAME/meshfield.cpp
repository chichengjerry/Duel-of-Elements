//-----------------------------------------------
// [meshfield.cpp]
// メッシュフィールドの管理
// 作者 Chicheng Zhang
//-----------------------------------------------
#include "meshfield.h"

//
// 地面に色付けの情報を設定する。
//
COLORMAP::COLORMAP(INT steps, COLORSTEP *cols)
{
	if (steps < 2) {
		return;
	}
	else {
		this->steps = steps;
		this->mCols = new COLORSTEP[steps];
		for (int i = 0; i < steps; i++) {
			this->mCols[i] = cols[i];
		}
	}
}
//
// バーセントで計算した色情報を計算する。
//
D3DXCOLOR COLORMAP::GetColor(FLOAT t)
{
	if (t <= mCols[0].t)
		return mCols[0].color;
	if (t >= mCols[steps - 1].t)
		return mCols[steps - 1].color;

	for (int i = 0; i < steps - 1; i++) {
		if (t >= mCols[i].t && t <= mCols[i + 1].t) {

			// Liner Interpolate
			FLOAT dif = mCols[i + 1].t - mCols[i].t;

			if (dif <= 0.0f) {
				return mCols[i + 1].color;
			}
			t = (t - mCols[i].t) / dif;

			return mCols[i].color * (1 - t) + mCols[i + 1].color * t;
		}
	}

	return D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
}

//
// メッシュフィールドの情報構造する。
//
MESHFIELD::MESHFIELD(DWORD mapSizeX, DWORD mapSizeY, DWORD blockSizeX, DWORD blockSizeY, FLOAT minHeight, FLOAT maxHeight)
{
	pVtxBuf = NULL;
	pIdxBuf = NULL;
	pTexBuf = NULL;

	nBlkSizeX = blockSizeX;
	nBlkSizeZ = blockSizeY;
	nMapSizeX = mapSizeX;
	nMapSizeZ = mapSizeY;
	fMinHeight = minHeight;
	fMaxHeight = maxHeight;
	dwVtxCount = (nMapSizeX + 1) * (nMapSizeZ + 1);
	dwIdxCount = (nMapSizeZ * ((nMapSizeX + 1) + 1) - 1) * 2;
	dwPolyCount = nMapSizeZ * (nMapSizeX * 2 + 4) - 4;
}

//
// メッシュフィールドのリソースを解放する。
//
MESHFIELD::~MESHFIELD()
{
	SAFE_RELEASE(pVtxBuf);
	SAFE_RELEASE(pIdxBuf);
	SAFE_RELEASE(pTexBuf);
}

//
// メッシュフィールドを描画する。
//
HRESULT MESHFIELD::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = D3D::GetDevice();

	D3DXMATRIX mtx;
	D3DXMatrixIdentity(&mtx);

	// 情報設定
	pDevice->SetTransform(D3DTS_WORLD, &mtx);
	pDevice->SetStreamSource(0, pVtxBuf, 0, sizeof(VERTEX_3D));
	pDevice->SetIndices(pIdxBuf);
	pDevice->SetFVF(FVF_VERTEX_3D);
	pDevice->SetTexture(0, pTexBuf);

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, dwVtxCount, 0, dwPolyCount);

	return S_OK;
}
//
// メッシュフィールドを更新する。
//
void MESHFIELD::Update()
{
	// 現在特に更新はしない。
	return;
}

//
// メッシュフィールドの地面にテクスチャを貼り付ける。
//
HRESULT MESHFIELD::SetTexture(LPCWSTR textureSrc)
{
	LPDIRECT3DDEVICE9 pDevice = D3D::GetDevice();
	if (FAILED(D3DXCreateTextureFromFile(
		pDevice,
		textureSrc,
		&pTexBuf))) {
		return E_FAIL;
	}
	return S_OK;
}

//
// 頂点情報を計算しバッファに入れる。
//
HRESULT MESHFIELD::SetVertex(BYTE* pHeightMap, COLORMAP* colorMap)
{
	LPDIRECT3DDEVICE9 pDevice = D3D::GetDevice();

	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * dwVtxCount,
		D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &pVtxBuf, NULL)))
	{
		return E_FAIL;
	}

	// オブジェクトのインデックスバッファを生成
	if (FAILED(pDevice->CreateIndexBuffer(sizeof(DWORD) * dwIdxCount,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX32,		// インデックス数は65535を越えるので、32BITのインデックスを使用する
		D3DPOOL_MANAGED,
		&pIdxBuf,
		NULL)))
	{
		return E_FAIL;
	}

	// 頂点作成
	{

		VERTEX_3D *pVtx;
		FLOAT texSizeX = 64.0f / nMapSizeX;
		FLOAT texSizeZ = 64.0f / nMapSizeZ;

		pVtxBuf->Lock(0, 0, (void**)&pVtx, 0);

		for (int z = 0; z < (nMapSizeZ + 1); z++) {
			for (int x = 0; x < (nMapSizeX + 1); x++) {
				FLOAT height = fMinHeight + (fMaxHeight - fMinHeight) * (pHeightMap[z * (nMapSizeX + 1) + x] / 255.0f);
				D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				// 頂点位置を設定する。
				pVtx[z * (nMapSizeX + 1) + x].vtx = D3DXVECTOR3((x - nMapSizeX / 2.0f) * nBlkSizeX, height, (z - nMapSizeZ / 2.0f) * nBlkSizeZ);

				// 色を設定する。
				pVtx[z * (nMapSizeX + 1) + x].dif = GetDiffuse(pHeightMap[z * (nMapSizeX + 1) + x], colorMap);

				// テクスチャのUVを設定する。
				pVtx[z * (nMapSizeX + 1) + x].tex = D3DXVECTOR2(texSizeX * x, texSizeZ * z);
			}
		}

		//法線ベクターを計算する。
		for (int z = 0; z < (nMapSizeZ + 1); z++) {
			for (int x = 0; x < (nMapSizeX + 1); x++) {
#define	GetVtx(x, z)	((x) < 0 || (z) < 0 || (x) > nMapSizeX || (z) > nMapSizeZ) ? 0 : pVtx[(z) * (nMapSizeX + 1) + (x)].vtx.y
				FLOAT hL = GetVtx(x - 1, z);
				FLOAT hR = GetVtx(x + 1, z);
				FLOAT hD = GetVtx(x, z - 1);
				FLOAT hU = GetVtx(x, z + 1);
#undef GetVtx
				// 外積の代わりに、高度差で法線を計算する。
				// 精確ではないが、十分に使える。
				D3DXVECTOR3 nor = D3DXVECTOR3(hL - hR, (FLOAT)(nBlkSizeX + nBlkSizeZ), hD - hU);
				D3DXVec3Normalize(&nor, &nor);

				pVtx[z * (nMapSizeX + 1) + x].nor = nor;
			}
		}

		pVtxBuf->Unlock();
	}

	// インデックス作成
	{
		DWORD* pIdx;
		DWORD index = 0;

		pIdxBuf->Lock(0, sizeof(DWORD), (void**)&pIdx, 0);

		for (int z = 0; z < nMapSizeZ; z++) {

			// 縮退ポリゴンのためのダブりの設定
			if (z > 0) {
				pIdx[index++] = z * (nMapSizeX + 1);
			}

			for (int x = 0; x < nMapSizeX + 1; x++) {
				pIdx[index++] = z * (nMapSizeX + 1) + x;
				pIdx[index++] = (z + 1) * (nMapSizeX + 1) + x;
			}

			// 縮退ポリゴンのためのダブりの設定
			if (z < (nMapSizeZ - 1))
			{
				pIdx[index++] = (z + 1) * (nMapSizeX + 1) + nMapSizeX;
			}
		}
		pIdxBuf->Unlock();
	}

	return S_OK;
}

//
// 高度により色情報を求める。
//
D3DXCOLOR MESHFIELD::GetDiffuse(BYTE height, COLORMAP* colorMap)
{
	FLOAT t = height / 255.0f;
	if (colorMap)
		return colorMap->GetColor(t);
	else
		return D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}

//
// 平面の座標で高さを求める。
//
FLOAT MESHFIELD::GetPositionHeight(FLOAT fx, FLOAT fz)
{
	FLOAT height = 0.0f;

	fx /= nBlkSizeX;
	fz /= nBlkSizeZ;

	INT gx = (DWORD)floorf(fx);
	INT gz = (DWORD)floorf(fz);

	gx = gx <= -nMapSizeX / 2 ? -nMapSizeX / 2 : gx;
	gz = gz <= -nMapSizeZ / 2 ? -nMapSizeZ / 2 : gz;
	gx = gx >= nMapSizeX / 2 ? nMapSizeX / 2 - 1 : gx;
	gz = gz >= nMapSizeZ / 2 ? nMapSizeX / 2 - 1 : gz;

	// 座標点を正規化する。
	// fx, fzは三角形の上の比率
	// gx, gzは格子の順番
	fx -= gx;
	fz -= gz;
	gx += nMapSizeX / 2;
	gz += nMapSizeZ / 2;

	VERTEX_3D* pVtx;
	pVtxBuf->Lock(0, sizeof(VERTEX_3D), (void**)&pVtx, 0);

	FLOAT lt, rt, ld, rd;
	lt = pVtx[gz * (nMapSizeX + 1) + gx].vtx.y;
	rt = pVtx[(gz + 1) * (nMapSizeX + 1) + gx].vtx.y;
	ld = pVtx[(gz)* (nMapSizeX + 1) + gx + 1].vtx.y;
	rd = pVtx[(gz + 1) * (nMapSizeX + 1) + gx + 1].vtx.y;
	//
	// 格子は二つの三角形で構成している。
	// ここで、ポイントはどの三角形の上にあるかを判断する必要がある。
	//
	if (fx < 1.0f - fz) {
		//上半分の三角形の重心系座標を求める。
		height = BarryCentric(
			D3DXVECTOR3(0.0f, lt, 0.0f),
			D3DXVECTOR3(0.0f, rt, 1.0f),
			D3DXVECTOR3(1.0f, ld, 0.0f),
			D3DXVECTOR2(fx, fz));
	}
	else {
		//下半分の三角形の重心系座標を求める。
		height = BarryCentric(
			D3DXVECTOR3(0.0f, rt, 1.0f),
			D3DXVECTOR3(1.0f, rd, 1.0f),
			D3DXVECTOR3(1.0f, ld, 0.0f),
			D3DXVECTOR2(fx, fz));
	}

	return height;
}

//
// 座標はメッシュフィールドの範囲内であるかをチェックする。
//
BOOL MESHFIELD::IsOutofBound(FLOAT fx, FLOAT fz)
{
	FLOAT posXmax = (FLOAT)(nBlkSizeX * nMapSizeX / 2);
	FLOAT posZmax = (FLOAT)(nBlkSizeZ * nMapSizeZ / 2);
	FLOAT posXmin = -posXmax;
	FLOAT posZmin = -posZmax;

	return (fx > posXmax || fx < posXmin || fz > posZmax || fz < posZmin);
}
