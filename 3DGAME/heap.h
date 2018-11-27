#ifndef _HEAP_H_
#define _HEAP_H_

#include "main.h"
#include "node.h"

#define HEAP_MAX			65536

typedef struct heap_t {
	node_t* heap[HEAP_MAX];
	int length = 0;
} heap_t;

int heap_find(node_t* node, heap_t * heap);

void heap_push(heap_t* heap, node_t* node);

node_t* heap_remove(heap_t* heap, const int index);

void heap_sort(heap_t* heap);

#endif // !_HEAP_H_
