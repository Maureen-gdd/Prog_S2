#include "tp1.h"
#include <QApplication>
#include <time.h>

#define return_and_display(result) return _.store(result);

int fibonacci(int value)
{
    Context _("fibonacci", value); // do not care about this, it allow the display of call stack

    switch (value) 
    {
        case 0:
            return 0;
            break;
        case 1:
            return 1;
            break;
        default:
            return fibonacci(value - 2) + fibonacci(value - 1);
            break;
    }

    return_and_display(value);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);  // create a window manager
    MainWindow::instruction_duration = 400;  // make a pause between instruction display
    MainWindow* w = new FibonacciWindow(fibonacci); // create a window for this exercice
    w->show(); // show exercice

    return a.exec(); // main loop while window is opened
}
