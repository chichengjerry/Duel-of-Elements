#include "stage.h"

GAMESTATE::GAMESTATE(STAGETYPE eStageType)
{
	SetStage(eStageType);
}

GAMESTATE::~GAMESTATE(void)
{
	switch (stage) {
	case STAGE_INTRO:
		SAFE_DELETE(pStateIntro);
		break;
	case STAGE_SINGLE:
	case STAGE_VERSUS:
	case STAGE_TUTORIAL:
		SAFE_DELETE(pStateGame);
		break;
	case STAGE_END:
		SAFE_DELETE(pStateEnd);
		break;
	}
}

void GAMESTATE::SetStage(STAGETYPE eStageType)
{
	stage = eStageType;
	switch (eStageType) {
	case STAGE_INTRO:
		break;
	case STAGE_SINGLE:
		pStateGame = new MAINGAME(1);
		break;
	case STAGE_VERSUS:
		pStateGame = new MAINGAME(2);
		break;
	case STAGE_END:
		break;
	case STAGE_TUTORIAL:
		pStateGame = new MAINGAME(0);
		break;
	}
}

void GAMESTATE::Update(void)
{
	switch (stage) {
	case STAGE_INTRO:
		pStateIntro->Update();
		break;
	case STAGE_SINGLE:
	case STAGE_VERSUS:
	case STAGE_TUTORIAL:
		pStateGame->Update();
		break;
	case STAGE_END:
		pStateEnd->Update();
		break;
	}
}

void GAMESTATE::Draw(void)
{
	switch (stage) {
	case STAGE_INTRO:
		pStateIntro->Draw();
		break;
	case STAGE_SINGLE:
	case STAGE_VERSUS:
	case STAGE_TUTORIAL:
		pStateGame->Draw();
		break;
	case STAGE_END:
		pStateEnd->Draw();
		break;
	}
}
