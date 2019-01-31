#ifndef _NODE_H_
#define _NODE_H_

#include "main.h"

#define NODE_LENGTH				10
#define NODE_LENGTH_DIAGONAL	14

typedef struct NODE {
	NODE*						pFrom;
	DWORD						fScore;
	DWORD						gScore;
	INT							weight;
	INT							x;
	INT							y;

	NODE(INT x, INT y, INT weight);

	static INT					ManhattanDistance(NODE* a, NODE* b);
	static FLOAT				EuclideanDistance(NODE* a, NODE* b);
	static BOOL					Compare(const NODE* a, const NODE* b);
	static void					ReconstructPath(NODE* path[], NODE* current);
	static void					Swap(NODE** a, NODE** b);
} NODE;

#endif // !_NODE_H_
