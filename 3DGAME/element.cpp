#include "element.h"

ELEMENT* create_element(void* map)
{
	ELEMENT* el = (ELEMENT*)malloc(sizeof(ELEMENT));
	ELEMENT_TYPE type = (ELEMENT_TYPE)GET_RANDOM(ELEMENT_EARTH, ELEMENT_AIR);

	SET_ELEMENTS(el->elements, type, 1);

	return el;
}
