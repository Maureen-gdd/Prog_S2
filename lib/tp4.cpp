#include <QDebug>
#include <QTime>

#include "tp4.h"
#include "array.h"

#ifndef DISABLE_HEAP

class HeapChildsThread : public HeapThread
{

public:
    HeapChildsThread(MainWindow* mainWindow, Heap& tree, QObject *parent = nullptr)
        : HeapThread(mainWindow, tree, parent) {}
    void run() override;

    ~HeapChildsThread() override{}
};

class HeapInsertThread : public HeapThread
{

public:
    HeapInsertThread(MainWindow* mainWindow, Heap& tree, QObject *parent = nullptr)
        : HeapThread(mainWindow, tree, parent) {}
    void run() override;

    ~HeapInsertThread() override{}
};

class HeapHeapifyThread : public HeapThread
{

public:
    HeapHeapifyThread(MainWindow* mainWindow, Heap& tree, QObject *parent = nullptr)
        : HeapThread(mainWindow, tree, parent) {}
    void run() override;

    ~HeapHeapifyThread() override{}
};

class HeapSortThread : public HeapThread
{

public:
    HeapSortThread(MainWindow* mainWindow, Heap& tree, QObject *parent = nullptr)
        : HeapThread(mainWindow, tree, parent) {}
    void run() override;

    ~HeapSortThread() override{}
};

HeapWindow::HeapWindow(QWidget *parent) : MainWindow(parent),
    buildButton("Build"), indexingButton("Indexing"),
    heapifyButton("Heapify"), sortButton("Sort")
{
    this->heaps.push_back(Heap(1));
    this->heap = &heaps[heapItems.size()];

    this->addIntParam("n", qrand() % 20+ 10);

    startButton.hide();
    stopButton.hide();

    this->workerLayout->addWidget(&indexingButton);
    this->workerLayout->addWidget(&buildButton);
    this->workerLayout->addWidget(&heapifyButton);
    this->workerLayout->addWidget(&sortButton);

    threads.insert(&indexingButton, new HeapChildsThread(this, *heap, this));
    threads.insert(&buildButton, new HeapInsertThread(this, *heap, this));
    threads.insert(&heapifyButton, new HeapHeapifyThread(this, *heap, this));
    threads.insert(&sortButton, new HeapSortThread(this, *heap, this));

    connect(&buildButton, SIGNAL(clicked()), this, SLOT(runThread()));
    connect(&indexingButton, SIGNAL(clicked()), this, SLOT(runThread()));
    connect(&heapifyButton, SIGNAL(clicked()), this, SLOT(runThread()));
    connect(&sortButton, SIGNAL(clicked()), this, SLOT(runThread()));
}

void HeapWindow::runThread()
{
    QObject* source = sender();
    if (workerThread)
    {
        disconnect(workerThread, SIGNAL(finished()), this, SLOT(handleResult()));
        workerThread->quit();
        workerThread = nullptr;
    }
    if (source == &this->buildButton || source == &this->indexingButton)
    {
        int size = this->getParam("n").toInt();
        heap->resize(size);
        heap->fillRandom(0, 200);

        this->clearTrees();
        dirty = true;
    }

    QPushButton* button = dynamic_cast<QPushButton*>(source);
    if (button)
    {
        workerThread = this->threads[button];
        connect(workerThread, SIGNAL(finished()), this, SLOT(handleResult()));
        workerThread->start();
    }
}

void HeapWindow::clearTrees()
{
    for (QVector<HeapNumberGraphicsItem*>& items : heapItems)
    {
        for (HeapNumberGraphicsItem* item : items)
            delete item;
        items.clear();
    }
    heapItems.clear();
    this->scene.setSceneRect(0, 0, width(), height());
    updateScene();
}


Heap &HeapWindow::newRandomHeap(uint size)
{
	heaps.push_back(Heap(size));
	Heap& array = heaps.last();
	array.fillSortedRandom(0,500);
	this->dirty = true;
	std::this_thread::sleep_for(std::chrono::milliseconds(MainWindow::instruction_duration / 3));
	return array;
}

