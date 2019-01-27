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

typedef struct _map {
	int width;
	int height;
	NODE* nodes;


} MAP;

MAP*					map_init(MAP* pMap, int width, int height);
void					map_uninit(MAP* pMap);

void					map_aasterisk(const MAP* map, NODE* start, NODE* goal, NODE* path[]);
BOOL					map_out_of_bound(const MAP* map, int x, int y);
NODE*					map_get_node(const MAP* map, int x, int y);
void					map_find_neighbours(const MAP* map, NODE* pCurrentNode, NODE* pNeighbourNodes[]);

HRESULT					map_draw(const MAP* pMap);
void					map_update(const MAP* pMap, int x, int y, const NODE* new_node);

#endif // !__MAP_H__
