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

#include <eirExe/Log.h>
#include <eirFile/QQDir.h>
#include <eirGUI/CancelAction.h>
#include <eirGUI/CheckAction.h>
#include <eirGUI/Gallery.h>
#include <eirGUI/GalleryItem.h>
#include <eirGUI/ImageLabel.h>
#include <eirGUI/QQIcon.h>
#include <eirSDK1/ImageDirSink.h>
#include <eirSDK1/SdkClient.h>
#include <eirSDK1/SdkCommand.h>
#include <eirTypes/QQSound.h>

void LivePage::enterAddEnroll(void)
{
    FUNCTION();
    STATE("Add Enroll");
    emit stateName("Add Enroll");

    OBJPOINTER(IdPane, idPane());
    OBJPOINTER(QAction, addEnroll_action);
    OBJPOINTER(Gallery, bot_gallery);
    OBJPOINTER(SdkClient, searchSdk());

    manualSearch_action->setEnabled(false);
    addEnroll_action->setEnabled(false);
    enrollNew_action->setEnabled(false);
    confirm_action->setEnabled(false);
    reject_action->setEnabled(false);
    finish_action->setEnabled(false);
    cancel_action->setEnabled(true);
    maintenance_action->setEnabled(false);

    bot_gallery->clearSelection();
    bot_gallery->setSelectionMode(Gallery::SelectMultiple);
    foreach (GalleryItem * top_gi, top_gallery->selectedList())
    {
        GVIPOINTER(GalleryItem, top_gi);
        qDebug() << "Moving" << top_gi->id();
        GalleryItem * bot_gi = bot_gallery->add(top_gi->image(), top_gi->id());
        GVIPOINTER(GalleryItem, bot_gi);
        bot_gallery->selectAdd(bot_gi);
        top_gallery->remove(top_gi);
    }

    idPane()->reset();
    idPane()->setId(resultPersonId);
    addEnroll_action->setChecked(true);
    addEnroll_action->setEnabled(true);
    TRACE("AddEnroll for %1", resultPersonId);
}

void LivePage::leaveAddEnroll(void)
{
    FUNCTION();
    STATE("-Add Enroll");

    OBJPOINTER(IdPane, idPane());
    OBJPOINTER(QAction, addEnroll_action);
    OBJPOINTER(Gallery, bot_gallery);
    OBJPOINTER(SdkClient, searchSdk());

    qDebug() << "LivePage::leaveAddEnroll()";
    addEnroll_action->setChecked(false);

    manualSearch_action->setEnabled(false);
    addEnroll_action->setEnabled(false);
    enrollNew_action->setEnabled(false);
    confirm_action->setEnabled(false);
    reject_action->setEnabled(false);
    finish_action->setEnabled(false);
    maintenance_action->setEnabled(false);

    if (cancel_action->isCancelled())
    {
        TRACE("AddEnroll cancelled");
        manualSearch_action->setChecked(false);
        addEnroll_action->setChecked(false);
        bot_gallery->clear();
        cancel_action->reset();
        emit enrollError();
        return;
    }

    cancel_action->setEnabled(false);
    idPane()->reset();
    idPane()->message("Enrolling...");

    SdkCommand command = searchSdk()->newCommand("Enroll", "Enroll");
    foreach (GalleryItem * gi, bot_gallery->selectedList())
    {
        GVIPOINTER(GalleryItem, gi);
        if (gi->id().startsWith("FK"))
            bot_gallery->unselect(gi);
        else
            command.addInputImage(gi->image(), gi->id());
    }
    command.set(VariableId("Settings/Enroll/PersonId"), resultPersonId);
    command.set(VariableId("Settings/Enroll/SetDescription"), QString());
    TRACE("Enroll/Enroll #%1 for %2",
          command.key(), resultPersonId);
    CONNECT(searchSdk(), SIGNAL(enrollResult(SdkCommand)),
            this, SLOT(enrollResult(SdkCommand)));
    searchSdk()->submit(command);
}

void LivePage::enterEnrollNew(void)
{
    FUNCTION();
    STATE("Enroll New");
    emit stateName("Enroll New");

    OBJPOINTER(Gallery, top_gallery);
    OBJPOINTER(Gallery, bot_gallery);
    OBJPOINTER(IdPane, idPane());
    OBJPOINTER(QAction, enrollNew_action);

    bot_gallery->clear();
    bot_gallery->setSelectionMode(Gallery::SelectMultiple);
    enrollNew_action->setChecked(true);
    enrollNew_action->setEnabled(true);
    cancel_action->setEnabled(true);

    foreach (GalleryItem * top_gi, top_gallery->selectedList())
    {
        GVIPOINTER(GalleryItem, top_gi);
        qDebug() << "Moving" << top_gi->id();
        GalleryItem * bot_gi = bot_gallery->add(top_gi->image(), top_gi->id());
        GVIPOINTER(GalleryItem, bot_gi);
        bot_gallery->selectAdd(bot_gi);
        top_gallery->remove(top_gi);
    }

    idPane()->reset();
    idPane()->edit();
}

