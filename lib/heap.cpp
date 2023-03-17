#include "heap.h"

#ifndef DISABLE_HEAP

Heap::Heap(uint size)
	: Array(size)
{}

Heap::Heap(const Heap &other)
	: Array(other)
{}

Heap::Heap(const std::vector<int> &other)
	: Array(other)
{}

#endif
