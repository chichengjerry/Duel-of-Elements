#include "map.h"
#define DEBUG	1

map_t* map_create(int width, int height)
{
	map_t* weight_map = (map_t*)malloc(sizeof(map_t));
	if (!weight_map)
		return NULL;

	node_t* nodes = (node_t*)malloc(sizeof(node_t) * width * height);
	if (!nodes) {
		free(weight_map);
		return NULL;
	}

	memset(nodes, 0, sizeof(node_t) * width * height);

	weight_map->width = width;
	weight_map->height = height;
	weight_map->nodes = nodes;

	return weight_map;
}

void map_delete(map_t * map)
{
	if (map) {
		free(map->nodes);
		free(map);
	}
}

void map_find_neighbours(const map_t* map, node_t* neighbours[], node_t* current)
{
	// Check all available node nodes
	node_t** pp;

	for (int x, y = 0; y <= 2; y++) {
		for (x = 0; x <= 2; x++) {
			pp = &neighbours[y * 3 + x];
			*pp = map_read(map, current->x + x - 1, current->y + y - 1);

		}
	}

#define REMOVE_NEIGHBOUR(t, t1, t2) \
	if (neighbours[t] && -1 == neighbours[t]->weight) { \
		pp = &neighbours[t1]; \
		*pp = NULL; \
		pp = &neighbours[t2]; \
		*pp = NULL; \
	}

	REMOVE_NEIGHBOUR(1, 0, 2)
	REMOVE_NEIGHBOUR(3, 0, 6)
	REMOVE_NEIGHBOUR(5, 2, 8)
	REMOVE_NEIGHBOUR(7, 6, 8)

#undef REMOVE_NEIGHBOUR

	for (int i = 0; i < 9; i++) {
		pp = &neighbours[i];
		if (*pp && -1 == (*pp)->weight) {
			// Remove this neighbours
			*pp = NULL;
			continue;
		}
	}
}

void map_find_path_aa(node_t * path[], const map_t* map, node_t* start, node_t* goal)
{
	static heap_t closed_set;
	static heap_t open_set;

	static node_t* neighbours[9];

	node_t* current;

	if (!path || !map)
		return;

	// Create start position 
	current = start;

	// Add start point to open set
	heap_push(&open_set, current);

	// Use manhattan distance
	current->came_from = NULL;
	current->g_score = 0;
	current->f_score = node_manhattan_distance(start, goal);

	while (open_set.length) {
#if DEBUG
		system("cls");
		printf("\n");
		for (int y = 0; y < map->height; y++) {
			for (int x = 0; x < map->width; x++) {
				node_t* node = map_read(map, x, y);
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

		map_find_neighbours(map, neighbours, current);

		for (int x, y = 0; y <= 2; y++) {
			for (x = 0; x <= 2; x++) {
				node_t* node = neighbours[y * 3 + x];
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

bool map_out_of_bound(const map_t* weight_map, int x, int y)
{
	return !(0 <= x && 0 <= y && weight_map->width - 1 >= x && weight_map->height - 1 >= y);
}

node_t* map_read(const map_t* weight_map, int x, int y)
{
	if (!weight_map || map_out_of_bound(weight_map, x, y))
		return NULL;

	return weight_map->nodes + y * weight_map->width + x;
}

void map_update(const map_t* weight_map, int x, int y, const node_t* new_node)
{
	if (!weight_map || map_out_of_bound(weight_map, x, y))
		return;

	node_t* old_node = map_read(weight_map, x, y);

	if (new_node)
		memcpy((void*)old_node, new_node, sizeof(node_t));
	else
		memset((void*)old_node, 0, sizeof(node_t));
}

