/** @file WindowsProcess.h
*	Declaration of WindowsProcess class for DDT core library
*
*	@author	Dynamic DeZigns Technology
*/

#pragma once
#include <qglobal.h>
#ifdef DDTCORE_LIB
# define DDTCORE_EXPORT Q_DECL_EXPORT
#else
# define DDTCORE_EXPORT Q_DECL_IMPORT
#endif

#include <Return.h>

#include <QMultiMap>
#include <QObject>

class StatusHandler;
class DDTCORE_EXPORT WindowsProcess : public QObject
{
    Q_OBJECT;
public:
    enum Returns
    {
        ReturnStatusNull = 0x85000101, ReturnMemory, ReturnLoadFail,
    };

public:
    WindowsProcess(QObject * parent=0);
    ~WindowsProcess();
#ifdef WINDOWS
    Return enumerate(void);
    QList<quint32> pidsFor(const QString &  name) const;
    QString nameOfPid(const quint32 pid) const;
    Return killPid(quint32 pid);
    Return killSiblings(void);
    static Return loadLibrary(const QString & name);
    static Return loadLibraries(const QStringList & names);
    static Return loadPlugin(const QString & name);
    static Return loadPlugins(const QStringList & names);
#ifndef IJM
    static Return physicalMemory(void);
#endif
    static int removeDirTree(const QString & dirName);

private:
    QMultiMap<quint32, QString> mapPidToName;
    QMultiMap<QString, quint32> mapNameToPid;
#endif
}; // class WindowsProcess
