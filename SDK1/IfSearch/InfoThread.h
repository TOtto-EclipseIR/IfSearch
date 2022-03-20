/** @file InfoThread.h
*	Internal declaration of InfoThread class for DDT core library
*
*	@author	Dynamic DeZigns Technology
*/
#pragma once
#include <qglobal.h>


#include <QThread>

class Info;

class InfoThread : public QThread
{
    Q_OBJECT

public:
    InfoThread(QThread::Priority priority=QThread::LowPriority,
               int msecShort=50, int msecLong=1000);
    ~InfoThread();
    void run(void);

private slots:
    void pump(void);

private:
    QThread::Priority threadPriority;
    int msShort;
    int msLong;
}; // class InfoThread