void HeapWindow::updateScene()
{
    while (heapItems.size() < heaps.size())
    {
        Heap& heap = heaps[heapItems.size()];
        heapItems.push_back(QVector<HeapNumberGraphicsItem*>());
        HeapNumberGraphicsItem* item = new HeapNumberGraphicsItem(&heap, 0);
        heapItems.last().push_back(item);
        toAdd.append(item);
        dirty = true;
    }

    int i=0;
    for (QVector<HeapNumberGraphicsItem*>& items : heapItems)
    {
        for (HeapNumberGraphicsItem* item : items)
        {
            if (item->needUpdate())
            {
                dirty = true;
                break;
            }
        }
        i++;
    }
    Base::updateScene();
}


void HeapWindow::updateLayout()
{
    int itemWidth = qMax<int>(50, width() * 0.01f);
    int maxX = 0, maxY = 0;
    updateStatusItem(itemWidth, maxX, maxY);
    updateBackground();
    updateNumberItems(itemWidth, maxX, maxY);
    updateHeapNumberItems(itemWidth, maxX, maxY);
    this->scene.setSceneRect(0, 0, qMax(width(), maxX), qMax(height(),maxY+itemWidth * 2));

    this->dirty=false;
}


void HeapWindow::updateHeapNumberItems(int itemWidth, int &maxX, int &maxY)
{
    for (QVector<HeapNumberGraphicsItem*>& items : heapItems)
    {
        for (HeapNumberGraphicsItem* item : items)
        {
            item->updateNode();
            int treeWidth = item->width();
            int startX = scene.width() * 0.45f;
            item->setRect(startX, maxY, itemWidth, itemWidth);
            item->updateLayout(maxX, maxY);
            maxY += item->height();
            if (startX + treeWidth > maxX)
            {
                maxX = startX + treeWidth;
            }
        }
    }
}


void HeapChildsThread::run()
{
	try {
		qsrand(time(nullptr));

        for (size_t i=0; i < this->heap.size() / 2; ++i)
		{
			int left = i * 2 + 1;
			int right = i * 2 + 2;
            if (this->heap.leftChildIndex(i) != left)
			{
                QString message("Left Child: Node %1 left child is not node %2 (value=%3) "
								"but node %4 (value=%5)");
				throw std::runtime_error(message.arg(i)
                                                .arg(this->heap.leftChildIndex(i))
                                                .arg(this->heap[this->heap.leftChildIndex(i)])
												.arg(left)
												.arg(this->heap[left])
												.toStdString());
			}
            else if (this->heap.rightChildIndex(i) != right) {
                QString message("Right Child: Node %1 right child is not node %2 (value=%3) "
								"but node %4 (value=%5)");
				throw std::runtime_error(message.arg(i)
                                                .arg(this->heap.rightChildIndex(i))
                                                .arg(this->heap[this->heap.rightChildIndex(i)])
												.arg(right)
												.arg(this->heap[right])
												.toStdString());
			}

		}
		_message = QString("Heap is well indexed !");
		success = true;
	}
	catch(std::exception& e)
	{
		_message = QString(e.what());
	}
}

class HeapNode : public Node
{
public:
    int data;
    HeapNode* left;
    HeapNode* right;

    HeapNode(int data) : Node(data)
    {
        this->left = nullptr;
        this->right = nullptr;
    }

    void initNode(int);
    int get_value() const;
    Node *get_left_child() const;
    Node *get_right_child() const;
};

HeapNumberGraphicsItem::HeapNumberGraphicsItem(Heap *heap, int i,
											   QGraphicsItem *parent, float leftShiftting,
											   float rightShiftting)
    : TreeNumberGraphicsItem(new HeapNode(heap->__get__(i)), parent, leftShiftting, rightShiftting),
      heap(heap), i(i)
{

}

