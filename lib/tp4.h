#ifndef TP4_H
#define TP4_H

#include <QMap>

#include "mainwindow.h"
#include "heap.h"
#include "thread.h"
#include "tp3.h"

#ifndef DISABLE_HEAP
class HeapNumberGraphicsItem : public TreeNumberGraphicsItem
{
public:
	HeapNumberGraphicsItem(Heap* heap, int i, QGraphicsItem* parent=nullptr,
						   float leftShiftting=0, float rightShiftting=0);
    void updateNode() override;
    bool needUpdate() const override;

private:
	Heap* heap;
    uint i;
};


class HeapThread : public Thread
{

public:
	HeapThread(MainWindow* mainWindow, Heap& tree, QObject *parent = nullptr)
		: Thread(parent), mainWindow(mainWindow), success(false), heap(tree) {}

	virtual bool succeeded() const {return success;}
	virtual const QString& message() const {return _message;}
	~HeapThread() override{}

	void assertHeap();
protected:
	MainWindow* mainWindow;
	QString _message;
	bool success;

	Heap& heap;
};


class HeapWindow : public MainWindow
{
    Q_OBJECT

public:
    typedef MainWindow Base;

    HeapWindow(QWidget *parent=nullptr);
    ~HeapWindow() {}
    Heap& newRandomHeap(uint size);

    void updateScene() override;
    void updateLayout() override;
    void clearTrees();
    void updateHeapNumberItems(int itemWidth, int &maxY, int &maxX);

public slots:
    void runThread();

private:
	QVector<Heap> heaps;
	Heap* heap;
    QMap<QObject*, Thread*> threads;

    QPushButton buildButton;
    QPushButton indexingButton;
    QPushButton heapifyButton;
    QPushButton sortButton;

    QVector<QVector<HeapNumberGraphicsItem*> > heapItems;
};
#endif // DISABLE_HEAP

struct HuffmanNode;
class HuffmanMainWindow;

class HuffmanThread : public TestThread<std::function<void(HuffmanNode*&)> >
{
public:
    HuffmanThread(HuffmanMainWindow* mainWindow, HuffmanThread::ThreadFunctionType function,
               QObject *parent = nullptr);

    virtual void run() override;
};

void displayCharacterFrequences(Array frequences);

#endif // TP4_H
