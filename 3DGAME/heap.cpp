#include "heap.h"


void heap_sift_down(heap_t* heap, int start, int end);

int heap_find(node_t* node, heap_t * heap)
{
	for (int i = heap->length; i;) {
		if (heap->heap[--i] == node) {
			return i;
		}
	}
	return -1;
}

void heap_heapify(heap_t* heap)
{
	int start = (heap->length - 2) / 2;

	while (start >= 0) {
		heap_sift_down(heap, start, heap->length - 1);
		start--;
	}
}

void heap_push(heap_t* heap, node_t* node)
{
	if (heap->length < HEAP_MAX - 1) {
		heap->heap[heap->length] = node;
		heap->length++;

		heap_heapify(heap);
	}
}

node_t* heap_remove(heap_t* heap, const int index)
{
	if (index < 0 || index > heap->length - 1)
		return NULL;

	heap_sort(heap);

	node_t* node = heap->heap[index];
	// Copy the data of node node_current

	for (int i = index, k = heap->length - 1; i < k; i++) {
		heap->heap[i] = heap->heap[i + 1];
	}
	heap->heap[(--heap->length)] = NULL;

	heap_heapify(heap);

	return node;
}

void heap_sift_down(heap_t* heap, int start, int end)
{
	int root = start;
	int left, right, swap;

	while (2 * root + 1 <= end) {
		left = 2 * root + 1;
		right = 2 * root + 2;
		swap = root;

		if (node_compare(heap->heap[swap], heap->heap[left]))
			swap = left;

		if (right <= end && node_compare(heap->heap[swap], heap->heap[right]))
			swap = right;

		if (root == swap)
			return;

		else {
			node_swap(&heap->heap[root], &heap->heap[swap]);
			root = swap;
		}
	}
}

void heap_sort(heap_t* heap)
{
	int end;
	heap_heapify(heap);

	end = heap->length - 1;
	while (end > 0) {
		node_swap(&heap->heap[end], &heap->heap[0]);

		heap_sift_down(heap, 0, --end);
	}
}
