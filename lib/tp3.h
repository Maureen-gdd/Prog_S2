#ifndef TP3_H
#define TP3_H

#include "mainwindow.h"
#include "array.h"
#include "binarytree.h"
#include "thread.h"

class TreeNumberGraphicsItem;

class _TestMainWindow : public MainWindow
{
    Q_OBJECT

public:
    typedef MainWindow Base;

    _TestMainWindow(QWidget *parent=nullptr);
    void addBinaryNode(Node* node);
    void updateScene();
    void updateTreeItems(int itemWidth, int &maxY, int &maxX);
    void updateLayoutItems(int itemWidth, int& originX, int& originY) override;
    void clearTrees();

    static float xFactor;
    static float yFactor;

protected:
    QVector<Node*> nodes;
    QLinkedList<TreeNumberGraphicsItem *> treeItems;

public slots:
    virtual void runThread() {};
};

template <class ThreadType>
class TestMainWindow : public MainWindow
{

public:
    typedef ThreadType WindowThreadType;
    typedef typename ThreadType::ThreadFunctionType FunctionType;

    TestMainWindow(FunctionType function, QWidget *parent=nullptr)
        : MainWindow(parent)
    {
        workerThread = new WindowThreadType(this, function, this);
        connect(workerThread, SIGNAL(finished()), this, SLOT(handleResult()));
    }
};


class NodeGraphicsItem : public QGraphicsEllipseItem
{
public:
    explicit NodeGraphicsItem(const int _number, QGraphicsItem* parent=nullptr);
    explicit NodeGraphicsItem(const std::string& data, QGraphicsItem* parent=nullptr);
    explicit NodeGraphicsItem(const QVariant& data, QGraphicsItem* parent=nullptr);

    void setData(const QVariant& data);
    void setData(const int _number);
    void setData(const std::string& data);
    void setData(const char* data);

    void paint(QPainter* painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget=nullptr) override;

    void displayDefault();
    void displayReadenState();
    void displayWrittenState();
    QVariant data() const;

private:
    enum State
    {
        DEFAULT=0,
        READEN,
        WRITEN,
        STATE_COUNT
    };
    void displayDefault(bool force);

    QVariant _data;
    State state;
};


class TreeNumberGraphicsItem : public NodeGraphicsItem
{
public:
    typedef	NodeGraphicsItem Base;

    explicit TreeNumberGraphicsItem(Node* node, QGraphicsItem* parent=nullptr, float leftShiftting=0, float rightShiftting=0);
    ~TreeNumberGraphicsItem();

    void setNode(Node* node);
    Node* node() const;

    int width() const;
    int height() const;
    int _treeHeight() const;

    virtual bool needUpdate() const;
    virtual void updateNode();
    void updateLayout(int& maxX, int& maxY);

    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget=nullptr) override;

    void dropChildren();


    Node* _node;
    TreeNumberGraphicsItem* left;
    TreeNumberGraphicsItem* right;

    float leftShifting=0;
    float rightShifting=0;
};


class BinarySearchThread : public TestThread<std::function<int(Array&, int)> >
{

public:
    BinarySearchThread(MainWindow* mainWindow, ThreadFunctionType function, QObject *parent = nullptr)
        : TestThread<ThreadFunctionType>(mainWindow, function, parent)
    {
        mainWindow->addIntParam("n", 10 + qrand() % 10);
        mainWindow->addIntParam("index", (qrand() % 20) - 1);
    }
    void run() override;
};


class BinarySearchWindow : public TestMainWindow<BinarySearchThread>
{
    Q_OBJECT

public:
    BinarySearchWindow(BinarySearchThread::ThreadFunctionType function, QWidget* parent=nullptr)
        : TestMainWindow<BinarySearchThread>(function, parent)
    {}
};


