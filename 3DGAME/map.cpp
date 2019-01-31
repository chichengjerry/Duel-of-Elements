#include "map.h"
#include "util.h"

MAP::MAP(INT width, INT height)
{
	NODE* nodes = (NODE*)malloc(sizeof(NODE) * width * height);
	if (!nodes) {
		return;
	}

	memset(nodes, 0, sizeof(NODE) * width * height);

	this->width = width;
	this->height = height;
	this->nodes = nodes;
}

MAP::~MAP()
{
	free(nodes);
}

void MAP::FindPath(NODE * start, NODE * goal, NODE * path[])
{
	static HEAPARRAY heapClosedSet;
	static HEAPARRAY heapOpenSet;

	static NODE* neighbours[9];

	NODE* current;

	if (!path)
		return;

	// Create start position 
	current = start;

	// Add start point to open set
	heapOpenSet.Push(current);

	// Use manhattan distance
	current->pFrom = NULL;
	current->g_score = 0;
	current->f_score = NODE::ManhattanDistance(start, goal);

	while (heapOpenSet.length) {
		// Get the node with lowest cost(weight)
		current = heapOpenSet.Remove(0);

		// Is Goal
		if (current == goal) {

			// Reconstruct path
			NODE::ReconstructPath(path, current);

			return;
		}

		// Add current node to closed set
		heapClosedSet.Push(current);

		FindNeighbours(current, neighbours);

		for (int x, y = 0; y <= 2; y++) {
			for (x = 0; x <= 2; x++) {
				NODE* node = neighbours[y * 3 + x];
				if (node) {
					if (0 <= heapClosedSet.Find(node))
						continue;

					unsigned g_score = current->g_score + (abs(x) == abs(y) ? NODE_LENGTH_DIAGONAL : NODE_LENGTH) + node->weight;

					if (0 > heapOpenSet.Find(node)) {
						heapOpenSet.Push(node);
					}
					else if (g_score > node->g_score)
						continue;

					node->pFrom = current;
					node->g_score = g_score;
					node->f_score = g_score + NODE::ManhattanDistance(node, goal);
				}
			}
		}
	}
}

BOOL MAP::IsOutOfBound(INT x, INT y)
{
	return !(0 <= x && 0 <= y && width - 1 >= x && height - 1 >= y);
}

NODE * MAP::GetNode(INT x, INT y)
{
	if (!IsOutOfBound(x, y))
		return NULL;

	return nodes + y * width + x;
}

void MAP::FindNeighbours(NODE * pCurrentNode, NODE * pNeighbourNodes[])
{
	// Check all available node nodes
	NODE** pp;

	for (int x, y = 0; y <= 2; y++) {
		for (x = 0; x <= 2; x++) {
			pp = &pNeighbourNodes[y * 3 + x];
			*pp = GetNode(pCurrentNode->x + x - 1, pCurrentNode->y + y - 1);
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

HRESULT MAP::Draw()
{
	return E_NOTIMPL;
}

void MAP::Update()
{
}

void MAP::SetNode(INT x, INT y, NODE * newNode)
{
	if (IsOutOfBound(x, y))
		return;

	NODE* oldNode = GetNode(x, y);

	if (newNode)
		memcpy((void*)oldNode, newNode, sizeof(NODE));
	else
		memset((void*)oldNode, 0, sizeof(NODE));
}
