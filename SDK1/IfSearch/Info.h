/** @file Info.h
*	Declaration of Info class for DDT core library
*
*	@author	Dynamic DeZigns Technology
*/
#pragma once

#define INFO_NOTHREAD

#include <InfoFunction.h>
#include <InfoItem.h>
#include <InfoOutputBase.h>
#include <InfoOutputFile.h>
#include <InfoOutputSetting.h>
#include <InfoOutputQDebug.h>
#include <InfoOutputs.h>
#include <InfoSeverity.h>
#include <InfoThread.h>

#include <QDateTime>
#include <QList>
#include <QQueue>
#include <QReadWriteLock>
#include <QThread>
#include <QVariant>

class InfoOutputBase;

class DDTCORE_EXPORT Info
{
public:
    ~Info();
    static void add(const InfoItem & item);
    static void add(const Return & rtn);
    static bool expect(const QString & strValue, const QVariant & varValue,
                       const QString & strTest,  const QVariant & varTest,
                       const QString & file, const int line);
    static bool expectNot(const QString & strValue, const QVariant & varValue,
                          const QString & strTest,  const QVariant & varTest,
                          const QString & file, const int line);
    //		static void applicationVersion(VersionInfo * ver);
    //		static void libraryVersion(VersionInfo * ver);
#ifdef NDEBUG
    static void add(InfoOutputBase * out,
                    const InfoSeverity minSev=InfoSeverity::Info,
                    const InfoSeverity maxSev=InfoSeverity::Unknown);
#else
    static void add(InfoOutputBase * out,
                    const InfoSeverity minSev=InfoSeverity::Debug,
                    const InfoSeverity maxSev=InfoSeverity::Unknown);
#endif
    static void remove(InfoOutputBase * out);
    static void flush(void);
    static void clear(void);
    static void start(QThread::Priority priority=QThread::LowPriority,
                      int msecShort=50, int msecLong=1000);
    static void stop(void);
    static void setFlushEvery(int n)
    { flush(); flushEvery_ = n; }
    static QDateTime timeBase(void)
    { return TimeBase; }
    static QString timeBaseString(void)
    { return TimeBase.toString("DyyyyMMdd-Thhmm"); }
    static QReadWriteLock * outLock(void)
    { return &outputLock; }
    static void write(const InfoItem & item);
    static bool isEmpty(void)
    { QReadLocker lock(&queueLock); return queue.isEmpty(); }
    static int size(void)
    { QReadLocker lock(&queueLock); return queue.size(); }
    static InfoItem take(void)
    { QWriteLocker lock(&queueLock); return queue.dequeue(); }
    static void rollover(void);
    static void setRolloverKeep(int keep)
    { rolloverKeep_i = keep; }
    static int rolloverKeep(void)
    { return rolloverKeep_i; }

private:
    static QDateTime TimeBase;
    static QQueue<InfoItem> queue;
    static QReadWriteLock queueLock;
    static InfoOutputs outputs;
    static QReadWriteLock outputLock;
    static InfoThread * thread;
    static int flushEvery_;
    static int rolloverKeep_i;
}; // class Info
