#ifndef __HEAP_H__
#define __HEAP_H__

#include "main.h"
#include "node.h"

#define HEAP_MAX			65536

typedef struct _heap {
	NODE* heap[HEAP_MAX];
	int length = 0;
} HEAPARRAY;

void heap_sift_down(HEAPARRAY* pHeapArray, int nStartIndex, int nEndIndex);

int heap_find(NODE* pNode, HEAPARRAY* heapArray);

void heap_push(HEAPARRAY* heapArray, NODE* pNode);

NODE* heap_remove(HEAPARRAY* heapArray, int nNodeIndex);

void heap_sort(HEAPARRAY* heapArray);

#endif // !__HEAP_H__
