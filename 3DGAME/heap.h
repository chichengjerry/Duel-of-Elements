#ifndef __HEAP_H__
#define __HEAP_H__

#include "main.h"
#include "node.h"

#define HEAP_MAX			65536

typedef struct HEAPARRAY {
	NODE*					heap[HEAP_MAX];
	int						length = 0;

	void					Heapify(void);
	int						Find(NODE* node);
	void					Push(NODE* node);
	NODE*					Remove(int index);
	void					SiftDown(int start, int end);
	void					Sort(void);
} HEAPARRAY;

#endif // !__HEAP_H__
