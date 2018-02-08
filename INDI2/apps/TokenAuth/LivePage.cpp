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

LivePage::LivePage(MainWindow * parent)
    : QWidget(parent)
    , authStatus_enum(AuthStatus::Idle)
    , mw_ptr(parent)
    , top_gallery(new Gallery(&top_galleryParameters, this))
    , bot_gallery(new Gallery(&bot_galleryParameters, this))
    , top_image(new ImageLabel(QImage(":/icon/eIRonly.svg"), this))
    , bot_image(new ImageLabel(QImage(":/icon/eIRonly.svg"), this))
    , live_toolBar(0)
    , manualSearch_action(0)
    , addEnroll_action(0)
    , enrollNew_action(0)
    , confirm_action(0)
    , reject_action(0)
    , finish_action(0)
    , cancel_action(0)
    , maintenance_action(0)
    , quit_action(0)
{
    FUNCTION();
    OBJPOINTER(Gallery, top_gallery);
    OBJPOINTER(Gallery, bot_gallery);
    OBJPOINTER(ImageLabel, top_image);
    OBJPOINTER(ImageLabel, bot_image);
    setObjectName("TokenAuth:LivePage");
    top_gallery->setObjectName("top_gallery");
    bot_gallery->setObjectName("bot_gallery");
    top_image->setObjectName("top_image");
    bot_image->setObjectName("bot_image");
    QTimer::singleShot(10, this, SLOT(setupUi()));
}

MainWindow * LivePage::mainw(void) const
{
    OBJPOINTER(MainWindow, mw_ptr);
    return mw_ptr;
}

CancelAction * LivePage::cancelAction(void) const
{
    return cancel_action;
}

QAction * LivePage::maintenanceAction(void) const
{
    OBJPOINTER(QAction, maintenance_action);
    return maintenance_action;
}

QAction * LivePage::finishAction(void) const
{
    OBJPOINTER(QAction, finish_action);
    return finish_action;
}

QAction * LivePage::quitAction(void) const
{
    OBJPOINTER(QAction, quit_action);
    return quit_action;
}


IdPane * LivePage::idPane(void) const
{
    return mainw()->idPane();
}

SdkClient * LivePage::gatherSdk(void) const
{
    return mainw()->gatherSdk();
}

SdkClient * LivePage::searchSdk(void) const
{
    return mainw()->searchSdk();
}

QToolBar * LivePage::toolBar(void) const
{
    OBJPOINTER(QToolBar, live_toolBar);
    return live_toolBar;
}

Gallery * LivePage::bottomGallery(void) const
{
    OBJPOINTER(Gallery, bot_gallery);
    return bot_gallery;
}

