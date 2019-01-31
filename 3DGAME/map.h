/***********************************************
 * [map.h]
 * •ﬁ•√•◊ÑI¿Ì
 * GP11A341 24 èàÒYÚG
 ***********************************************/

#ifndef __MAP_H__
#define __MAP_H__

#include "main.h"
#include "heap.h"
#include "node.h"

typedef struct MAP {
	int width;
	int height;
	NODE* nodes;

	MAP(INT width, INT height);
	~MAP();

	void				FindPath(NODE* start, NODE* goal, NODE* path[]);
	BOOL				IsOutOfBound(INT x, INT y);
	NODE*				GetNode(INT x, INT y);
	void				FindNeighbours(NODE* pCurrentNode, NODE* pNeighbourNodes[]);
	HRESULT				Draw();
	void				Update();
	void				SetNode(INT x, INT y, NODE* newNode);
} MAP;

#endif // !__MAP_H__
