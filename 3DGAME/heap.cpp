#include "heap.h"

int heap_find(NODE* pNode, HEAPARRAY * pHeapArray)
{
	for (int i = pHeapArray->length; i;) {
		if (pHeapArray->heap[--i] == pNode) {
			return i;
		}
	}
	return -1;
}

void heap_heapify(HEAPARRAY* pHeapArray)
{
	int start = (pHeapArray->length - 2) / 2;

	while (start >= 0) {
		heap_sift_down(pHeapArray, start, pHeapArray->length - 1);
		start--;
	}
}

void heap_push(HEAPARRAY* pHeapArray, NODE* pNode)
{
	if (pHeapArray->length < HEAP_MAX - 1) {
		pHeapArray->heap[pHeapArray->length] = pNode;
		pHeapArray->length++;

		heap_heapify(pHeapArray);
	}
}

NODE* heap_remove(HEAPARRAY* pHeapArray, int nNodeIndex)
{
	if (nNodeIndex < 0 || nNodeIndex > pHeapArray->length - 1)
		return NULL;

	heap_sort(pHeapArray);

	NODE* node = pHeapArray->heap[nNodeIndex];

	for (int i = nNodeIndex, k = pHeapArray->length - 1; i < k; i++) {
		pHeapArray->heap[i] = pHeapArray->heap[i + 1];
	}
	pHeapArray->heap[(--pHeapArray->length)] = NULL;

	heap_heapify(pHeapArray);

	return node;
}

void heap_sift_down(HEAPARRAY* pHeapArray, int nStartIndex, int nEndIndex)
{
	int root = nStartIndex;
	int left, right, swap;

	while (2 * root + 1 <= nEndIndex) {
		left = 2 * root + 1;
		right = 2 * root + 2;
		swap = root;

		if (node_compare(pHeapArray->heap[swap], pHeapArray->heap[left]))
			swap = left;

		if (right <= nEndIndex && node_compare(pHeapArray->heap[swap], pHeapArray->heap[right]))
			swap = right;

		if (root == swap)
			return;

		else {
			node_swap(&pHeapArray->heap[root], &pHeapArray->heap[swap]);
			root = swap;
		}
	}
}

void heap_sort(HEAPARRAY* heap)
{
	int end;
	heap_heapify(heap);

	end = heap->length - 1;
	while (end > 0) {
		node_swap(&heap->heap[end], &heap->heap[0]);

		heap_sift_down(heap, 0, --end);
	}
}
