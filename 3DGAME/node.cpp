#include "node.h"

INT NODE::ManhattanDistance(NODE * a, NODE * b)
{
	return (abs(a->x - b->x) + abs(a->y - b->y)) * NODE_LENGTH;
}

FLOAT NODE::EuclideanDistance(NODE * a, NODE * b)
{
	float x = (a->x - b->x);
	float y = (a->y - b->y);
	return sqrtf(x * x + y * y) * NODE_LENGTH;
}

BOOL NODE::Compare(const NODE * a, const NODE * b)
{
	return a->f_score < b->f_score;
}

void NODE::ReconstructPath(NODE * path[], NODE * current)
{
	for (int i = 0; current; i++) {
		path[i] = current;
		current = current->pFrom;
	};
}

void NODE::Swap(NODE ** a, NODE ** b)
{
	NODE* temp = *a;
	*a = *b;
	*b = temp;
}
