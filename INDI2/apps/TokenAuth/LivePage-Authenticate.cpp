#include "MainWindow.h"
#include "LivePage.h"
#include "IdPane.h"

#include <QtCore/QDate>
#include <QtCore/QTime>
#include <QtCore/QTimer>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>
#include <QtGui/QToolBar>

#include <eirTypes/QQSound.h>
#include <eirExe/Log.h>
#include <eirFile/QQDir.h>
#include <eirGUI/CancelAction.h>
#include <eirGUI/CheckAction.h>
#include <eirGUI/Gallery.h>
#include <eirGUI/GalleryItem.h>
#include <eirGUI/ImageLabel.h>
#include <eirExe/Log.h>
#include <eirSDK1/ImageDirSink.h>
#include <eirSDK1/SdkClient.h>
#include <eirSDK1/SdkCommand.h>

DEFINE_ENUMERATION(AuthStatus, AUTHSTATUS_ENUM);

void LivePage::enterAuthenticate(void)
{
    qDebug() << "LivePage::enterAuthenticate()";
    emit stateName("Authenticate");

    mainw()->markedFaceDirSink()->clear();
    top_gallery->clear();
    bot_gallery->clear();
    CONNECT(top_gallery, SIGNAL(removed(GalleryItem*)),
            this, SLOT(removeAuthFace(GalleryItem*)));
    CONNECT(this, SIGNAL(statusChange(AuthStatus)),
            this, SLOT(updateActions(AuthStatus)));
    idPane()->setToken(mainw()->authenticateToken());
    emit statusChange(AuthStatus::Start);

    QString faceDirName(searchSdk()->dirName("Search/InputDir"));
    authenticationFace_dir.setPath(faceDirName);
    QQDir::removeAll(authenticationFace_dir);
    mainw()->markedFaceDirSink()->setMoveAfter(authenticationFace_dir);
    gatherSdk()->resume();

    SdkCommand command = searchSdk()->newCommand("Retrieve", "Retrieve");
    command.set("Settings/Retrieve/PersonId", mainw()->authenticatePersonId());
    CONNECT(searchSdk(), SIGNAL(retrieveResult(SdkCommand)),
            this, SLOT(authRetrieveResult(SdkCommand)));
    searchSdk()->submit(command);
}

void LivePage::enterAuthPass(void)
{
    qDebug() << "LivePage::enterAuthPass()";
    emit stateName("AuthPass");
    QQSound::play("../TPIRdata/sound/Pass.wav");
}

void LivePage::enterAuthFail(void)
{
    qDebug() << "LivePage::enterAuthFail()";
    emit stateName("AuthFail");
    QQSound::play("../TPIRdata/sound/Fail.wav");
}

void LivePage::enterAuthCancel(void)
{
    qDebug() << "LivePage::enterAuthCancel()";
    emit stateName("AuthCancel");

    if (authStatus_enum < AuthStatus::Complete)
    {
        gatherSdk()->pause();
        top_gallery->clear();
        authenticationFace_dir.removeAll();
    }

    leaveAuthFinish();
    cancel_action->reset();
}

void LivePage::enterAuthFinish(void)
{
    qDebug() << "LivePage::enterAuthFinish()";
    emit stateName("AuthFinish");
}

void LivePage::leaveAuthFinish(void)
{
    qDebug() << "LivePage::leaveAuthFinish()";
    emit statusChange(AuthStatus::Idle);
    DISCONNECT(top_gallery, SIGNAL(removed(GalleryItem*)),
               this, SLOT(removeAuthFace(GalleryItem*)));
    DISCONNECT(this, SIGNAL(statusChange(AuthStatus)),
               this, SLOT(updateActions(AuthStatus)));
    mainw()->markedFaceDirSink()->setMoveAfter(QQDir::nullDir);
}

void LivePage::enterConfirm(void)
{
    qDebug() << "LivePage::enterConfirm()";
    emit stateName("Confirm");
    gatherSdk()->pause();
    QString authenticate = searchSdk()->interface()->value("Search/Authenticate").toString();
    int confidence = searchSdk()->interface()->value("Search/Confidence").toInt();
    qDebug() << mainw()->authenticatePersonId() << authenticate << confidence;
    idPane()->progress(1000, 0, confidence);
    if ("Pass" == authenticate)
    {
        resultPersonId = mainw()->authenticatePersonId();
        emit statusChange(AuthStatus::AutoPass);
        emit confirmed();
    }
    else if ("Fail" == authenticate)
    {
        emit statusChange(AuthStatus::AutoFail);
        emit rejected();
    }
    else
    {
        QQSound::play("../TPIRdata/sound/Maybe.wav");
        emit statusChange(AuthStatus::Maybe);
    }
}


