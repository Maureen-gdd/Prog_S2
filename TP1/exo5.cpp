#include "tp1.h"
#include <QApplication>
#include <time.h>

int isMandelbrot(Point z, int n, Point point){
    // recursiv Mandelbrot

    // check n

    // check length of z
    // if Mandelbrot, return 1 or n (check the difference)
    // otherwise, process the square of z and recall
    // isMandebrot

    // z avec z.x partie réelle et z.y partie imaginaire
    float tempX = z.x;
    float tempY = z.y;

    // z au carré avec:
    // a = x^2 - y^2
    // b = 2 * x * y
    z.x = (tempX * tempX - tempY * tempY);
    z.y = 2 * tempX * tempY;

    // z^2 + point
    z.x += point.x;
    z.y += point.y;

    // module
    float module = sqrt(z.x * z.x + z.y * z.y);

    if (module > 2) 
    {
        // vrai
        return 1;
    } else if (n == 0) 
    {
        // faux
        return 0;
    }

    return isMandelbrot(z, n - 1, point);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow* w = new MandelbrotWindow(isMandelbrot);
    w->show();

    a.exec();
}



