#include "tp4.h"
#include "mainwindow.h"

#include <QApplication>
#include <time.h>
#include <stdio.h>
#include <algorithm> 

MainWindow* w = nullptr;
using std::size_t;
using std::string;

int Heap::leftChildIndex(int nodeIndex)
{
    return nodeIndex * 2 + 1;
}

int Heap::rightChildIndex(int nodeIndex)
{
    return nodeIndex * 2 + 2;
}

void Heap::insertHeapNode(int heapSize, int value)
{
	// use (*this)[i] or this->get(i) to get a value at index i
	size_t i = heapSize;
	this->get(i) = value;
	while(i > 0 && this->get(i) > this->get((i - 1) / 2))
	{
		std::swap(this->get(i), this->get((i - 1) / 2));
		i = (i - 1) / 2;
	}
}

// Si le noeud à l’indice nodeIndex n’est pas supérieur à ses enfants, reconstruit le tas à partir de cette index.
void Heap::heapify(int heapSize, int nodeIndex)
{
	
	size_t i_max = nodeIndex;

	if(leftChildIndex(nodeIndex) < heapSize && this->get(leftChildIndex(nodeIndex)) > this->get(i_max))
		i_max = leftChildIndex(nodeIndex);

	if(rightChildIndex(nodeIndex) < heapSize && this->get(rightChildIndex(nodeIndex)) > this->get(i_max))
		i_max = rightChildIndex(nodeIndex);

	if(i_max != nodeIndex)
	{
		std::swap(this->get(nodeIndex), this->get(i_max));
		heapify(heapSize, i_max);
	}
}

// Construit un tas à partir des valeurs de numbers
void Heap::buildHeap(Array& numbers)
{
	for(size_t i = 0; i < numbers.size(); i++)
		insertHeapNode(numbers.size(), numbers[i]);
}

// Construit un tableau trié à partir d’un tas heap
void Heap::heapSort()
{
	for(size_t i = this->size() - 1; i > 0; i--)
	{
		std::swap(this->get(0), this->get(i));
		heapify(i, 0);
	}
}

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
    MainWindow::instruction_duration = 50;
    w = new HeapWindow();
	w->show();

	return a.exec();
}