void LivePage::confirm(void)
{
    qDebug() << "LivePage::confirm()";
    emit statusChange((AuthStatus::Maybe == authStatus_enum)
                      ? AuthStatus::ManualPass
                      : AuthStatus::OverridePass);
    resultPersonId = mainw()->authenticatePersonId();
    emit confirmed();
}

void LivePage::reject(void)
{
    qDebug() << "LivePage::reject()";
    emit statusChange((AuthStatus::Maybe == authStatus_enum)
                      ? AuthStatus::ManualFail
                      : AuthStatus::OverrideFail);
    emit rejected();
}

void LivePage::authRetrieveResult(SdkCommand result)
{
    FUNCTION(result.key());
    DISCONNECT(searchSdk(), SIGNAL(retrieveResult(SdkCommand)),
            this, SLOT(authRetrieveResult(SdkCommand)));

    RetrieveCommand retrieve(result);
    retrieve.parseResult();
    retrieve.parseOutput();
    DUMPVSET(retrieve);

    int nFaces = retrieve.resultCount();
    for (int kFace = 1; kFace <= nFaces; ++kFace)
    {
        QImage img(retrieve.resultFace(kFace));
        int faceKey = retrieve.faceKey(kFace);
        QString faceId(QString("FK%1").arg(faceKey, 6, 10, QChar('0')));
        TRACE("Adding %1x%2 as %3", img.width(), img.height(), faceId);
        bot_gallery->add(img, faceId);
    }

    idPane()->progress(1000, 0, 0);

    emit statusChange(AuthStatus::Collect);
    SdkCommand command = searchSdk()->newCommand("Search", "Authenticate");
    command.set("Settings/Search/PersonId", mainw()->authenticatePersonId());
    command.setTimeoutMsec(7000);
    CONNECT(searchSdk(), SIGNAL(searchResult(SdkCommand)),
            this, SLOT(authSearchResult(SdkCommand)));
    CONNECT(searchSdk(), SIGNAL(notifyUpdate()),
            this, SLOT(authUpdate()));
    searchSdk()->submit(command);
}

void LivePage::authUpdate(void)
{
    QVariant v = searchSdk()->interface()->read("Search/Confidence");
    int confidence = v.toInt();
    qDebug() << "LivePage::authUpdate()" << v << confidence;
    idPane()->progress(1000, 0, confidence);
    QQSound::play("../TPIRdata/sound/Update.wav");
}

void LivePage::authSearchResult(SdkCommand result)
{
    qDebug() << "LivePage::authRetrieveResult()" << result.key();
    DISCONNECT(searchSdk(), SIGNAL(searchResult(SdkCommand)),
               this, SLOT(authSearchResult(SdkCommand)));
    CONNECT(searchSdk(), SIGNAL(notifyUpdate()),
            this, SLOT(authUpdate()));
    DUMPVSET(result);
    if (result.isError())
    {
        emit statusChange(AuthStatus::Error);
        emit authResultError();
    }
    else
    {
        emit statusChange(AuthStatus::Complete);
        emit gotAuthResult();
    }
}

void LivePage::removeAuthFace(GalleryItem * gi)
{
    GVIPOINTER(GalleryItem, gi);
    authenticationFace_dir.remove(gi->id() + ".PNG");
}

void LivePage::updateActions(AuthStatus auth)
{
    qDebug() << "LivePage::authUpdate()" << auth;

    idPane()->setId(tr("%1: %2")
                    .arg(auth.name())
                    .arg(mainw()->authenticatePersonId()));
    authStatus_enum = auth;

    manualSearch_action->setEnabled(auth > AuthStatus::Error);
    manualSearch_action->setChecked(false);

    addEnroll_action->setEnabled(AuthStatus::AutoPass == auth
                                 || AuthStatus::ManualPass == auth
                                 || AuthStatus::OverridePass == auth);
    addEnroll_action->setChecked(false);

    enrollNew_action->setEnabled(AuthStatus::AutoFail == auth
                                 || AuthStatus::ManualFail == auth
                                 || AuthStatus::OverrideFail == auth);
    enrollNew_action->setChecked(false);

    confirm_action->setEnabled(AuthStatus::Collect == auth
                               || auth > AuthStatus::Error);
    confirm_action->setChecked(AuthStatus::AutoPass == auth
                               || AuthStatus::ManualPass == auth
                               || AuthStatus::OverridePass == auth);

    reject_action->setEnabled(AuthStatus::Collect == auth
                              || auth > AuthStatus::Error);
    reject_action->setChecked(AuthStatus::AutoFail == auth
                              || AuthStatus::ManualFail == auth
                              || AuthStatus::OverrideFail == auth);

    finish_action->setEnabled(auth > AuthStatus::Error
                              && auth != AuthStatus::Maybe);
    finish_action->setChecked(false);

    cancel_action->setEnabled(true);
    cancel_action->setChecked(false);

    maintenance_action->setEnabled(false);
    maintenance_action->setChecked(false);
}
