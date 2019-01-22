#include "stage.h"

STAGETYPE stage_get(STAGE * pStage)
{
	return pStage->stage;
}

void stage_set(STAGE* pStage, STAGETYPE eStageType)
{
	pStage->stage = eStageType;
}
