#include "SdkClient.h"

#include <QtCore/QTimer>

#include "../eirExe/Log.h"
#include "../eirGUI/CheckAction.h"
#include "../eirGUI/QQIcon.h"
#include "SdkMachine.h"

QAction * SdkClient::action(Action act, QObject * parent)
{
    OBJPOINTER(SdkMachine, _machine);

    CheckAction * result = new CheckAction(parent);

    switch (act)
    {
    case PauseResume:
        result->setIcon(QQIcon("media-playback-pause"),
                        QQIcon("media-playback-resume"));
        result->setText("Pause Video", "Resume Playing");
        result->setIconText("Pause", "Resume");
        result->setToolTip("Pause", "Resume");
        result->setChecked(isPaused());
        CONNECT(result, SIGNAL(toggled(bool)),
                this, SLOT(pauseResume(bool)));
        CONNECT(this, SIGNAL(doPause()),
                this, SLOT(setPauseResumeCheck()));
        CONNECT(this, SIGNAL(doResume()),
                this, SLOT(setPauseResumeCheck()));
        pauseResume_action = result;
        break;
    }

    return result;
}

void SdkClient::pauseResume(bool checked)
{
    if (checked)
        pause();
    else
        resume();
}

void SdkClient::setPauseResumeCheck(void)
{
    if (pauseResume_action)
    {
        OBJPOINTER(QAction, pauseResume_action);
        pauseResume_action->setChecked(isPaused());
    }
}

void SdkClient::pause(void)
{
    qDebug() << "SdkClient::pause() emit doPause()" << paused_bool;
    paused_bool = true;
    emit doPause();
}

void SdkClient::resume(void)
{
    qDebug() << "SdkClient::resume() emit doResume()" << paused_bool;
    paused_bool = false;
    emit doResume();
}

void SdkClient::startUrl(QString url)
{
    qDebug() << "SdkClient::startUrl()" << url << paused_bool;
    pause();
    newUrl_s = url;
    interface()->write("Input/URL", QString());
    currentPollCount_i = interface()->read("Options/PollCount").toInt();
    QTimer::singleShot(50, this, SLOT(checkPollCount()));
}

void SdkClient::checkPollCount(void)
{
    int i = interface()->read("Options/PollCount").toInt();
    qDebug() << "SdkClient::checkPollCount()"
             << currentPollCount_i << i;
    if (i - currentPollCount_i > 2)
        resume();
    else
        QTimer::singleShot(50, this, SLOT(checkPollCount()));
}
