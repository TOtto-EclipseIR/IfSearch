#include "SdkClient.h"

#include <QtCore/QDateTime>
#include <QtCore/QProcess>
#include <QtCore/QTimer>
#include <QtGui/QAction>
#include <QtGui/QImage>

//#include "../eirCore/SettingsVariableGroup.h"
#include "../eirCore/VariableHeader.h"
#include "../eirFile/CsvReader.h"
#include "../eirFile/QQDir.h"
#include "../eirExe/Log.h"
#include "PersonReportRow.h"
#include "SdkClientParameters.h"
#include "SdkCommand.h"
#include "SdkMachine.h"

SdkClient::SdkClient(SdkClientParameters * parms,
                     QObject *parent)
    : QObject(parent)
    , _parms(parms)
    , interface_group(0)
    , _machine(new SdkMachine(this))
    , exe_process(new QProcess(this))
    , pauseResume_action(0)
    , currentUpdate_i(-1)
    , currentPollCount_i(-1)
    , base_ems(0)
    , paused_bool(false)
    , personReport_reader(0)
{
    FUNCTION(parms->getBinary_PathName(), parent->objectName());
    OBJPOINTER(SdkMachine, _machine);
    setObjectName("SdkClient");
    current_command.clear();
    PersonReportRow prr;
    VariableHeader vh(&prr);
    personReport_table.setHeader(vh);
}

SdkClientParameters * SdkClient::parms(void) const
{
    return _parms;
}

VariableSettings * SdkClient::interface(void)
{
    return interface_group;
}

SdkMachine * SdkClient::machine(void) const
{
    OBJPOINTER(SdkMachine, _machine);
    return _machine;
}

QProcess * SdkClient::process(void) const
{
    OBJPOINTER(QProcess, exe_process);
    return exe_process;
}

void SdkClient::setResumeUrl(const QString & url)
{
    resumeUrl_s = url;
}

void SdkClient::setInterface(VariableSettings * interfaceSettings,
                             const VariableSet & defaults)
{
    FUNCTION(interfaceSettings->name(), defaults.name());
    interface_group = interfaceSettings;
    interface_group->removeAll();
    interface_group->import(defaults);
    interface_group->write();
    paused_bool = interface_group->value(VariableId("Input/Pause")).toBool();
}

QString SdkClient::dirName(QString settingName)
{
    FUNCTION(settingName);
    if ( ! settingName.contains("/")) settingName.prepend("Output/");
    QString settingDirName = interface_group->value(settingName).toString().split(';').at(0);
    if (settingDirName.isEmpty())
        return QString();

    QQDir settingDir(settingDirName, base_ems);
    if (settingDir.isRelative())
    {
        QString baseDirName = interface()->value(VariableId("Output/BaseDir")).toString();
        QQDir baseDir(baseDirName, base_ems);
        baseDir.mkpath(settingDirName);
        baseDir.cd(settingDirName);
        FNRETURN(baseDir.absolutePath());
        return baseDir.absolutePath();
    }
    else
    {
        FNRETURN(settingDir.absolutePath());
        return settingDir.absolutePath();
    }
}

void SdkClient::shutdown(void)
{
    qDebug() << objectName() << "SdkMachine::shutdown()";
    setProperty("State", "Shutdown");
    if (QProcess::NotRunning != exe_process->state())
    {
        interface()->set(VariableId("Options/Shutdown"), true);
        interface()->write();
        qDebug() << objectName() << "shutdown() waiting for QProcess finished";
        if ( ! exe_process->waitForFinished())
            qCritical() << "waitForFinished() timeout";
    }
}

bool SdkClient::isPaused(void) const
{
    return paused_bool;
}

void SdkClient::stateChanged(void)
{
    OBJPOINTER(SdkMachine, _machine);
    emit stateName(_machine->property("State").toString());
}

QString SdkClient::errorString(void) const
{
    return error_string;
}

bool SdkClient::getPersonReport(SdkCommand result)
{
    FUNCTION(result.key());
    FNRETURN(false);
    if ( ! result.isRetrieve())    return false;
    if ("PersonReport" != result.getCommand()) return false;

    QString prFileName(result.value("Settings/Retrieve/ReportFile").toString());
    if ( ! QFile::exists(prFileName)) return false;

    personReport_file = new QFile(prFileName, this);
    personReport_reader = new CsvReader(personReport_file, this);
    OBJPOINTER(CsvReader, personReport_reader);
    CONNECT(personReport_reader, SIGNAL(atEnd()),
            this, SLOT(personReportAtEnd()));
    personReport_reader->start();
    FNRETURN(true);
    return true;
}

VariableTable * SdkClient::personReport(void)
{
    return &personReport_table;
}

void SdkClient::personReportAtEnd(void)
{
    FUNCTION();
    personReport_table.loadData(personReport_reader->matrix());
    emit gotPersonReport();
    delete personReport_reader;
    personReport_reader = 0;
    delete personReport_file;
    personReport_file = 0;
}

