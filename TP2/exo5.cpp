#include <QApplication>
#include <time.h>
#include "tp2.h"

MainWindow* w=nullptr;

void merge(Array& first, Array& second, Array& result);

void splitAndMerge(Array& origin)
{
    if (origin.size() < 2)
        return;
    
    Array& first = w->newArray(origin.size() / 2);
    Array& second = w->newArray(origin.size() - first.size());
    
    for (int i = 0; i < origin.size(); i++) 
    {
        if (i < first.size()) 
            first[i] = origin[i];
        else
            second[i - first.size()] = origin[i];
    }
    
    splitAndMerge(first);
    splitAndMerge(second);
    
    merge(first, second, origin);
}

void merge(Array& first, Array& second, Array& result)
{
	int i = 0;
    int j = 0;
    int index = 0;
    
    while (i < first.size() && j < second.size()) 
    {
        if (first[i] <= second[j]) 
        {
            result[index] = first[i];
            i++;
        } else 
        {
            result[index] = second[j];
            j++;
        }
        index++;
    }
    
    while (i < first.size()) 
    {
        result[index] = first[i];
        i++;
        index++;
    }
    
    while (j < second.size()) 
    {
        result[index] = second[j];
        j++;
        index++;
    }
}

void mergeSort(Array& toSort)
{
    splitAndMerge(toSort);
}

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow::instruction_duration = 50;
    w = new TestMainWindow(mergeSort);
	w->show();

	return a.exec();
}
