#ifndef HEAP_H
#define HEAP_H

#include "array.h"

#ifndef DISABLE_HEAP
class Heap : public Array
{
public:
	Heap(uint size=100);
	Heap(const Heap& other);
	Heap(const std::vector<int>& other);

    int leftChildIndex(int nodeIndex);

    int rightChildIndex(int nodeIndex);

	void insertHeapNode(int heapSize, int value);

	void heapify(int heapSize, int nodeIndex);

	void buildHeap(Array& numbers);

	void heapSort();
};

//class HuffmanHeap : public Heap
//{
//public:
//	HuffmanHeap(uint size=100);
//	HuffmanHeap(const HuffmanHeap& other);

//	char getCharacter(const uint i);
//	void setCharacter(const uint i, char c);

//	void resize(uint size);

//private:
//	std::vector<char> characters;
//};

#endif // DISABLE_HEAP
#endif // HEAP_H
