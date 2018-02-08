#include "MainWindow.h"
#include "MaintenancePage.h"

#include <QtDebug>
#include <QtCore/QByteArray>
#include <QtCore/QFile>
#include <QtCore/QStringList>
#include <QtCore/QTimer>
#include <QtGui/QAction>
#include <QtGui/QHBoxLayout>
#include <QtGui/QMessageBox>
#include <QtGui/QToolBar>

#include <eirExe/Log.h>
#include <eirGUI/Gallery.h>
#include <eirGUI/GalleryItem.h>
#include <eirGUI/QQIcon.h>
#include <eirSDK1/RetrieveAllCommand.h>
#include <eirSDK1/SdkClient.h>
#include <eirSDK1/SdkCommand.h>

#include "IdPane.h"
#include "LivePage.h"

MaintenancePage::MaintenancePage(MainWindow * parent)
    : QWidget(parent)
    , mw_ptr(parent)
    , maint_toolbar(0)
    , clearTaib_action(0)
    , importTaib_action(0)
    , exportTaib_action(0)
    , cancel_action(0)
    , finish_action(0)
    , gallery_parms(QString())
    , _gallery(new Gallery(&gallery_parms, this))
    , people_dir("../TPIRdata/people")
{
    FUNCTION();
    setObjectName("TokenAuth:MaintenancePage");
    QTimer::singleShot(10, this, SLOT(setupUi()));
}

MainWindow * MaintenancePage::mainw(void) const
{
    OBJPOINTER(MainWindow, mw_ptr);
    return mw_ptr;
}

SdkClient * MaintenancePage::searchSdk(void) const
{
    return mainw()->searchSdk();
}

IdPane * MaintenancePage::idPane(void) const
{
    return mainw()->idPane();
}

QAction * MaintenancePage::finishAction(void) const
{
    OBJPOINTER(QAction, finish_action);
    return finish_action;
}


void MaintenancePage::setupUi(void)
{
    FUNCTION();
    maint_toolbar = mainw()->addToolBar("Maintenance");
    clearTaib_action = new QAction(QQIcon("database-enrollment-clear"), "Clear TAIB", this);
    importTaib_action = new QAction(QQIcon("database-enrollment-import"), "Import TAIB", this);
    exportTaib_action = new QAction(QQIcon("database-enrollment-export"), "Export TAIB", this);
    cancel_action  = new QAction(QQIcon("action-cancel"), "Cancel", this);
    finish_action  = new QAction(QQIcon("action-finish"), "Finished", this);
    OBJPOINTER(QToolBar, maint_toolbar);
    OBJPOINTER(QAction, clearTaib_action);
    OBJPOINTER(QAction, importTaib_action);
    OBJPOINTER(QAction, exportTaib_action);
    OBJPOINTER(QAction, cancel_action);
    OBJPOINTER(QAction, finish_action);
    maint_toolbar->setIconSize(QSize(48,48));
    maint_toolbar->setVisible(false);
    clearTaib_action->setEnabled(false);
    importTaib_action->setEnabled(false);
    exportTaib_action->setEnabled(false);
    cancel_action->setEnabled(false);
    finish_action->setEnabled(false);
    maint_toolbar->addAction(clearTaib_action);
    maint_toolbar->addAction(importTaib_action);
    maint_toolbar->addAction(exportTaib_action);
    maint_toolbar->addSeparator();
    maint_toolbar->addAction(cancel_action);
    maint_toolbar->addAction(finish_action);
    CONNECT(clearTaib_action, SIGNAL(triggered()),
            this, SLOT(clearTaib()));
    CONNECT(importTaib_action, SIGNAL(triggered()),
            this, SLOT(importTaib()));

    QHBoxLayout * hBox = new QHBoxLayout(this);
    OBJPOINTER(QHBoxLayout, hBox);
    OBJPOINTER(Gallery, _gallery);
    hBox->addWidget(_gallery->widget());
    setLayout(hBox);
}

void MaintenancePage::enterMaintenance(void)
{
    FUNCTION();
    STATE("Maintenance");
    emit stateName("Maintenance");

    mainw()->gatherSdk()->pause();
    mainw()->setPage(this);
    mainw()->live()->toolBar()->setVisible(false);
    maint_toolbar->setVisible(true);
    _gallery->clear();
    mainw()->setSourceKeyEnable(false);
    initiateRetriveAll();
}

void MaintenancePage::leaveMaintenance(void)
{
    FUNCTION();
    STATE("-Maintenance");
    maint_toolbar->setVisible(false);
    clearTaib_action->setEnabled(false);
    importTaib_action->setEnabled(false);
    exportTaib_action->setEnabled(false);
    cancel_action->setEnabled(false);
    finish_action->setEnabled(false);
}

void MaintenancePage::clearTaib(void)
{
    FUNCTION();
    QMessageBox * msgBox = new QMessageBox(this);
    msgBox->setText("Clear TokenAuth INDIbase");
    msgBox->setInformativeText("Do you want to remove ALL enrollment from the system?");
    msgBox->setIcon(QMessageBox::Question);
    msgBox->setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
    int rc = msgBox->exec();
    if (QMessageBox::Yes == rc)
    {
        idPane()->message("Destroying INDIbase");
        SdkCommand command = searchSdk()->newCommand("Enroll", "RemoveAll");
        command.set("Settings/Enroll/PersonId", "Destroy");
        CONNECT(searchSdk(), SIGNAL(enrollResult(SdkCommand)),
                this, SLOT(removeAllResult(SdkCommand)));
        searchSdk()->submit(command);
    }
}
void MaintenancePage::importTaib(void)
{
    FUNCTION();
    personId_list = people_dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    TRACE("%1 People to Import", personId_list.size());
    if (personId_list.isEmpty())
    {
        idPane()->error("No People in Import Directory"
                        + people_dir.absolutePath());
        initiateRetriveAll();
    }
    else
    {
        CONNECT(searchSdk(), SIGNAL(enrollResult(SdkCommand)),
                this, SLOT(importEnrollResult(SdkCommand)));
        idPane()->progress(personId_list.size(), 0, imported_k = 0);
        QTimer::singleShot(10, this, SLOT(importNext()));
    }
}

