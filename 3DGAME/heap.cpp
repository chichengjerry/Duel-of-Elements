#include "heap.h"

void HEAPARRAY::SiftDown(int start, int end)
{
	int root = start;
	int left, right, swap;

	while (2 * root + 1 <= end) {
		left = 2 * root + 1;
		right = 2 * root + 2;
		swap = root;

		if (NODE::Compare(heap[swap], heap[left]))
			swap = left;

		if (right <= end && NODE::Compare(heap[swap], heap[right]))
			swap = right;

		if (root == swap)
			return;

		else {
			NODE::Swap(&heap[root], &heap[swap]);
			root = swap;
		}
	}
}

void HEAPARRAY::Heapify(void)
{
	int start = (length - 2) / 2;

	while (start >= 0) {
		SiftDown(start, length - 1);
		start--;
	}
}

int HEAPARRAY::Find(NODE * node)
{
	for (int i = length; i;) {
		if (heap[--i] == node) {
			return i;
		}
	}
	return -1;
}

void HEAPARRAY::Push(NODE * node)
{
	if (length < HEAP_MAX - 1) {
		heap[length] = node;
		length++;

		Heapify();
	}
}

NODE * HEAPARRAY::Remove(int index)
{
	if (index < 0 || index > length - 1)
		return NULL;

	Sort();

	NODE* node = heap[index];

	for (int i = index, k = length - 1; i < k; i++) {
		heap[i] = heap[i + 1];
	}
	heap[(--length)] = NULL;

	Heapify();

	return node;
}

void HEAPARRAY::Sort(void)
{
	int end;
	Heapify();

	end = length - 1;
	while (end > 0) {
		NODE::Swap(&heap[end], &heap[0]);

		SiftDown(0, --end);
	}
}