void LivePage::setupUi(void)
{
    FUNCTION();
    OBJPOINTER(Gallery, top_gallery);

    QGridLayout * live_layout = new QGridLayout;
    top_image->setFixedSize(QSize(320, 240));
    bot_image->setFixedSize(QSize(320, 240));
    live_layout->addWidget(top_image, 0, 0);
    live_layout->addWidget(bot_image, 1, 0);
    live_layout->addWidget(top_gallery->widget(),    0, 1);
    live_layout->addWidget(bot_gallery->widget(),    1, 1);
    live_layout->setAlignment(top_image, Qt::AlignCenter);
    live_layout->setAlignment(bot_image, Qt::AlignCenter);
    QWidget::setLayout(live_layout);
    top_image->clear();
    bot_image->clear();
    top_galleryParameters.setMaxItems(50);
    top_gallery->setSelectionMode(Gallery::SelectMultiple);
    bot_gallery->setSelectionMode(Gallery::SelectSingle);

    live_toolBar = mainw()->addToolBar("Live");
    live_toolBar->setEnabled(false);
    live_toolBar->setIconSize(QSize(48,48));
    live_toolBar->addAction(gatherSdk()->action(SdkClient::PauseResume));
    live_toolBar->addAction(top_gallery->action(Gallery::ClearSelection));
    live_toolBar->addAction(top_gallery->action(Gallery::RemoveUnselected));
    manualSearch_action = new CheckAction(this);
    manualSearch_action->setIcon(QQIcon("database-enrollment-search"), QQIcon("action-proceed"));
    manualSearch_action->setText("Search Selected", "Search Complete");
    manualSearch_action->setIconText("Search", "Complete");
    live_toolBar->addAction(manualSearch_action);
    addEnroll_action = new CheckAction(this);
    addEnroll_action->setIcon(QQIcon("database-enrollment-add"), QQIcon("action-proceed"));
    addEnroll_action->setText("Add to Enrollment", "Complete Enrollment");
    addEnroll_action->setIconText("Add", "Complete");
    live_toolBar->addAction(addEnroll_action);
    enrollNew_action = new CheckAction(this);
    enrollNew_action->setIcon(QQIcon("database-enrollment-new"), QQIcon("action-proceed"));
    enrollNew_action->setText("Enroll New", "Complete Enrollment");
    enrollNew_action->setIconText("New", "Complete");
    live_toolBar->addAction(enrollNew_action);
    confirm_action = new CheckAction(this);
    confirm_action->setIcon(QQIcon("action-confirm"));
    confirm_action->setText("Confirm", "Confirmed");
    live_toolBar->addAction(confirm_action);
    reject_action = new CheckAction(this);
    reject_action->setIcon(QQIcon("action-reject"));
    reject_action->setText("Reject", "Rejected");
    live_toolBar->addAction(reject_action);
    finish_action = new CheckAction(this);
    finish_action->setIcon(QQIcon("action-finish"), QQIcon("Finished"));
    finish_action->setText("Finish", "Finished");
    live_toolBar->addAction(finish_action);
    cancel_action = new CancelAction(this);
    live_toolBar->addAction(cancel_action);
    maintenance_action = new QAction(QQIcon("Search"), "Maintenance", this);
    OBJPOINTER(QAction, maintenance_action);
    live_toolBar->addAction(maintenance_action);
    quit_action = new QAction(QQIcon("action-quit"), "Quit", this);
    OBJPOINTER(QAction, quit_action);
    live_toolBar->addAction(quit_action);

    manualSearch_action->setEnabled(false);
    addEnroll_action->setEnabled(false);
    enrollNew_action->setEnabled(false);
    confirm_action->setEnabled(false);
    reject_action->setEnabled(false);
    finish_action->setEnabled(false);
    cancel_action->setEnabled(false);
    maintenance_action->setEnabled(false);
    quit_action->setEnabled(true);

    CONNECT(manualSearch_action, SIGNAL(toggled(bool)),
            this, SLOT(manualSearch(bool)));
    CONNECT(addEnroll_action, SIGNAL(toggled(bool)),
            this, SLOT(addEnroll(bool)));
    CONNECT(enrollNew_action, SIGNAL(toggled(bool)),
            this, SLOT(enrollNew(bool)));
    CONNECT(confirm_action, SIGNAL(triggered()),
            this, SLOT(confirm()));
    CONNECT(reject_action, SIGNAL(triggered()),
            this, SLOT(reject()));
    CONNECT(quit_action, SIGNAL(triggered()),
            qApp, SLOT(quit()));
}

void LivePage::enterReady(void)
{
    FUNCTION();
    OBJPOINTER(QToolBar, live_toolBar);
    OBJPOINTER(QAction, enrollNew_action);
    OBJPOINTER(QAction, addEnroll_action);
    OBJPOINTER(QAction, manualSearch_action);
    OBJPOINTER(Gallery, top_gallery);
    OBJPOINTER(Gallery, bot_gallery);

    emit stateName("Ready");
    STATE("Ready");

    mainw()->setPage(this);
    live_toolBar->setEnabled(true);
    live_toolBar->setVisible(true);
    manualSearch_action->setEnabled(top_gallery->selectedList().size());
    addEnroll_action->setEnabled(false);
    enrollNew_action->setEnabled(false);
    confirm_action->setEnabled(false);
    reject_action->setEnabled(false);
    finish_action->setEnabled(false);
    cancel_action->setEnabled(false);
    maintenance_action->setEnabled(true);
    enrollNew_action->setChecked(false);
    addEnroll_action->setChecked(false);
    manualSearch_action->setChecked(false);
    mainw()->setTokenKeyEnable(true);
    mainw()->setSourceKeyEnable(true);
    CONNECT(top_gallery, SIGNAL(hasSelected(bool)),
            manualSearch_action, SLOT(setEnabled(bool)));
    bot_gallery->clear();
    resultPersonId.clear();
    idPane()->reset();
}

void LivePage::leaveReady(void)
{
    FUNCTION();
    STATE("-Ready");

    DISCONNECT(top_gallery, SIGNAL(hasSelected(bool)),
               manualSearch_action, SLOT(setEnabled(bool)));
    manualSearch_action->setEnabled(false);
    addEnroll_action->setEnabled(false);
    enrollNew_action->setEnabled(false);
    confirm_action->setEnabled(false);
    reject_action->setEnabled(false);
    finish_action->setEnabled(false);
    cancel_action->setEnabled(false);
    maintenance_action->setEnabled(false);
    mainw()->setTokenKeyEnable(true);
    mainw()->setSourceKeyEnable(true);
    idPane()->reset();
}

void LivePage::gotMarkedImage(QImage image, QString id)
{
    OBJPOINTER(ImageLabel, top_image);
    top_image->set(image.scaled(top_image->size(), Qt::KeepAspectRatio));
}

void LivePage::gotMarkedFace(QImage image, QString id)
{
    OBJPOINTER(Gallery, top_gallery);
    top_gallery->add(image, id);
}


