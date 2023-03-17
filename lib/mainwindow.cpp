#include "mainwindow.h"

#include <QDebug>
#include <time.h>
#include <chrono>
#include <thread>
#include <stdexcept>
#include <math.h>
#include <QApplication>
#include <QGraphicsItem>
#include <QDockWidget>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QGraphicsPixmapItem>
#include <QGraphicsWidget>
#include <QMutex>
#include <QWaitCondition>
#include <QPair>

#include "array.h"

unsigned long MainWindow::instruction_duration = 1000;
MainWindow* MainWindow::_instance = nullptr;

MainWindow* MainWindow::instance()
{
    return MainWindow::_instance;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    timer(this), background(nullptr), zoom(false), scale(1),
    maxNumbersY(150), workerLayout(nullptr), startButton("Start"), stopButton("Stop"), workerThread(nullptr)
{
    initialize();
    resize(1280, 720);

    timer.setInterval((int)(MainWindow::instruction_duration * 0.49f));
    timer.start();
    MainWindow::_instance = this;
}

void MainWindow::initialize()
{
    qsrand((uint)time(nullptr));

    setBackground(nullptr);

    view.setScene(&scene);
    view.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    view.setMouseTracking(true);
    view.installEventFilter(this);
    view.viewport()->installEventFilter(this);
    this->setCentralWidget(&view);

    QWidget* workerUI = new QWidget();

    workerLayout = new QVBoxLayout();

    workerUI->setLayout(workerLayout);
    workerLayout->addWidget(&parametersView);
    workerLayout->addStretch();
    workerLayout->addWidget(&startButton);
    workerLayout->addWidget(&stopButton);

    QSpinBox* delayEditor = new QSpinBox();
    delayEditor->setRange(100, 2000);
    delayEditor->setSingleStep(100);
    delayEditor->setValue(this->instruction_duration);
    parametersLayout.addRow("Instruction Duration", delayEditor);
    connect(delayEditor, SIGNAL(valueChanged(int)), this, SLOT(updateInstructionDuration(int)));

    parametersView.setMinimumWidth(200);
    parametersView.setLayout(&parametersLayout);
    QDockWidget *dockWidget = new QDockWidget(tr("Parameters"), this);
    dockWidget->setAllowedAreas(Qt::LeftDockWidgetArea |
                                Qt::RightDockWidgetArea);
    dockWidget->setWidget(workerUI);
    this->addDockWidget(Qt::RightDockWidgetArea, dockWidget);

    toAdd.append(&statusItem);
    statusItem.setDefaultTextColor(QColor(215,235,255));

    connect(&timer, SIGNAL(timeout()), this, SLOT(updateScene()));
    connect(&startButton, SIGNAL(clicked()), this, SLOT(startWorkerThread()));
    connect(&stopButton, SIGNAL(clicked()), this, SLOT(stopWorkerThread()));
    arrays.reserve(40);
    this->resize(600, 600);
}


Array &MainWindow::newRandomArray(uint size)
{
	arrays.push_back(new Array(size));
	Array& array = *arrays.last();
	array.fillRandom(0,255);
	this->dirty = true;
	custom_msleep(MainWindow::instruction_duration / 3);
	return array;
}

Array &MainWindow::newSortedRandomArray(uint size)
{
	arrays.push_back(new Array(size));
	Array& array = *arrays.last();
	array.fillSortedRandom(0,500);
	custom_msleep(MainWindow::instruction_duration / 2);
	return array;
}

Array &MainWindow::newArray(uint size)
{
	arrays.push_back(new Array(size));
	Array& array = *arrays.last();
	custom_msleep(MainWindow::instruction_duration / 2);
	this->dirty = true;
	return array;
}

void MainWindow::updateLayout()
{

    if (dirty)
    {
        int itemWidth = qMin<int>(qMax<int>(50, width() * 0.01f), 150);
        int maxX=0, maxY=0;

        updateLayoutItems(itemWidth, maxX, maxY);

        this->scene.setSceneRect(0, 0, qMax(width(), maxX), qMax(height(),maxY+itemWidth * 2));
        maxNumbersY = qMax(maxNumbersY, maxY);
    }

    this->dirty=false;
}

void MainWindow::updateLayoutItems(int itemWidth, int& originX, int& originY)
{
    updateStatusItem(itemWidth, originX, originY);
    updateBackground();
    updateNumberItems(itemWidth, originX, originY);
    updateFunctionCalls(itemWidth, originX, originY);
}


void MainWindow::updateBackground()
{
	if (background)
	{
		background->setPos(0,0);
		background->setPixmap(background->pixmap().scaledToWidth(scene.width()));
	}
}

void MainWindow::pushFunctionCall(QString functionName, int parameter)
{
    functionCalls.push(QString("%1(%2)").arg(functionName).arg(QString::number(parameter)));
    custom_msleep(MainWindow::instruction_duration);
}

void MainWindow::pushFunctionCall(QString functionName, int parameter1, int parameter2)
{
    functionCalls.push(QString("%1(%2, %3)").arg(functionName)
                                            .arg(parameter1)
                                            .arg(parameter2));
    custom_msleep(MainWindow::instruction_duration);
}

void MainWindow::pushFunctionCall(QString functionName, Point parameter1, int parameter2, Point parameter3)
{
    functionCalls.push(QString("%1([%2, %3], %4, [%5, %6])").arg(functionName)
                                            .arg(parameter1.x)
                                            .arg(parameter1.y)
                                            .arg(parameter2)
                                            .arg(parameter3.x)
                                            .arg(parameter3.y));
    custom_msleep(MainWindow::instruction_duration);
}

void MainWindow::pushFunctionCall(QString functionName, const Array& parameter1, int parameter2)
{
    functionCalls.push(QString("%1(array<%2 elements>, %3)").arg(functionName)
                                            .arg(parameter1.size())
                                            .arg(parameter2));
    custom_msleep(MainWindow::instruction_duration);
}

void MainWindow::pushFunctionCall(QString functionName, const Array &parameter1, const Array &parameter2, int parameter3, int parameter4)
{
    functionCalls.push(QString("%1(array<%2 elements>, array<%2 elements>, %4, %3)")
                                            .arg(functionName)
                                            .arg(parameter1.size())
                                            .arg(parameter2.size())
                                            .arg(parameter3)
                                            .arg(parameter4));
    custom_msleep(MainWindow::instruction_duration);
}

int MainWindow::popFunctionCall(int result)
{
    popFunctionCall(QString::number(result));
    return result;
}

void MainWindow::popFunctionCall(QVariant result)
{
    if (!functionCalls.empty())
    {
        if (!functionCallItems.isEmpty())
        {
            QGraphicsTextItem* item = functionCallItems.takeLast();
            while (toAdd.contains(item))
                custom_msleep(20);
            toUpdate.push_back(QPair<QGraphicsTextItem*, QString>(item, item->toPlainText() +
                                                                  QString(" -> %1").arg(result.toString())));
            custom_msleep(MainWindow::instruction_duration);
            toRemove.push_back(item);
        }
        functionCalls.pop();
    }
}

void MainWindow::clearFunctionsCall()
{
    while (!functionCallItems.isEmpty())
    {
        QGraphicsTextItem* item = functionCallItems.takeLast();
        toRemove.push_back(item);
    }
    functionCalls.clear();
}


void MainWindow::clearArrays()
{
    while (!this->numberItems.isEmpty())
    {
        for (QGraphicsItem* item : numberItems.last()) {
            toRemove.push_back(item);
        }
		numberItems.pop_back();
    }
    arrays.clear();
    maxNumbersY = 0;
}

QVariant &MainWindow::addIntParam(QString param_name,
                                  int defaultValue, int min, int max)
{
    if (parameters.contains(param_name))
        return parameters[param_name];
    QSpinBox* intEditor = new QSpinBox();
    intEditor->setObjectName(param_name);
    intEditor->setRange(min, max);
    intEditor->setValue(defaultValue);
    this->connect(intEditor, SIGNAL(valueChanged(int)), this, SLOT(handleParamChanged()));
    parametersLayout.addRow(param_name, intEditor);
    parameters.insert(param_name, QVariant(defaultValue));
    return parameters[param_name];
}

QVariant &MainWindow::addFloatParam(QString param_name, float defaultValue, float min, float max)
{
    QDoubleSpinBox* floatEditor = new QDoubleSpinBox();
    floatEditor->setValue(defaultValue);
    floatEditor->setObjectName(param_name);
    floatEditor->setRange(min, max);
    this->connect(floatEditor, SIGNAL(valueChanged(double)), this, SLOT(handleParamChanged()));
    parametersLayout.addRow(param_name, floatEditor);
    parameters.insert(param_name, QVariant(defaultValue));
    return parameters[param_name];
}

const QVariant &MainWindow::getParam(QString param_name)
{
    return parameters[param_name];
}

int MainWindow::updateFunctionCalls(int itemWidth, int&, int& maxY)
{
    for (QGraphicsTextItem* item : functionCallItems)
    {
        int startX = 0.05f * (float)width();

        item->setPos(startX, maxY);
        maxY += itemWidth * 1.2;
    }
    return maxY;
}

void MainWindow::handleResult()
{
    if (workerThread && workerThread->message().size())
    {
        if (workerThread->succeeded())
        {
            statusItem.setPlainText(QString("Success, gg bg: %1").arg(workerThread->message()));
            statusItem.setDefaultTextColor(QColor(100,230,100));
        }
        else
        {
            statusItem.setPlainText(QString("Fail: %1").arg(workerThread->message()));
            statusItem.setDefaultTextColor(QColor(250,50,80));
        }
    }
}

void MainWindow::handleParamChanged()
{
    QObject* source = sender();
    QVariant& value = parameters[source->objectName()];
    if (QVariant::Double & value.type() || QVariant::Int & value.type())
    {
        QSpinBox* spinBox = dynamic_cast<QSpinBox*>(source);
        if (spinBox)
            value.setValue(spinBox->value());
        else {
            QDoubleSpinBox* doubleSpinBox = dynamic_cast<QDoubleSpinBox*>(source);
            value.setValue(doubleSpinBox->value());
        }
    }
}

void MainWindow::updateInstructionDuration(int value)
{
    this->instruction_duration = value;
    this->timer.setInterval(instruction_duration);
}


int MainWindow::updateNumberItems(int itemWidth, int& maxX, int& maxY)
{
    if (numberItems.size())
        maxY += qMin<int>(qMax<int>(50, scene.height() * 0.01f), 150);

    maxX = 0;
	for (QVector<NumberGraphicsItem*> items : numberItems)
	{
		int startX = 0.05 * width();
		for (NumberGraphicsItem* item : items)
		{
			item->setRect(startX, maxY, itemWidth, itemWidth);
			startX += itemWidth * 1.5f;
			if (startX + itemWidth * 2 > scene.width())
			{
				maxY += itemWidth * 1.5f;
				startX = qMax((50 + scene.width() - itemWidth * 1.5f * items.size()) * 0.5,
											0.05 * scene.width());
			}
		}
		if (startX > maxX)
			maxX = startX;
		maxY += itemWidth * 2;
	}
	return maxY;
}

bool MainWindow::eventFilter(QObject* object, QEvent* event)
{
	if (event->type() == QEvent::Wheel)
	{
		QWheelEvent* wheel_event = static_cast<QWheelEvent*>(event);
		if (QApplication::keyboardModifiers() == Qt::KeyboardModifier::ControlModifier) {
			float wheelDelta = wheel_event->delta();
			scale += std::pow(1.01f, std::abs(wheelDelta) / 24.f) * (std::signbit(wheelDelta) ? -1 : 1) / 5.f;
			scale = qMin<float>(2.5f, qMax<float>(0.25f, scale));
			zoom = true;
			targetPos = wheel_event->pos();
			return true;
		}
	}
	return QMainWindow::eventFilter(object, event);
}

void MainWindow::resizeEvent(QResizeEvent *)
{
	this->dirty = true;
}

void MainWindow::closeEvent(QCloseEvent* e)
{
	if (workerThread && workerThread->isRunning())
	{
		MainWindow::instruction_duration = 1;
		workerThread->wait(500);
	}
	QMainWindow::closeEvent(e);
}

void MainWindow::setBackground(QImage *image)
{
	if (background)
	{
		scene.removeItem(background);
	}
	if (!image)
	{
		background = nullptr;
		QRadialGradient gradient(-20, -40, 1920);
		gradient.setSpread(QGradient::ReflectSpread);
		gradient.setColorAt(0,QColor(60,28,52));
		gradient.setColorAt(1,QColor(30,10,24));
		scene.setBackgroundBrush(gradient);
	}
	else
	{
		scene.setBackgroundBrush(QBrush(Qt::black));
		background = new QGraphicsPixmapItem(QPixmap::fromImage(*image));
		background->setPos(0, 0);
		background->setZValue(-1);
		toAdd.push_back(background);
	}
}

void MainWindow::updateStatusItem(int itemWidth, int &maxX, int &maxY)
{
	QFont font("Arial", itemWidth * 0.35, QFont::DemiBold);
	statusItem.setFont(font);
	statusItem.setPos(scene.width() * 0.05f, scene.height() * 0.05f);
	statusItem.setTextWidth(width() / 2);
    maxY = statusItem.pos().y() + statusItem.boundingRect().height();
}

void MainWindow::updateScene()
{
    QFont font("Arial", 15, QFont::DemiBold);
    while (functionCallItems.size() < functionCalls.size())
    {
        QGraphicsTextItem* item = new QGraphicsTextItem(functionCalls[functionCallItems.size()]);
        functionCallItems.push_back(item);
        toAdd.push_back(item);
        int y = maxNumbersY + 30 * functionCallItems.size();
        item->setPos(50 + 30 * functionCallItems.size(), y);
        item->setFont(font);
        item->setDefaultTextColor(QColor(215,235,255));

    }

	while (numberItems.size() < arrays.size())
	{
		Array& array = *arrays[numberItems.size()];
		numberItems.push_back(QVector<NumberGraphicsItem*>());
		for (uint i=0; i < array.size(); ++i)
		{
            NumberGraphicsItem* item = new NumberGraphicsItem();
            item->setData(array.__get__(i));
			numberItems.last().push_back(item);
			toAdd.push_back(item);
		}
	}
	int i=0;
	for (QVector<NumberGraphicsItem*>& numbers : numberItems)
	{
		Array& array = *arrays[i];
        for (int j=0; j < (int) array.size(); ++j)
		{
			if (j >= numbers.size())
			{
                NumberGraphicsItem* item = new NumberGraphicsItem();
                item->setData(array.__get__(j));
                numbers.push_back(item);
				// toAdd.push_back(numbers[j]);
				dirty = true;
			}
			NumberGraphicsItem* item = numbers[j];
            if (item->data() != array.__get__(j))
			{
                item->setData(array.__get__(j));
				dirty = true;
			}
			if (array.hasBeenReadenAt(j))
				item->displayReadenState();
			else if (array.hasBeenWrittenAt(j))
				item->displayWrittenState();
			else
				item->displayDefault();
		}
		++i;
	}

	for (QGraphicsItem* item : toAdd)
	{
		scene.addItem(item);
		dirty=true;
	}
	toAdd.clear();

	for (QPair<QGraphicsTextItem*, QString>& update : toUpdate)
	{
		update.first->setPlainText(update.second);
		dirty=true;
	}
	toUpdate.clear();

	for (QGraphicsItem* item : toRemove)
	{
		item->hide();
		dirty=true;
	}
	toRemove.clear();

	if (zoom)
	{
		dirty = true;
		view.centerOn(view.mapToScene(targetPos.toPoint()));
		view.setTransform(QTransform::fromScale(scale, scale));
		zoom = false;
	}

	if (dirty)
	{
		updateLayout();
		repaint();
    }
}

void MainWindow::startWorkerThread()
{
    if (workerThread && !workerThread->isRunning())
    {
        workerThread->start();
    }
}

void MainWindow::stopWorkerThread()
{
    if (workerThread && workerThread->isRunning())
    {
        workerThread->terminate();
    }
}

NumberGraphicsItem::NumberGraphicsItem(QGraphicsItem* parent) :
    QGraphicsEllipseItem(parent), state(STATE_COUNT)
{
	QPen pen;
	pen.setWidth(3);
	setPen(pen);
	displayDefault();
}

void NumberGraphicsItem::setData(const QVariant &data)
{
    if(this->_data != data)
	{
        text = data.toString();
        this->_data = data;
		displayDefault(true);
		update();
	}
}


const QVariant &NumberGraphicsItem::data() const
{
    return _data;
}


void NumberGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
							   QWidget *widget)
{
	QGraphicsEllipseItem::paint(painter, option, widget);
	painter->setFont(widget->parentWidget()->font());
	painter->drawText(this->rect(), Qt::AlignCenter, text);
}

QColor NumberGraphicsItem::baseColor(150, 220, 183);

void NumberGraphicsItem::displayDefault()
{
	displayDefault(false);
}

void NumberGraphicsItem::displayDefault(bool force)
{
	if (state == DEFAULT && !force)
		return;
	QPen pen(this->pen());
    pen.setColor(baseColor.lighter(60+80.f*_data.toInt()/255.f));
	setPen(pen);
	state = DEFAULT;
}

void NumberGraphicsItem::displayReadenState()
{
	if (state == READEN)
		return;
	QPen pen(this->pen());
    pen.setColor(QColor(255,20,100).lighter(100+20.f*_data.toInt()/500.f));
	setPen(pen);
	state = READEN;
}
void NumberGraphicsItem::displayWrittenState()
{
	if (state == WRITEN)
		return;
	QPen pen(this->pen());
    pen.setColor(QColor(150,150,255).lighter(100+20.f*_data.toInt()/255.f));
	setPen(pen);
    state = WRITEN;
}

