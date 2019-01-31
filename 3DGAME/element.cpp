#include "element.h"

ELEMENT* create_element(void* map)
{
	ELEMENT* el = (ELEMENT*)malloc(sizeof(ELEMENT));
	ELEMENTTYPE type = (ELEMENTTYPE)GET_RANDOM(ELEMENTTYPE_EARTH, ELEMENTTYPE_AIR);

	SET_ELEMENTS(el->elements, type, 1);

	return el;
}
