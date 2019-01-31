#include "player.h"

PLAYER::PLAYER(BOOL isComputer, SRT* spawn)
{
	this->isComputer = isComputer;
	srt = *spawn;
	mdl = new MODEL(_T(""), srt);
}