class BinarySearchAllThread : public TestThread<std::function<void(Array&, int, int&, int&)> >
{

public:
    BinarySearchAllThread(MainWindow* mainWindow, ThreadFunctionType function, QObject *parent = nullptr)
        : TestThread<ThreadFunctionType>(mainWindow, function, parent)
    {
        mainWindow->addIntParam("n", 10 + qrand() % 10);
        mainWindow->addIntParam("index", (qrand() % 20) - 1);
    }

    void run() override;
};


class BinarySearchAllWindow : public TestMainWindow<BinarySearchAllThread>
{
    Q_OBJECT

public:
    BinarySearchAllWindow(BinarySearchAllThread::ThreadFunctionType function, QWidget* parent=nullptr)
        : TestMainWindow<BinarySearchAllThread>(function, parent) {}
};


class BinarySearchTreeThread : public Thread
{

public:
    BinarySearchTreeThread(_TestMainWindow* mainWindow, BinaryTree& tree, QObject *parent = nullptr)
        : Thread(parent), mainWindow(mainWindow), success(false), tree(tree) {}

    virtual bool succeeded() const {return success;}
    virtual const QString& message() const {return _message;}
    ~BinarySearchTreeThread() override{}
protected:
    _TestMainWindow* mainWindow;
    QString _message;
    bool success;

    BinaryTree& tree;
};

class BinarySearchBuildThread : public BinarySearchTreeThread
{
public:
    BinarySearchBuildThread(_TestMainWindow* mainWindow, BinaryTree& tree, QObject *parent = nullptr)
        : BinarySearchTreeThread(mainWindow, tree, parent)
    {
        mainWindow->addIntParam("n", 10 + qrand() % 20);
    }
    void run() override;
    ~BinarySearchBuildThread() override{}

};


class BinarySearchTreeHeightThread : public BinarySearchTreeThread
{

public:
    BinarySearchTreeHeightThread(_TestMainWindow* mainWindow, BinaryTree& tree, QObject *parent = nullptr)
        : BinarySearchTreeThread(mainWindow, tree, parent) {}
    void run() override;

    ~BinarySearchTreeHeightThread() override{}
private:
    uint treeHeight(const BinaryTree& tree) const;
};


class BinarySearchTreeCountThread : public BinarySearchTreeThread
{

public:
    BinarySearchTreeCountThread(_TestMainWindow* mainWindow, BinaryTree& tree, QObject *parent = nullptr)
        : BinarySearchTreeThread(mainWindow, tree, parent) {}
    void run() override;

    ~BinarySearchTreeCountThread() override{}
};


class BinarySearchTreeLeafThread : public BinarySearchTreeThread
{

public:
    BinarySearchTreeLeafThread(_TestMainWindow* mainWindow, BinaryTree& tree, QObject *parent = nullptr)
        : BinarySearchTreeThread(mainWindow, tree, parent) {}
    void run() override;

    ~BinarySearchTreeLeafThread() override{}
private:
    const Node* assertLeaf(const BinaryTree& tree) const;
};


void assertNodesOrder(Node* nodes1[], Node* nodes2[], size_t count);


class BinarySearchTreeInorderThread : public BinarySearchTreeThread
{

public:
    BinarySearchTreeInorderThread(_TestMainWindow* mainWindow, BinaryTree& tree, QObject *parent = nullptr)
        : BinarySearchTreeThread(mainWindow, tree, parent) {}
    void run() override;

    ~BinarySearchTreeInorderThread() override{}

private:
    void _assert(Node* nodes[], size_t count) const;
};


class BinarySearchTreePreorderThread : public BinarySearchTreeThread
{

public:
    BinarySearchTreePreorderThread(_TestMainWindow* mainWindow, BinaryTree& tree, QObject *parent = nullptr)
        : BinarySearchTreeThread(mainWindow, tree, parent) {}
    void run() override;

    ~BinarySearchTreePreorderThread() override{}

private:
    void _assert(Node* nodes[], size_t count) const;
};


