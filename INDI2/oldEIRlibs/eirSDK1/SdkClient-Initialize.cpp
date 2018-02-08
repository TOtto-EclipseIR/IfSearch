#include "SdkClient.h"

#include <QtCore/QFinalState>
#include <QtCore/QProcess>
#include <QtCore/QState>
#include <QtCore/QTimer>
#include <QtGui/QImage>

#include "../eirExe/Log.h"
#include "../eirGUI/TimeoutBar.h"
#include "SdkClientParameters.h"
#include "SdkCommand.h"
#include "SdkMachine.h"



void SdkClient::enterStartProcess(void)
{
    qDebug() << objectName() << "SdkMachine::enterStartingProcess";
    OBJPOINTER(QProcess, exe_process);
    emit stateName("Start Process");
    setProperty("State", "Start Process");

    interface()->write(VariableId("Options/Shutdown"), true);

    QString exePathName(makeExePathName());
    if (exePathName.isEmpty())
        return; // throw an error somewhere

    _machine->timeoutBar()->start(_parms->getInitialize_TimeoutMsec());
    exe_process->start(exePathName,
                       QStringList() << interface()->settingName()
                                     << parms()->getBinary_ExeArgs().split(";"),
                       QIODevice::ReadOnly);
}

void SdkClient::enterStartSdk(void)
{
    qDebug() << objectName() << "SdkMachine::enterStartingSdk";
    setProperty("State", "Start SDK");
    emit stateName("Start SDK");
    QTimer::singleShot(20000, this, SLOT(notifyObject()));
}

void SdkClient::enterProcessError(void)
{
    OBJPOINTER(QProcess, exe_process);
    setProperty("State", "Process Error");
    emit stateName("Process Error");
    _machine->timeoutBar()->stop();
    error_string = "Error Starting Process for SDK: " + exe_process->errorString();
    qDebug() << objectName() << "SdkMachine::enterProcessError" << error_string;
    emit error(error_string);
}

void SdkClient::enterSdkTimeout(void)
{
    error_string = "SDK Start Timeout";
    setProperty("State", "SDK Timeout");
    emit stateName("SDK Timeout");
    _machine->timeoutBar()->reset();
    qDebug() << objectName() << "SdkMachine::enterSdkTimeout" << error_string;
    setProperty("State", "SDK Timeout");
    emit error(error_string);
}

QString SdkClient::makeExePathName(void)
{
    QString exePathName(parms()->getBinary_ExeName());
    QDir dir(parms()->getBinary_PathName());
    if ( ! dir.exists())
    {
        error_string = QString("%1 directory does not exist")
                .arg(dir.absolutePath());
        emit error(error_string);
        return QString();
    }
    if ( ! dir.exists(exePathName))
    {
        error_string = QString("%1 executable does not exist")
                        .arg(dir.absoluteFilePath(exePathName));
        emit error(error_string);
        return QString();
    }
    if (QDir::current() != dir)
        exePathName = dir.absoluteFilePath(exePathName);
    qDebug() << "exePathName =" << exePathName;
    return exePathName;
}
