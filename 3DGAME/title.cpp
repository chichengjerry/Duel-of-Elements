#include "title.h"
#include "stage.h"
#include "terrain.h"
#include "ui_title.h"

STAGETYPE stageType[TITLEMENU_COUNT] = {
	STAGE_VERSUS, STAGE_EXIT
};
TITLE::TITLE(GAMESTAGE* state)
{
	menuSelected = 0;

	pState = state;
	terrain = new TERRAIN(GM_TERRIAN_LEVEL);
	sky = new SKY();

	camera = new CAMERA(NULL, NULL);
	camera->v_tar = D3DXVECTOR3(0.0f, GM_MAX_HEIGHT / 2, 0.0f);
	camera->v_up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	FLOAT dist = GM_TERRAIN_SIZE * (2 << GM_TERRIAN_LEVEL);
	D3DXMATRIX mtxRot;
	D3DXVECTOR4 vec = D3DXVECTOR4(0.0f, 0.0f, dist, 1.0f);
	D3DXMatrixRotationX(&mtxRot, D3DXToRadian(30.0f));
	D3DXVec4Transform(&vec, &vec, &mtxRot);
	rot = D3DXVECTOR3(vec.x, vec.y, vec.z);

	titleUI = new TITLEUI(this);
}

TITLE::~TITLE()
{
	SAFE_DELETE(camera);
	SAFE_DELETE(sky);
	SAFE_DELETE(terrain);
	SAFE_DELETE(titleUI);
}

void TITLE::Update() {

	sky->Update();
	terrain->Update();
	UpdateCamera();

	if (DINPUT::KeyTriggered(DIK_UP)) {
		menuSelected = (menuSelected + TITLEMENU_COUNT - 1) % TITLEMENU_COUNT;
	}
	if (DINPUT::KeyTriggered(DIK_DOWN)) {
		menuSelected = (menuSelected + 1) % TITLEMENU_COUNT;
	}
	if (DINPUT::KeyTriggered(DIK_RETURN)) {
		pState->SetStage(stageType[menuSelected]);
	}
	titleUI->Update();
}

void TITLE::UpdateCamera()
{
	LPDIRECT3DDEVICE9 pDevice = D3D::GetDevice();
	D3DXVECTOR4 vec;
	D3DXMATRIX mtxRot;
	D3DXMatrixRotationY(&mtxRot, D3DXToRadian(2.0f) * D3D::fAnimationRate);
	D3DXVec3Transform(&vec, &rot, &mtxRot);
	rot = D3DXVECTOR3(vec.x, vec.y, vec.z);
	camera->v_pos = camera->v_tar - rot;

	// ビューマトリックスの作成
	D3DXMatrixIdentity(&camera->m_view);
	D3DXMatrixLookAtLH(&camera->m_view,
		&camera->v_pos,		// カメラの視点
		&camera->v_tar,		// カメラの注視点
		&camera->v_up);		// カメラの上方向

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &camera->m_view);
}

HRESULT TITLE::Draw()
{
	sky->Draw(camera);
	camera->SetCamera();

	D3D::EnableFog(sky->light->GetFogColor());
	terrain->Draw(camera);
	D3D::DisableFog();

	WCHAR str[256];
	swprintf_s(str, _T("menuSelected %d"), menuSelected);
	D3D::ShowText(str, 0, 0);

	titleUI->Draw();

	return S_OK;
}