class BinarySearchTreePostorderThread : public BinarySearchTreeThread
{

public:
    BinarySearchTreePostorderThread(_TestMainWindow* mainWindow, BinaryTree& tree, QObject *parent = nullptr)
        : BinarySearchTreeThread(mainWindow, tree, parent) {}
    void run() override;

    ~BinarySearchTreePostorderThread() override{}

private:
    void _assert(Node* nodes[], size_t count) const;
};


class BinarySearchTreeSearchThread : public BinarySearchTreeThread
{

public:
    BinarySearchTreeSearchThread(_TestMainWindow* mainWindow, BinaryTree& tree, QObject *parent = nullptr)
        : BinarySearchTreeThread(mainWindow, tree, parent) {}
    void run() override;

    ~BinarySearchTreeSearchThread() override{}
};


template<typename TreeType>
class BinarySearchTreeWindow : public _TestMainWindow
{

public:
    typedef MainWindow Base;

    BinarySearchTreeWindow(QWidget *parent=nullptr)
        : _TestMainWindow(parent), tree(0), buildButton("Build"), heightButton("Height"),
          countButton("Count"), leafButton("Leafs"),
          inOrderButton("Inorder Travel"), preOrderButton("Preorder Travel"),
          postOrderButton("Postorder Travel"), searchButton("Search")
    {
        startButton.hide();
        stopButton.hide();

        this->workerLayout->addWidget(&buildButton);
        this->workerLayout->addWidget(&heightButton);
        this->workerLayout->addWidget(&countButton);
        this->workerLayout->addWidget(&leafButton);
        this->workerLayout->addWidget(&inOrderButton);
        this->workerLayout->addWidget(&preOrderButton);
        this->workerLayout->addWidget(&postOrderButton);
        this->workerLayout->addWidget(&searchButton);

        threads.insert(&buildButton, new BinarySearchBuildThread(this, tree, this));
        threads.insert(&heightButton, new BinarySearchTreeHeightThread(this, tree, this));
        threads.insert(&countButton, new BinarySearchTreeCountThread(this, tree, this));
        threads.insert(&leafButton, new BinarySearchTreeLeafThread(this, tree, this));
        threads.insert(&inOrderButton, new BinarySearchTreeInorderThread(this, tree, this));
        threads.insert(&preOrderButton, new BinarySearchTreePreorderThread(this, tree, this));
        threads.insert(&postOrderButton, new BinarySearchTreePostorderThread(this, tree, this));
        threads.insert(&searchButton, new BinarySearchTreeSearchThread(this, tree, this));

        connect(&buildButton, SIGNAL(clicked()), this, SLOT(runThread()));
        connect(&heightButton, SIGNAL(clicked()), this, SLOT(runThread()));
        connect(&countButton, SIGNAL(clicked()), this, SLOT(runThread()));
        connect(&leafButton, SIGNAL(clicked()), this, SLOT(runThread()));
        connect(&inOrderButton, SIGNAL(clicked()), this, SLOT(runThread()));
        connect(&preOrderButton, SIGNAL(clicked()), this, SLOT(runThread()));
        connect(&postOrderButton, SIGNAL(clicked()), this, SLOT(runThread()));
        connect(&searchButton, SIGNAL(clicked()), this, SLOT(runThread()));
    }

    virtual void runThread()
    {
        QObject* source = sender();
        if (workerThread)
        {
            disconnect(workerThread, SIGNAL(finished()), this, SLOT(handleResult()));
            workerThread->quit();
            workerThread = nullptr;
        }
        if (source == &this->buildButton)
        {
            this->nodes.clear();
            this->tree.initNode(0);
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

private:
    TreeType tree;
    QMap<QObject*, Thread*> threads;

    QPushButton buildButton;
    QPushButton heightButton;
    QPushButton countButton;
    QPushButton leafButton;
    QPushButton inOrderButton;
    QPushButton preOrderButton;
    QPushButton postOrderButton;
    QPushButton searchButton;

};

#endif // TP3_H
