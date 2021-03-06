//-----------------------------------------------
// [title.cpp]
// タイトル処理
// 作者 Chicheng Zhang
//-----------------------------------------------
#include "title.h"
#include "stage.h"
#include "sound.h"
#include "terrain.h"
#include "ui_title.h"
#include "ui_tutorial.h"

//
// ステージ遷移用
//
STAGETYPE stageType[TITLEMENU_COUNT] = {
	STAGE_VERSUS, STAGE_TUTORIAL, STAGE_EXIT
};

//
// タイトルの初期化
//
TITLE::TITLE(GAMESTAGE* state)
{
	menuSelected = 0;

	pState = state;
	terrain = new TERRAIN(GM_TERRIAN_LEVEL);
	sky = new SKY();

	// プレイヤーとビュー情報使わない
	{
		camera = new CAMERA(NULL, NULL);
		camera->v_tar = D3DXVECTOR3(0.0f, GM_MAX_HEIGHT / 2, 0.0f);
		camera->v_up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		FLOAT dist = GM_TERRAIN_SIZE * (2 << GM_TERRIAN_LEVEL);
		D3DXMATRIX mtxRot;
		rot = D3DXVECTOR3(0.0f, 0.0f, dist);
		D3DXMatrixRotationX(&mtxRot, D3DXToRadian(30.0f));
		D3DXVec3TransformCoord(&rot, &rot, &mtxRot);
	}
	titleUI = new TITLEUI(this);
	tutorialUI = new TUTORIALUI(this);

	DSOUND::Play(SOUND_BGM_TITLE);
}

//
// タイトルのリソースの解放
//
TITLE::~TITLE()
{
	SAFE_DELETE(camera);
	SAFE_DELETE(sky);
	SAFE_DELETE(terrain);
	SAFE_DELETE(titleUI);
	SAFE_DELETE(tutorialUI);

	DSOUND::Stop(SOUND_BGM_TITLE);
}

//
// タイトルの情報を更新する。
//
void TITLE::Update() {

	sky->Update();
	terrain->Update();
	UpdateCamera();

	switch (pState->newStage) {
	case STAGE_TUTORIAL:
		if (DINPUT::KeyTriggered(DIK_RETURN) || DINPUT::ButtonTriggered(BUTTON_START)) {
			pState->SetStage(STAGE_TITLE);
			DSOUND::Play(SOUND_CLICK);
		}

		tutorialUI->Update();
		break;
	case STAGE_TITLE:
	default:
		if (DINPUT::KeyTriggered(DIK_UP) || DINPUT::ButtonTriggered(BUTTON_UP)) {
			menuSelected = (menuSelected + TITLEMENU_COUNT - 1) % TITLEMENU_COUNT;
			DSOUND::Play(SOUND_CLICK);
		}
		if (DINPUT::KeyTriggered(DIK_DOWN) || DINPUT::ButtonTriggered(BUTTON_DOWN)) {
			menuSelected = (menuSelected + 1) % TITLEMENU_COUNT;
			DSOUND::Play(SOUND_CLICK);
		}
		if (DINPUT::KeyTriggered(DIK_RETURN) || DINPUT::ButtonTriggered(BUTTON_START)) {
			pState->SetStage(stageType[menuSelected]);
			DSOUND::Play(SOUND_CLICK);
		}
		titleUI->Update();
		break;
	}

}

//
// タイトルの背景を回転させる。
//
void TITLE::UpdateCamera()
{
	LPDIRECT3DDEVICE9 pDevice = D3D::GetDevice();
	D3DXMATRIX mtxRot;
	D3DXMatrixRotationY(&mtxRot, D3DXToRadian(2.0f) * D3D::fAnimationRate);
	D3DXVec3TransformCoord(&rot, &rot, &mtxRot);
	camera->v_pos = camera->v_tar - rot;

	D3DXMatrixLookAtLH(&camera->m_view,	&camera->v_pos,	&camera->v_tar,	&camera->v_up);

	pDevice->SetTransform(D3DTS_VIEW, &camera->m_view);
}

//
// タイトルを描画する。
//
HRESULT TITLE::Draw()
{
	sky->Draw(camera);
	camera->SetCamera();

	D3D::EnableFog(sky->light->GetFogColor());
	terrain->Draw(camera);
	D3D::DisableFog();

#if _DEBUG
	WCHAR str[256];
	swprintf_s(str, _T("menuSelected %d"), menuSelected);
	D3D::ShowText(str, 0, 0);
#endif

	switch (pState->newStage) {
	case STAGE_TITLE:
		titleUI->Draw();
		break;
	case STAGE_TUTORIAL:
		tutorialUI->Draw();
		break;
	}

	return S_OK;
}
