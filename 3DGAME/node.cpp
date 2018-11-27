#include "node.h"

bool node_compare(const node_t * a, const node_t * b)
{
	return a->f_score < b->f_score;
}

void node_reconstruct_path(node_t* sequence[], node_t* current)
{
	for (int i = 0; current; i++) {
		sequence[i] = current;
		current = current->came_from;
	};
}

int node_manhattan_distance(node_t * a, node_t * b)
{
	return (abs(a->x - b->x) + abs(a->y - b->y)) * NODE_LENGTH;
}

void node_swap(node_t ** a, node_t ** b)
{
	node_t* temp = *a;
	*a = *b;
	*b = temp;
}
