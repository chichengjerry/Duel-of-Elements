#include "stage.h"

#define STAGE_INIT(p, a)	(p)->lpVtbl->init(p, a)
#define STAGE_UNINIT(p)		(p)->lpVtbl->uninit(p)

void stage_init(GAMESTATE* pStage, STAGETYPE eStageType)
{
	switch (eStageType) {
	case STAGE_INTRO:
		STAGE_INIT(pStage->pStateIntro, NULL);
		break;
	case STAGE_SINGLE:
		STAGE_INIT(pStage->pStateGame, 1);
		break;
	case STAGE_VERSUS:
		STAGE_INIT(pStage->pStateGame, 0);
		break;
	case STAGE_END:
		STAGE_INIT(pStage->pStateEnd, NULL);
		break;
	case STAGE_TUTORIAL:
		STAGE_INIT(pStage->pStateGame, 2);
		break;
	}
}

void stage_uninit(GAMESTATE* pStage)
{
	switch (pStage->stage) {
	case STAGE_INTRO:
		STAGE_UNINIT(pStage->pStateIntro);
		break;
	case STAGE_SINGLE:
	case STAGE_VERSUS:
	case STAGE_TUTORIAL:
		STAGE_UNINIT(pStage->pStateGame);
		break;
	case STAGE_END:
		STAGE_UNINIT(pStage->pStateEnd);
		break;
	}
}

STAGETYPE stage_get(GAMESTATE* pStage)
{
	return pStage->stage;
}

void stage_set(GAMESTATE* pStage, STAGETYPE eStageType)
{
	if (pStage->stage != eStageType) {
		STAGE_INIT(pStage, eStageType);
		GAME_UNINIT(pStage);
		pStage->stage = eStageType;
	};
}
