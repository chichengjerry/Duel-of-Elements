//-----------------------------------------------
// [stage.cpp]
// ステージ管理
// 作者 Chicheng Zhang
//-----------------------------------------------
#include "stage.h"
#include "game.h"

//
// コンストラクター
//
GAMESTAGE::GAMESTAGE(STAGETYPE eStageType)
{
	pStateTitle = NULL;
	pStateGame = NULL;
	oldStage = newStage = eStageType;

	SetStage(eStageType);
}

//
// デストラクター
//
GAMESTAGE::~GAMESTAGE(void)
{
	SAFE_DELETE(pStateTitle);
	SAFE_DELETE(pStateGame);
}

//
// ステージの遷移を行う
//
void GAMESTAGE::SetStage(STAGETYPE eStageType)
{
	newStage = eStageType;
}

//
// ステージの内容の更新
//
void GAMESTAGE::Update(void)
{
	switch (newStage) {
	case STAGE_TITLE:
	case STAGE_TUTORIAL:
		if (!pStateTitle) {
			pStateTitle = new TITLE(this);
		}
		else {
			pStateTitle->Update();
		}
		break;
	case STAGE_VERSUS:
		if (!pStateGame) {
			pStateGame = new MAINGAME(this, 2);
		}
		else {
			pStateGame->Update();
		}
		break;
	case STAGE_EXIT:
		PostQuitMessage(0);
		break;
	}
}

//
// ステージの内容の描画
//
void GAMESTAGE::Draw(void)
{
	switch (oldStage) {
	case STAGE_TITLE:
	case STAGE_TUTORIAL:
		if (pStateTitle) {
			pStateTitle->Draw();
		}
		break;
	case STAGE_VERSUS:
		if (pStateGame) {
			pStateGame->Draw();
		}
		break;
	}
	if (newStage != oldStage) {
		switch (oldStage) {
		case STAGE_TITLE:
			if (newStage == STAGE_VERSUS) {
				SAFE_DELETE(pStateTitle);
			}
			break;
		case STAGE_VERSUS:
			SAFE_DELETE(pStateGame);
			break;
		}

		oldStage = newStage;
	}
}
