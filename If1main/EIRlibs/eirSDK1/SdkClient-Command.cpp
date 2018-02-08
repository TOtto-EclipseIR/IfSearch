#include "SdkClient.h"

#include <QtCore/QProcess>
#include <QtCore/QTimer>
#include <QtGui/QImage>

#include "../eirFile/TemporaryDir.h"
#include "../eirGUI/TimeoutBar.h"
#include "../eirExe/Log.h"
#include "SdkClientParameters.h"
#include "SdkCommand.h"
#include "SdkMachine.h"

SdkCommand SdkClient::newCommand(const QString & mode,
                                 const QString & cmd)
{
    SdkCommand command;
    command.setMode(mode);
    command.setCommand(cmd);
    command.setName(cmd);
    TRACE("newCommand: #%1 %2 %3 %4ms",
          command.key(), command.getMode(),
          command.getCommand(),
          command.getTimeoutMsec());
    return command;
}

void SdkClient::submit(SdkCommand command)
{
    TRACE("submit: #%1 %2 %3 %4ms",
          command.key(), command.getMode(),
          command.getCommand(),
          command.getTimeoutMsec());
    command_queue.enqueue(command);
    emit commandQueued();
}


SdkCommand SdkClient::dequeueCommand(void)
{
    return command_queue.dequeue();
}

bool SdkClient::isCommandEmpty(void) const
{
    return command_queue.isEmpty();
}

void SdkClient::enterInitiate(void)
{
    FUNCTION();
    STATE("Initiate");
    emit stateName("Initiate");

    if ( ! current_command.isEmpty())
    {
        TRACE("enterInitiate busy with %1", current_command.key());
        QTimer::singleShot(250, this, SIGNAL(commandQueued()));
    }
    if (isCommandEmpty())
    {
        TRACE("enterInitiate empty()");
        emit empty();
        return;
    }

    current_command = dequeueCommand();
    TRACE("Initiating: #%1 %2 %3 %4ms",
          current_command.key(),
          current_command.getMode(),
          current_command.getCommand(),
          current_command.getTimeoutMsec());
    QString mode(current_command.getMode());
    VariableSet settings(current_command.exportSection("Settings"));
    LOG(dump(settings));
    interface()->import(settings);
    interface()->write();

    QString inDirName = transferInputImages(mode);
    if ( ! inDirName.isEmpty())
        settings.set(mode + "/InputDir", inDirName);
    QString outDirName = setupOutputDir(mode);
    if ( ! outDirName.isEmpty())
        settings.set(mode + "/OutputDir", outDirName);
    settings.set(mode + "/Command", current_command.getCommand());
    settings.set(mode + "/Status", QString());
    interface()->import(settings);
    interface()->write();
}

void SdkClient::enterPending(void)
{
    FUNCTION();
    STATE("Pending");
    emit stateName("Pending");

    OBJPOINTER(SdkMachine, _machine);

    currentUpdate_i = 0;
    QTimer::singleShot(current_command.getReadMsec(),
                       this, SLOT(checkComplete()));
    _machine->timeoutBar()->start(current_command.getTimeoutMsec());
}

void SdkClient::checkComplete(void)
{
    if (current_command.isEmpty()) return;

    QString status = interface()->read(current_command.getMode()+"/Status").toString();
    current_command.setStatus(status);
    if (current_command.isComplete())
    {
        TRACE("emit notifyComplete");
        emit notifyComplete();
    }
    else
    {
        if (current_command.isUpdate())
        {
            int u = current_command.getStatus().mid(6).toInt();
            if (currentUpdate_i != u)
            {
                currentUpdate_i = u;
                TRACE("emit notifyUpdate");
                emit notifyUpdate();
            }
        }
        QTimer::singleShot(100, this, SLOT(checkComplete()));
    }
}

