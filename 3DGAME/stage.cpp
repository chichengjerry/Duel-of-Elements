#include "stage.h"

GAMESTATE::GAMESTATE(STAGETYPE eStageType)
{
	setStage(eStageType);
}

GAMESTATE::~GAMESTATE(void)
{
	switch (stage) {
	case STAGE_INTRO:
		FREE(pStateIntro);
		break;
	case STAGE_SINGLE:
	case STAGE_VERSUS:
	case STAGE_TUTORIAL:
		FREE(pStateGame);
		break;
	case STAGE_END:
		FREE(pStateEnd);
		break;
	}
}

STAGETYPE GAMESTATE::getStage(void)
{
	return stage;
}

void GAMESTATE::setStage(STAGETYPE eStageType)
{
	stage = eStageType;
	switch (eStageType) {
	case STAGE_INTRO:
		break;
	case STAGE_SINGLE:
		break;
	case STAGE_VERSUS:
		break;
	case STAGE_END:
		break;
	case STAGE_TUTORIAL:
		break;
	}
}
