#ifndef _NODE_H_
#define _NODE_H_

#include "main.h"

#define NODE_LENGTH				10
#define NODE_LENGTH_DIAGONAL	14

typedef struct node_t {
	node_t* came_from;
	unsigned f_score;
	unsigned g_score;
	int weight;
	int x;
	int y;
} node_t;

int node_manhattan_distance(node_t* a, node_t* b);

float node_euclidean_distance(node_t* a, node_t* b);

bool node_compare(const node_t* a, const node_t* b);

void node_reconstruct_path(node_t* path[], node_t* current);

void node_swap(node_t** a, node_t** b);

#endif // !_NODE_H_
