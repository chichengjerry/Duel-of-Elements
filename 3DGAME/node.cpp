#include "node.h"

bool node_compare(const NODE * a, const NODE * b)
{
	return a->f_score < b->f_score;
}

void node_reconstruct_path(NODE* sequence[], NODE* current)
{
	for (int i = 0; current; i++) {
		sequence[i] = current;
		current = current->came_from;
	};
}

int node_manhattan_distance(NODE * a, NODE * b)
{
	return (abs(a->x - b->x) + abs(a->y - b->y)) * NODE_LENGTH;
}

float node_euclidean_distance(NODE * a, NODE * b)
{
	float x = (a->x - b->x);
	float y = (a->y - b->y);
	return sqrtf(x * x + y * y) * NODE_LENGTH;
}

void node_swap(NODE ** a, NODE ** b)
{
	NODE* temp = *a;
	*a = *b;
	*b = temp;
}