void MaintenancePage::importNext(void)
{
    FUNCTION();
    if (personId_list.isEmpty())
    {
        idPane()->progress(++imported_k);
        DISCONNECT(searchSdk(), SIGNAL(enrollResult(SdkCommand)),
                this, SLOT(importEnrollResult(SdkCommand)));
        initiateRetriveAll();
        return;
    }

    QString personId = personId_list.takeFirst();
    DETAIL("Importing %1", personId);
    idPane()->progress(++imported_k);
    idPane()->setId(personId);
    QDir enroll_dir(people_dir);
    if (enroll_dir.cd(personId))
        if (enroll_dir.cd("enroll"))
        {
            QString desc;
            QFile token_file(enroll_dir.filePath("desc.txt"));
            if (token_file.open(QIODevice::ReadOnly))
            {
                QByteArray ba = token_file.readAll();
                desc = ba.simplified();
                mainw()->addToken(desc, personId);
            }
            SdkCommand command = searchSdk()->newCommand("Enroll", "Enroll");
            command.setTimeoutMsec(2000);
            command.set("Settings/Enroll/PersonId", personId);
            command.set("Settings/Enroll/InputDir", enroll_dir.absolutePath());
            command.set("Settings/Enroll/SetDescription", desc);
            qDebug() << "Enrolling" << personId << desc;
            searchSdk()->submit(command);
            return;
        }
    QTimer::singleShot(10, this, SLOT(importNext()));
}

void MaintenancePage::importEnrollResult(SdkCommand result)
{
    FUNCTION(result.key());
    QTimer::singleShot(500, this, SLOT(importNext()));
}

void MaintenancePage::initiateRetriveAll(void)
{
    FUNCTION();
    SdkCommand command = mainw()->searchSdk()->newCommand("Retrieve", "RetrieveAll");
    command.setTimeoutMsec(60000);
    CONNECT(mainw()->searchSdk(), SIGNAL(retrieveResult(SdkCommand)),
            this, SLOT(displayRetrieveAll(SdkCommand)));
    mainw()->searchSdk()->submit(command);
}


void MaintenancePage::displayRetrieveAll(SdkCommand result)
{
    FUNCTION(result.key());
    DISCONNECT(mainw()->searchSdk(), SIGNAL(retrieveResult(SdkCommand)),
            this, SLOT(displayRetrieveAll(SdkCommand)));
    LOG(dump(result));

    if (result.isError())
    {
        idPane()->error(result.getReason());
        clearTaib_action->setEnabled(false);
        importTaib_action->setEnabled(false);
        exportTaib_action->setEnabled(false);
        cancel_action->setEnabled(false);
        finish_action->setEnabled(true);
    }

    RetrieveAllCommand retrieveAll(result);
    retrieveAll.parseResult();
    retrieveAll.parseOutput();
    DUMPVSET(retrieveAll);
    int nPersons = retrieveAll.personCount();
    for (int kPerson = 1; kPerson <= nPersons; ++kPerson)
    {
        QString personId(retrieveAll.personId(kPerson));
        int nFace = retrieveAll.faceCount(kPerson);
        DATA("Display All for %1 %2 faces", personId, nFace);
        for (int kFace = 1; kFace <= nFace; ++kFace)
        {
            int faceKey = retrieveAll.faceKey(kPerson, kFace);
            QImage img(retrieveAll.faceImage(kPerson, kFace));
            TRACE("FaceKey=%1 %2x%3 image", faceKey, img.width(), img.height());
            GalleryItem * gi = _gallery->add(img, QString("FK%1")
                                                    .arg(faceKey, 6, 10, QChar('0')));
            gi->addDecoration(GalleryItem::YinAndYang);
            if (1 == kFace)
            {
                gi->addDecoration(GalleryItem::Title, personId);
                QChar c(mainw()->tokenChar(mainw()->tokenFor(personId)));
                if ( ! c.isNull())
                    gi->addDecoration(GalleryItem::Chip, c);
            }
        }
        GalleryItem::toggleYinAndYang();
    }
    clearTaib_action->setEnabled(nPersons > 1);
    importTaib_action->setEnabled(true);
    exportTaib_action->setEnabled(false);
    cancel_action->setEnabled(false);
    finish_action->setEnabled(true);
    qDebug() << "MaintenancePage::displayRetrieveAll() finish"
             << finish_action->isEnabled();
}

void MaintenancePage::removeAllResult(SdkCommand result)
{
    FUNCTION(result.key());
    DISCONNECT(searchSdk(), SIGNAL(enrollResult(SdkCommand)),
            this, SLOT(removeAllResult(SdkCommand)));
    if (result.isError())
        idPane()->error(result.getReason());
    else
        idPane()->reset();
    QTimer::singleShot(10, this, SLOT(enterMaintenance()));
}
