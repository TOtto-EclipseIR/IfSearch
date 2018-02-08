#include "SdkClient.h"

#include <QtCore/QDateTime>
#include <QtCore/QProcess>
#include <QtCore/QTimer>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QImage>

#include "../eirExe/Log.h"
#include "../eirGUI/TimeoutBar.h"
#include "SdkClientParameters.h"
#include "SdkCommand.h"
#include "SdkMachine.h"

void SdkClient::enterReturning(void)
{
    FUNCTION();
    if (pauseResume_action)
    {
        OBJPOINTER(QAction, pauseResume_action);
        pauseResume_action->setEnabled(true);
        pauseResume_action->setChecked(isPaused());
    }

    if ( ! command_queue.isEmpty())
    {
        TRACE("commandQueued()");
        emit commandQueued();
    }
    if (isPaused())
    {
        TRACE("doPause()");
        emit doPause();
    }
    else
    {
        TRACE("doResume()");
        emit doResume();
    }
}

void SdkClient::enterPaused(void)
{
    FUNCTION();
    emit stateName("Paused");
    interface()->set(VariableId("Input/Pause"), true);
    interface()->write();
}

void SdkClient::enterRunning(void)
{
    FUNCTION();
    emit stateName("Running");
    if ( ! newUrl_s.isEmpty())
    {
        interface()->set("Input/URL", newUrl_s);
        newUrl_s.clear();
    }
    else if ( ! resumeUrl_s.isEmpty())
        interface()->set("Input/URL", resumeUrl_s);
    interface()->set(VariableId("Input/Pause"), false);
    qDebug() << "Input/URL" << interface()->value("Input/URL");
    interface()->write();
}

void SdkClient::notifyObject(void)
{
    FUNCTION();
    if ("Start SDK" == property("State"))
    {
        if ( ! interface()->read(VariableId("Options/Shutdown")).toBool())
        {
            machine()->timeoutBar()->stop();
            base_ems.set(interface()->value(VariableId("Output/BaseEms")).toLongLong());
            TRACE("emit initialized() @%1", base_ems.toString());
            emit initialized();
        }
        else
            QTimer::singleShot(2000, this, SLOT(notifyObject()));
        return;
    }
    else if ( ! current_command.isEmpty())
    {
        TRACE("%1 %2 is %3", current_command.getMode(),
              current_command.getCommand(), current_command.getStatus());
        if (current_command.isComplete())
        {
            TRACE("emit notifyComplete()");
            emit notifyComplete();
        }
        return;
    }
}
