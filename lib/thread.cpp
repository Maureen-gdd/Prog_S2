#include "thread.h"

void NoParameterThread::run() {
    try
    {
        qsrand(time(nullptr));
        function();
    }
    catch(std::exception& e)
    {
        _message = QString(e.what());
        success = false;
    }
}
