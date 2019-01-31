#include "element.h"

typedef struct ELEMENTMODEL {
	LPCWSTR				fileName;
	D3DXCOLOR			col;
} ELEMENTMODEL;
ELEMENTMODEL ElementModels[4] = {
	{_T("data/MODEL/hexahedron.x"),		D3DXCOLOR(0.25f, 1.0f, 0.25f, 1.0f) },
	{_T("data/MODEL/icosahedron.x"),	D3DXCOLOR(0.25f, 0.25f, 1.0f, 1.0f) },
	{_T("data/MODEL/tetrahedron.x"),	D3DXCOLOR(1.0f, 0.25f, 0.25f, 1.0f) },
	{_T("data/MODEL/octahedron.x"),		D3DXCOLOR(0.75f, 0.75f, 1.0f, 1.0f) },
};


ELEMENT::ELEMENT(SRT* srt)
{
	this->srt = *srt;
	Fresh();
}

ELEMENT::~ELEMENT()
{
	delete mdl;
}

void ELEMENT::Fresh(void)
{
	this->elementType = (ELEMENTTYPE)GET_RANDOM(ELEMENTTYPE_EARTH, ELEMENTTYPE_AIR);
	if (mdl)
		delete mdl;

	mdl = new MODEL(ElementModels[this->elementType].fileName, NULL);
	mdl->col = ElementModels[this->elementType].col;
}