void LivePage::leaveEnrollNew(void)
{
    FUNCTION();
    STATE("-Enroll New");

    OBJPOINTER(IdPane, idPane());
    OBJPOINTER(QAction, enrollNew_action);
    OBJPOINTER(Gallery, bot_gallery);
    OBJPOINTER(SdkClient, searchSdk());

    enrollNew_action->setChecked(false);
    idPane()->endEdit();
    manualSearch_action->setEnabled(false);
    addEnroll_action->setEnabled(false);
    enrollNew_action->setEnabled(false);
    confirm_action->setEnabled(false);
    reject_action->setEnabled(false);
    finish_action->setEnabled(false);
    maintenance_action->setEnabled(false);

    if (cancel_action->isCancelled())
    {
        TRACE("EnrollNew cancelled");
        bot_gallery->clear();
        enrollNew_action->setChecked(false);
        cancel_action->reset();
        emit enrollError();
        return;
    }

    cancel_action->setEnabled(false);
    QString desc(idPane()->token());
    QString personId(idPane()->personId());
    if (personId.isEmpty())
        personId = QString("Unknown-D%1-T%2")
                    .arg(QDate::currentDate().toString("yyyyMMdd"))
                    .arg(QTime::currentTime().toString("hhmmss"));
    mainw()->addToken(desc, personId);

    SdkCommand command = searchSdk()->newCommand("Enroll", "Enroll");
    command.set(VariableId("Settings/Enroll/PersonId"), personId);
    command.set(VariableId("Settings/Enroll/SetDescription"), desc);

    foreach (GalleryItem * gi, bot_gallery->items())
    {
        GVIPOINTER(GalleryItem, gi);
        command.addInputImage(gi->image(), gi->id());
    }
    CONNECT(searchSdk(), SIGNAL(enrollResult(SdkCommand)),
            this, SLOT(enrollResult(SdkCommand)));
    TRACE("Enroll/Enroll #%1 for %2 desc={%3} %4 images",
          command.key(), personId, desc,
          bot_gallery->size());
    searchSdk()->submit(command);
}

void LivePage::enterEnrollPending(void)
{
    FUNCTION();
    STATE("Enroll Pending");
    emit stateName("Enroll Pending");

    OBJPOINTER(IdPane, idPane());

    manualSearch_action->setEnabled(false);
    addEnroll_action->setEnabled(false);
    enrollNew_action->setEnabled(false);
    confirm_action->setEnabled(false);
    reject_action->setEnabled(false);
    finish_action->setEnabled(false);
    cancel_action->setEnabled(false);
    maintenance_action->setEnabled(false);
    idPane()->reset();
    idPane()->message("Enrolling");
}

void LivePage::leaveEnrollPending(void)
{
    FUNCTION();
    STATE("-Enroll Pending");

    OBJPOINTER(IdPane, idPane());

    manualSearch_action->setEnabled(false);
    addEnroll_action->setEnabled(false);
    enrollNew_action->setEnabled(false);
    confirm_action->setEnabled(false);
    reject_action->setEnabled(false);
    finish_action->setEnabled(false);
    cancel_action->setEnabled(false);
    maintenance_action->setEnabled(false);
    idPane()->reset();
}

void LivePage::addEnroll(bool checked)
{
    FUNCTION(checked);
    if (checked)
        emit doAddEnroll();
    else
        emit endAddEnroll();
}

void LivePage::enrollNew(bool checked)
{
    FUNCTION(checked);
    if (checked)
        emit doEnrollNew();
    else
        emit endEnrollNew();
}

void LivePage::enrollResult(SdkCommand result)
{
    FUNCTION(result.key());
    if (result.isValid())
    {
        enroll_result = result;
        TRACE("emit gotEnrollResult()");
        emit gotEnrollResult();
    }
    else
    {
        TRACE("enrollResult error: %1", result.getReason());
        idPane()->error("Enroll Error: " + result.getReason());
        idPane()->resetIn(5000);
        TRACE("emit enrollError()");
        emit enrollError();
    }
    DISCONNECT(searchSdk(), SIGNAL(enrollResult(SdkCommand)),
            this, SLOT(enrollResult(SdkCommand)));
}

