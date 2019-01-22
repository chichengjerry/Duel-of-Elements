#ifndef __STAGE_H__
#define __STAGE_H__

#include "main.h"

enum STAGETYPE {
	STAGE_INTRO,
	STAGE_GAME,
	STAGE_TUTOR,
	STAGE_END
};

typedef struct {
	STAGETYPE		stage;

	
} STAGE;

STAGETYPE		stage_get(STAGE* pStage);
void			stage_set(STAGE* pStage, STAGETYPE eStageType);


#endif // !__STAGE_H__
