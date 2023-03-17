#ifndef TP4_EXO2_H
#define TP4_EXO2_H

#include "tp4.h"

class HuffmanMainWindow : public _TestMainWindow
{
    Q_OBJECT
    friend class HuffmanThread;

public:
    typedef HuffmanThread::ThreadFunctionType HuffmanFunctionType;

    HuffmanMainWindow(HuffmanFunctionType function, QWidget *parent=nullptr)
        : _TestMainWindow(parent)
    {
        workerThread = new HuffmanThread(this, function, this);
        connect(workerThread, SIGNAL(finished()), this, SLOT(handleResult()));
    }
    void updateScene() override;
private:
    HuffmanNode* huffmanTree=nullptr;
};

#endif // TP4_EXO2_H
