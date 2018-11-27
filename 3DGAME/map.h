#ifndef _MAP_H_
#define _MAP_H_

#include "main.h"
#include "heap.h"
#include "node.h"

typedef struct map_t {
	int width;
	int height;
	node_t* nodes;
} map_t;

map_t*			map_create(int width, int height);
void			map_find_path_aa(node_t* path[], const map_t *node_map, node_t* start, node_t* goal);
bool			map_out_of_bound(const map_t* node_map, int x, int y);
node_t*			map_read(const map_t* weight_map, int x, int y);
void			map_find_neighbours(const map_t* map, node_t* neighbours[], node_t* current);
void			map_update(const map_t* weight_map, int x, int y, const node_t* new_node);

#endif // !_MAP_H_
