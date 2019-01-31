/***********************************************
 * [map.h]
 * �ޥåׄI��
 * GP11A341 24 ���Y�G
 ***********************************************/

#ifndef __MAP_H__
#define __MAP_H__

#include "main.h"
#include "model.h"
#include "heap.h"
#include "node.h"

#define	MAP_MAX_POLYGON			1024	// �ݥꥴ����

typedef struct MAP {
	NODE* nodes;
	POLYGON* polygons[MAP_MAX_POLYGON] = {};

	int width;
	int height;

	MAP(INT width, INT height, DWORD* mapData);
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
