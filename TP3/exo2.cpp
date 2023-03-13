#include "tp3.h"
#include <QApplication>
#include <time.h>

MainWindow* w = nullptr;
using std::size_t;

int binarySearch(Array& array, int toSearch, int start, int end, bool min)
{
	bool index = false;
	int result;
	while(start < end)
	{
		int mid = (start + end) / 2;

		if(toSearch > array[mid])
			start = mid + 1;
		else if(toSearch < array[mid])
			end = mid;
		else
		{
			// On veut trouver l'indexMin
			if(min)
				end = mid;
			// On veut trouver l'indexMax
			else
				start = mid + 1;

			index = true;
			result = mid;
		}
	}

	if(index)
		return result;
	else
		return -1;
}

/**
 * @brief define indexMin and indexMax as the first and the last index of toSearch
 * @param array array of int to process
 * @param toSearch value to find
 * @param indexMin first index of the value to find
 * @param indexMax last index of the value to find
 */
void binarySearchAll(Array& array, int toSearch, int& indexMin, int& indexMax)
{
	bool min = true;
	// do not use increments, use two different binary search loop
    indexMin = indexMax = -1;

    //Avec la première boucle on trouve la première occurence de la valeur dans le tableau si elle existe
    indexMin = binarySearch(array, toSearch, 0, array.size(), min);

    if(indexMin != -1)
    {
    	min = false;
    	indexMax = binarySearch(array, toSearch, indexMin, array.size(), false);
    }

}

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow::instruction_duration = 500;
	w = new BinarySearchAllWindow(binarySearchAll);
	w->show();

	return a.exec();
}
