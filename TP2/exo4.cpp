#include <QApplication>
#include <time.h>

#include "tp2.h"


MainWindow* w=nullptr;

void recursivQuickSort(Array& toSort, int size)
{
	if (size < 2)
        return;
    
    Array& lowerArray = w->newArray(size);
    Array& greaterArray = w->newArray(size);
    int lowerSize = 0, greaterSize = 0;
    
    int pivot = toSort[0];
    
    for (int i = 1; i < size; i++) 
    {
        if (toSort[i] <= pivot)
            lowerArray[lowerSize++] = toSort[i];
        else
            greaterArray[greaterSize++] = toSort[i];
    }
    
    recursivQuickSort(lowerArray, lowerSize);
    recursivQuickSort(greaterArray, greaterSize);
    
    int index = 0;
    
    for (int i = 0; i < lowerSize; i++) 
    {
        toSort[index++] = lowerArray[i];
    }
    
    toSort[index++] = pivot;
    
    for (int i = 0; i < greaterSize; i++) 
    {
        toSort[index++] = greaterArray[i];
    }
}

void quickSort(Array& toSort)
{
	recursivQuickSort(toSort, toSort.size());
}


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	uint elementCount=20;
	MainWindow::instruction_duration = 50;
    w = new TestMainWindow(quickSort);
	w->show();

	return a.exec();
}
