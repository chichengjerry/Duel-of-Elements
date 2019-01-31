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


struct STATEINTRO;
struct STATEEND;

typedef struct GAMESTATE {
	STAGETYPE			stage;
	MAINGAME*			pStateGame;
	MAINGAME*			pStateIntro;
	MAINGAME*			pStateEnd;

	GAMESTATE(STAGETYPE eStageType);
	~GAMESTATE(void);

	void				SetStage(STAGETYPE eStageType);
	void				Update(void);
	void				Draw(void);
} GAMESTATE;


#endif // !__STAGE_H__
