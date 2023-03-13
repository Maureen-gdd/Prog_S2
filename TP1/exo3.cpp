#include "tp1.h"
#include <QApplication>
#include <time.h>

#define return_and_display(result) return _.store(result);


int search(int value, Array& toSort, int size)
{
    Context _("search", value, size); // do not care about this, it allow the display of call stack

    // your code
    // check the last cell and if it does not correspond
    // then consider a smaller array when recalling search

    if (toSort[size - 1] == value)
    {
        return_and_display(size - 1);
    }
    else
    {
        // in case the value is not in te array
        if(size == 0)
        {
            return_and_display(-1);
        }
        else
        {
            return_and_display(search(value, toSort, size - 1));
        }
    }

}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);  // create a window manager
    MainWindow::instruction_duration = 400;  // make a pause between instruction display
    MainWindow* w = new SearchWindow(search); // create a window for this exercice
    w->show(); // show exercice

    return a.exec(); // main loop while window is opened
}




