/** @file InfoThread.cpp
*	Definition of InfoThread class for DDT core library
*
*	@author	Dynamic DeZigns Technology
*
*/

#include <InfoThread.h>

#include <QTimer>
#include <QThread>

using namespace DDT;
#include <Info.h>

InfoThread::InfoThread(QThread::Priority priority, int msecShort, int msecLong)
: threadPriority(priority), msShort(msecShort), msLong(msecLong)
{
}

InfoThread::~InfoThread()
{
#ifndef INFO_NOTHREAD
	terminate();
	wait(5000);
#endif
}

void InfoThread::run(void)
{
#ifndef INFO_NOTHREAD
	setPriority(threadPriority);
	QTimer::singleShot(msShort, this, SLOT(pump()));
	exec();
#endif
}

void InfoThread::pump(void)
{
	if (Info::isEmpty())
	{
		QTimer::singleShot(msLong, this, SLOT(pump()));
	}
	else
	{
		Info::write(Info::take());
		QTimer::singleShot(msShort, this, SLOT(pump()));
	}
} // pump()
