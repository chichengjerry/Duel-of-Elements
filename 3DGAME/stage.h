#ifndef __STAGE_H__
#define __STAGE_H__

#include "main.h"
#include "core.h"
#include "game.h"

enum STAGETYPE {
	STAGE_INTRO,
	STAGE_SINGLE,
	STAGE_VERSUS,
	STAGE_TUTORIAL,
	STAGE_END
};

#define VTBL STAGEVTABLE

struct VTBL;

typedef struct {
	STAGETYPE			stage;
	GAME*				pStateGame;
	INTRO*				pStateIntro;
	END*				pStateEnd;
	
	VTBL*				lpVtbl;
} GAMESTATE;

void					stage_init(GAMESTATE* pStage, STAGETYPE eStageType);
void					stage_uninit(GAMESTATE* pStage);

STAGETYPE				stage_get(GAMESTATE* pStage);
void					stage_set(GAMESTATE* pStage, STAGETYPE eStageType);

typedef struct VTBL{
	void				(*init)(GAMESTATE* _this, STAGETYPE eStageType) = stage_init;
	void				(*uninit)(GAMESTATE* _this) = stage_uninit;
};

#undef VTBL

#endif // !__STAGE_H__
