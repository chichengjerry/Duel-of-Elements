#ifndef _NODE_H_
#define _NODE_H_

#include "main.h"

#define NODE_LENGTH				10
#define NODE_LENGTH_DIAGONAL	14

typedef struct _node {
	NODE* came_from;
	unsigned f_score;
	unsigned g_score;
	int weight;
	int x;
	int y;
} NODE;

int node_manhattan_distance(NODE* a, NODE* b);

float node_euclidean_distance(NODE* a, NODE* b);

bool node_compare(const NODE* a, const NODE* b);

void node_reconstruct_path(NODE* path[], NODE* current);

void node_swap(NODE** a, NODE** b);

#endif // !_NODE_H_
