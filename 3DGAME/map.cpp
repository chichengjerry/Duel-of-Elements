#include "map.h"
#include "util.h"

#define DEBUG	1

MAP* map_init(MAP* pMap, int width, int height)
{
	NODE* nodes = (NODE*)malloc(sizeof(NODE) * width * height);
	if (!nodes) {
		return NULL;
	}

	memset(nodes, 0, sizeof(NODE) * width * height);

	pMap->width = width;
	pMap->height = height;
	pMap->nodes = nodes;

	return pMap;
}

void map_uninit(MAP * pMap)
{
	free(pMap->nodes);
}

void map_find_neighbours(const MAP* pMap, NODE* pCurrentNode, NODE* pNeighbourNodes[])
{
	// Check all available node nodes
	NODE** pp;

	for (int x, y = 0; y <= 2; y++) {
		for (x = 0; x <= 2; x++) {
			pp = &pNeighbourNodes[y * 3 + x];
			*pp = map_get_node(pMap, pCurrentNode->x + x - 1, pCurrentNode->y + y - 1);
		}
	}

#define REMOVE_NEIGHBOUR(t, t1, t2) \
	if (pNeighbourNodes[t] && -1 == pNeighbourNodes[t]->weight) { \
		pp = &pNeighbourNodes[t1]; \
		*pp = NULL; \
		pp = &pNeighbourNodes[t2]; \
		*pp = NULL; \
	}

	REMOVE_NEIGHBOUR(1, 0, 2)
	REMOVE_NEIGHBOUR(3, 0, 6)
	REMOVE_NEIGHBOUR(5, 2, 8)
	REMOVE_NEIGHBOUR(7, 6, 8)

#undef REMOVE_NEIGHBOUR

	for (int i = 0; i < 9; i++) {
		pp = &pNeighbourNodes[i];
		if (*pp && -1 == (*pp)->weight) {
			// Remove this neighbour
			*pp = NULL;
			continue;
		}
	}
}

void map_aasterisk(const MAP* pMap, NODE* pStartNode, NODE* goal, NODE * path[])
{
	static HEAPARRAY closed_set;
	static HEAPARRAY open_set;

	static NODE* neighbours[9];

	NODE* current;

	if (!path || !pMap)
		return;

	// Create start position 
	current = pStartNode;

	// Add start point to open set
	heap_push(&open_set, current);

	// Use manhattan distance
	current->came_from = NULL;
	current->g_score = 0;
	current->f_score = node_manhattan_distance(pStartNode, goal);

	while (open_set.length) {
#if DEBUG
		system("cls");
		printf("\n");
		for (int y = 0; y < pMap->height; y++) {
			for (int x = 0; x < pMap->width; x++) {
				NODE* node = map_get_node(pMap, x, y);
				if (node == current) {
					printf("()");
					continue;
				}
				if (node->weight == -1) {
					printf("[]");
					continue;
				}
				if (heap_find(node, &open_set) >= 0) {
					printf("oo");
					continue;
				}
				if (heap_find(node, &closed_set) >= 0) {
					printf("xx");
					continue;
				}
				printf("  ");
			}
			printf("\n");
		}
		printf("\n");

#endif
		// Get the node with lowest cost(weight)
		current = heap_remove(&open_set, 0);

		// Is Goal
		if (current == goal) {

			// Reconstruct path
			node_reconstruct_path(path, current);

			return;
		}

		// Add current node to closed set
		heap_push(&closed_set, current);

		map_find_neighbours(pMap, neighbours, current);

		for (int x, y = 0; y <= 2; y++) {
			for (x = 0; x <= 2; x++) {
				NODE* node = neighbours[y * 3 + x];
				if (node) {
					if (0 <= heap_find(node, &closed_set))
						continue;

					unsigned g_score = current->g_score + (abs(x) == abs(y) ? NODE_LENGTH_DIAGONAL : NODE_LENGTH) + node->weight;

					if (0 > heap_find(node, &open_set)) {
						heap_push(&open_set, node);
					}
					else if (g_score > node->g_score)
						continue;

					node->came_from = current;
					node->g_score = g_score;
					node->f_score = g_score + node_manhattan_distance(node, goal);
				}
			}
		}
	}
}

BOOL map_out_of_bound(const MAP* weight_map, int x, int y)
{
	return !(0 <= x && 0 <= y && weight_map->width - 1 >= x && weight_map->height - 1 >= y);
}

NODE* map_get_node(const MAP* weight_map, int x, int y)
{
	if (!weight_map || map_out_of_bound(weight_map, x, y))
		return NULL;

	return weight_map->nodes + y * weight_map->width + x;
}

void map_update(const MAP* weight_map, int x, int y, const NODE* new_node)
{
	if (!weight_map || map_out_of_bound(weight_map, x, y))
		return;

	NODE* old_node = map_get_node(weight_map, x, y);

	if (new_node)
		memcpy((void*)old_node, new_node, sizeof(NODE));
	else
		memset((void*)old_node, 0, sizeof(NODE));
}

