#ifndef UTILS_H
#define UTILS_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <cmath>
#include <chrono>

inline void custom_msleep(int milliseconds)
{
    QWaitCondition wc;
    QMutex mutex;
    QMutexLocker locker(&mutex);
    wc.wait(&mutex, milliseconds);
}

inline void custom_msleep(std::chrono::milliseconds period)
{
    QWaitCondition wc;
    QMutex mutex;
    QMutexLocker locker(&mutex);
    wc.wait(&mutex, period.count());
}


#endif // UTILS_H
