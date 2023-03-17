#include "HuffmanNode.h"
#include "tp4_exo2.h"

HuffmanThread::HuffmanThread(HuffmanMainWindow *mainWindow, TestThread::ThreadFunctionType function, QObject *parent)
    : TestThread(mainWindow, function, parent) {}

void HuffmanThread::run(){
    HuffmanMainWindow* mainWindow = dynamic_cast<HuffmanMainWindow*>(this->mainWindow);
    try
    {
        qsrand(time(nullptr));
        function(mainWindow->huffmanTree);
    }
    catch(std::exception& e)
    {
        _message = QString(e.what());
        success = false;
    }
}

void HuffmanMainWindow::updateScene()
{
    _TestMainWindow::updateScene();
    if (this->huffmanTree && !this->nodes.size())
    {
        nodes.push_back(this->huffmanTree);
    }
}


void displayCharacterFrequences(Array frequences)
{
    bool atLeastOne = false;
    qDebug("Frequences:");
    for (unsigned char i=0u; i<255u; i++)
    {
        if (frequences[i])
        {
            qDebug("\t%c: %d\n", i, frequences[i]);
            atLeastOne = true;
        }
    }
    if (!atLeastOne)
        qDebug("There is no frequences processed !");
}


