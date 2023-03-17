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
	int i = heapSize;
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
	
	int i_max = nodeIndex;
}

void Heap::buildHeap(Array& numbers)
{

}

void Heap::heapSort()
{

}

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
    MainWindow::instruction_duration = 50;
    w = new HeapWindow();
	w->show();

	return a.exec();
}