void SdkClient::enterComplete(void)
{
    OBJPOINTER(SdkMachine, _machine);
    qDebug() << objectName() << "SdkMachine::enterComplete()";
    emit stateName("Complete");
    _machine->timeoutBar()->stop();

    interface()->read();
    QString mode(current_command.getMode());
    QString delimiter(interface()->value(VariableId("Options/Default")).toString());
    if (delimiter.isEmpty()) delimiter = "^";
    current_command.setStatus(interface()->value(mode+"/Status").toString());
    current_command.setReason(interface()->value(mode+"/Reason").toString());
    current_command.setResults(interface()->value(mode+"/Results")
                                .toString().split(delimiter));
    TRACE("Complete: #%1 %2 %3 %4ms",
          current_command.key(), current_command.getMode(),
          current_command.getCommand(),
          current_command.getTimeoutMsec());
    TRACE("Status=%1 %2", current_command.getStatus(),
          current_command.isSuccess()
                ? current_command.getStatus()
                : current_command.getReason());

    current_command.parseResult();
    current_command.parseOutput();
    reportResult();
} // enterComplete()

void SdkClient::enterCommandTimeout(void)
{
    FUNCTION();
    STATE("Timeout");
    emit stateName("Timeout");
    current_command.setStatus("Timeout");
    current_command.setReason(QString("Timeout after %1 msec")
                               .arg(current_command.getTimeoutMsec()));
    QTimer::singleShot(2000, machine()->timeoutBar(), SLOT(reset()));
    reportResult();
}

QString SdkClient::transferInputImages(const QString & mode)
{
    FUNCTION(mode);
    int nImages = current_command.getImagesIn();
    if (0 == nImages) return QString();

    VariableSet input = current_command.exportSection(VariableId("Input"));
    LOG(dump(input));

    QString inDirName(dirName(mode+"/InputDir"));
    QDir inDir(inDirName);
    if (inDirName.isEmpty())
    {
        TemporaryDir tempDir(QDir::temp(), mode+"In", false);
        inDirName = tempDir.absolutePath();
        inDir = QDir(inDirName);
    }
    else
    {
        if (inDir.exists())
            EXPECT(QQDir::removeAll(inDir));
        else
            EXPECT(inDir.mkpath("."));
    }

    for (int x = 1; x <= nImages; ++x)
    {
        VariableId vid(QString::number(x));
        VariableId vidId(vid);
        VariableId vidImage(vid);
        vidId.append("Id");
        vidImage.append("Image");
        if (EXPECT(input.contains(vidImage)))
        {
            QString fileName;
            QImage image(input.value(vidImage).value<QImage>());
            if (input.contains(vidId))
                fileName = input.value(vidId).toString();
            else
                fileName = QString::number(x);
            fileName += ".png";
            EXPECT(image.save(inDir.absoluteFilePath(fileName), "png"));
        }
    }
    FNRETURN(inDirName);
    return inDirName;
}

QString SdkClient::setupOutputDir(const QString & mode)
{
    FUNCTION(mode);
    QString outDirName(dirName(mode+"/OutputDir"));
    if (outDirName.isEmpty())
    {
        TemporaryDir tempDir(QDir::temp(), mode+"Out", false);
        outDirName = tempDir.absolutePath();
    }
    else
    {
        QDir outDir(outDirName);
        if (outDir.exists())
        {
            if ( ! QQDir::removeAll(outDir))
                qWarning() << "Not all files removed from" << outDir.absolutePath();
        }
        else
            outDir.mkpath(".");
    }
    current_command.setOutBaseDir(outDirName);
    FNRETURN(outDirName);
    return outDirName;
}

void SdkClient::reportResult(void)
{
    FUNCTION();
    if (current_command.isSearch())
    {
        TRACE("emit searchResult(result)")
        emit searchResult(current_command);
    }
    else if (current_command.isRetrieve())
    {
        TRACE("emit retrieveResult(result)")
        emit retrieveResult(current_command);
    }
    else if (current_command.isEnroll())
    {
        TRACE("emit enrollResult(result)")
        emit enrollResult(current_command);
    }
    else
    {
        TRACE("emit commandError(result)")
        emit commandError(current_command);
    }
    current_command.clear();
}