void HeapNumberGraphicsItem::updateNode()
{
    if (!parentItem())
    {
        if (right)
            rightShifting = right->_treeHeight() + 1;

        if (left)
            leftShifting = left->_treeHeight() + 1;
    }
    if (data().toInt() != heap->__get__(i))
    {
        setData(heap->__get__(i));
    }

    int this_height = _treeHeight();
    if (i * 2 + 1 < heap->size())
    {
        if (!left)
            left = new HeapNumberGraphicsItem(heap, i * 2 + 1, this);
        left->leftShifting = leftShifting*(1.1f+left->_treeHeight()/this_height);
        left->rightShifting = rightShifting*0.75-leftShifting*0.2;
        left->updateNode();
    }
    if (i * 2 + 2 < heap->size())
    {
        if (!right)
            right = new HeapNumberGraphicsItem(heap, i * 2 + 2, this);
        right->rightShifting = rightShifting*(1.1f+right->_treeHeight()/this_height);
        right->leftShifting = leftShifting*0.75-rightShifting*0.2;
        right->updateNode();
    }
    update();
}

bool HeapNumberGraphicsItem::needUpdate() const
{
    bool update = TreeNumberGraphicsItem::needUpdate() || heap->__get__(i) != data().toInt();
    if (update)
        return true;
    if (left && left->needUpdate())
        return true;
    if (right && right->needUpdate())
        return true;
    return false;
}
void HeapInsertThread::run()
{
	try {
		qsrand(time(nullptr));


        for (size_t i=0; i<heap.size();++i)
			heap.__set__(i, -1);

        QTime myTimer;

        for (size_t i=0; i<heap.size();++i)
		{
            myTimer.start();
            int value = qrand() % 255;
            heap.insertHeapNode(i, value);
		}

		assertHeap();

		_message = QString("Nodes insertion correctly finished !");
		success = true;
	}
	catch(std::exception& e)
	{
        _message = "Insertion: " + QString(e.what());
	}
}

void HeapHeapifyThread::run()
{
	try {
		qsrand(time(nullptr));


        for (size_t i=0; i<heap.size();++i)
		{
			int value = qrand() % 255;
			heap.__set__(i, value);
		}
        QTime myTimer;

        for (int i=(int) heap.size() / 2 + 1; i>=0; i--)
		{
            myTimer.start();
            heap.heapify(heap.size(), i);
		}

		assertHeap();

		_message = QString("Nodes heapify correctly finished !");
		success = true;
	}
	catch(std::exception& e)
	{
        _message = "Heapify: " + QString(e.what());
	}
}

void HeapSortThread::run()
{
	try {
		heap.heapSort();

        for (size_t i=0; i<heap.size()-1;++i)
		{
			if (heap[i] > heap[i+1])
			{
                QString message("Sort: Node %1 (value=%2) is greater than Node %3 (value=%4)");
				throw std::runtime_error(message.arg(i)
												.arg(heap[i])
												.arg(i+1)
												.arg(heap[i+1])
												.toStdString());
			}
		}

		_message = QString("Array is correctly sorted !");
		success = true;
	}
	catch(std::exception& e)
	{
		_message = QString(e.what());
	}
}

void HeapThread::assertHeap()
{
    if (heap.effectiveSize() < 2)
    {
        QString message("Insertion: Heap is not correctly filled");
        throw std::runtime_error(message.toStdString());
    }
    for (size_t i=0; i * 2 + 1<heap.size(); i = i * 2 + 3)
	{
        for (size_t j=i; j>0; j = j / 2)
		{
			if (heap.__get__(i * 2 + 1) > heap.__get__(j))
			{
                QString message("Node %1 left child (value=%2) "
                                "is greater than parent %3 (value=%4)");
				throw std::runtime_error(message.arg(i)
												.arg(heap[i * 2 + 1])
												.arg(j)
												.arg(heap[j])
												.toStdString());
			}
			if (!j)
				break;
		}

		for (int j=i; j>=0; j = j / 2)
		{
			if (heap.__get__(i * 2 + 2) > heap.__get__(j))
			{
                QString message("Node %1 right child (value=%2) "
                                "is greater than parent %3 (value=%4)");
				throw std::runtime_error(message.arg(i)
												.arg(heap[i * 2 + 2])
												.arg(j)
												.arg(heap[j])
												.toStdString());
			}
			if (!j)
				break;
		}
	}
}

void HeapNode::initNode(int value)
{
    this->data = value;
}

int HeapNode::get_value() const
{
    return this->data;
}

Node* HeapNode::get_left_child() const
{
    return this->left;
}

Node* HeapNode::get_right_child() const
{
    return this->right;
}
#else

#endif
