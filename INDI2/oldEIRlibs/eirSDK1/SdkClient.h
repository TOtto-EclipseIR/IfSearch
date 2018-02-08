#ifndef SDKCLIENT_H
#define SDKCLIENT_H
#include "eirSDK1.h"

#include <QObject>

#include <QtCore/QQueue>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QUrl>

class QAction;
class QFile;
class QImage;
class QProcess;
class QSettings;

#include "../eirTypes/MillisecondTime.h"
#include "../eirCore/VariableTable.h"
#include "../eirCore/VariableSettings.h"
class CsvReader;

#include "SdkCommand.h"
class SdkClientParameters;
class SdkMachine;
class TimeoutBar;

class EIRSDK1SHARED_EXPORT SdkClient : public QObject
{
    Q_OBJECT

public:
    enum Action
    {
        PauseResume = 1,
    };

public:
    explicit SdkClient(SdkClientParameters * parms,
                       QObject * parent=0);
    SdkClientParameters * parms(void) const;
    VariableSettings * interface(void);
    SdkMachine * machine(void) const;
    QProcess * process(void) const;
    void setInterface(VariableSettings * interfaceSettings,
                      const VariableSet & defaults);
    QAction * action(Action act, QObject * parent=0);
    void setResumeUrl(const QString & url);
    bool isPaused(void) const;
    QString errorString(void) const;

    QString dirName(QString settingName);
    SdkCommand newCommand(const QString & mode,
                            const QString & cmd);
    void submit(SdkCommand command);
    bool getPersonReport(SdkCommand result);
    VariableTable * personReport(void);

signals:
    void error(QString errorMessage);
    void commandQueued(void);
    void empty(void);
    void stateName(QString name);
    void doPause(void);
    void doResume(void);
    void initialized(void);
    void notifyComplete(void);
    void notifyUpdate(void);

    void searchResult(SdkCommand results);
    void retrieveResult(SdkCommand results);
    void enrollResult(SdkCommand results);
    void commandError(SdkCommand results);
    void gotPersonReport(void);

public slots:
    void pause(void);
    void resume(void);
    void startUrl(QString url);
    void shutdown(void);

private slots:
    void pauseResume(bool pause);
    void setPauseResumeCheck(void);
    void stateChanged();
    void notifyObject(void);
    void checkComplete(void);
    void personReportAtEnd(void);
    void checkPollCount(void);

    void enterStartProcess(void);
    void enterStartSdk(void);
    void enterProcessError(void);
    void enterSdkTimeout(void);
    void enterReturning(void);
    void enterPaused(void);
    void enterRunning(void);
    void enterInitiate(void);
    void enterPending(void);
    void enterComplete(void);
    void enterCommandTimeout(void);

private:
    void reportResult(void);
    QString makeExePathName(void);
    SdkCommand dequeueCommand(void);
    bool isCommandEmpty(void) const;
    QString transferInputImages(const QString & mode);
    QString setupOutputDir(const QString & mode);

private:
    SdkClientParameters * _parms;
    VariableSettings * interface_group;
    SdkMachine * _machine;
    QProcess * exe_process;
    QAction * pauseResume_action;
    QString resumeUrl_s;
    QString newUrl_s;
    SdkCommand current_command;
    int currentUpdate_i;
    int currentPollCount_i;
    QString error_string;
    MillisecondTime base_ems;
    bool paused_bool;
    QQueue<SdkCommand> command_queue;
    QFile * personReport_file;
    CsvReader * personReport_reader;
    VariableTable personReport_table;
};

#endif // SDKCLIENT_H
