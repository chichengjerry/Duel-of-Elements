#include "element.h"
#include "game.h"

#define ELEMENT_ROTATION

LPCWSTR ElementModels[4] = {
	_T("data/MODEL/hexahedron.x"),
	_T("data/MODEL/icosahedron.x"),
	_T("data/MODEL/tetrahedron.x"),
	_T("data/MODEL/octahedron.x")
};

ELEMENT::ELEMENT(SRT* srt)
{
	this->srt = *srt;
	this->mdl = NULL;
	Fresh();
}

ELEMENT::~ELEMENT()
{
	SAFE_DELETE(mdl);
}

void ELEMENT::Fresh(void)
{
	elementType = (ELEMENTTYPE)GET_RANDOM(ELEMENTTYPE_EARTH - 1, ELEMENTTYPE_AIR - 1);
	
	if (mdl)
		SAFE_DELETE(mdl);

	mdl = new MODEL(ElementModels[elementType], NULL);
}

void ELEMENT::Update(MAINGAME* pGame)
{
	srt.rot.y += D3DXToRadian(1.0f);
	srt.rot.x += D3DXToRadian(1.0f);
	srt.rot.z += D3DXToRadian(1.0f);
	mdl->srt = srt;
	mdl->Update();
}

HRESULT ELEMENT::Draw()
{
	return mdl->Draw(NULL);
}
