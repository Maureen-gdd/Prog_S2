#include "tp2.h"

void SorterThread::run() {
	if (this->isFinished())
		return;
	try
	{
        int n = mainWindow->getParam("n").toInt();
        mainWindow->clearArrays();
        Array& arrayToSort = mainWindow->newRandomArray(n);
        custom_msleep(2000);
		this->function(arrayToSort);
		assertSort(arrayToSort);
        success = true;
        _message = QString("Sorting succeed !");
	}
	catch(std::exception& e)
	{
		_message = QString(e.what());
		success = false;
	}
}

void SorterThread::assertSort(const Array& array) const
{
	for (uint i=0; i<array.size()-1; ++i)
		if (array.__get__(i) > array.__get__(i+1))
		{
			QString message("Sorting failed: %1 > %2");
			throw std::runtime_error(message.arg(array.__get__(i))
										.arg(array.__get__(i+1))
										.toStdString());
		}
}

const QString &SorterThread::errorMessage() const
{
	return _message;
}

TestMainWindow::TestMainWindow(std::function<void (Array &)> sortFunction, QWidget *parent)
	: MainWindow(parent)
{
	workerThread = new SorterThread(this, sortFunction, this);
	connect(workerThread, SIGNAL(finished()), this, SLOT(handleResult()));
}
