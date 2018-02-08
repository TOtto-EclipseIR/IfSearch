#ifndef LIVEPAGE_H
#define LIVEPAGE_H

#include <QtGui/QWidget>

class QToolBar;

#include <eirBase/Enumeration.h>
#include <eirFile/QQDir.h>
#include <eirGUI/GalleryParameters.h>
#include <eirSDK1/RetrieveCommand.h>
#include <eirSDK1/SdkCommand.h>
#include <eirSDK1/SearchCommand.h>
class CancelAction;
class CheckAction;
class Gallery;
class GalleryItem;
class ImageLabel;
class SdkClient;

class MainWindow;
class IdPane;

#define AUTHSTATUS_ENUM(NV) \
    NV(Idle, = 0)  \
    NV(Cancelled, ) \
    NV(Start,)   \
    NV(Collect,)   \
    NV(Complete,)   \
    NV(Error,)   \
    NV(AutoPass,)   \
    NV(AutoFail,)   \
    NV(Maybe,)   \
    NV(ManualPass,)   \
    NV(ManualFail,)   \
    NV(OverridePass,)   \
    NV(OverrideFail,)   \

class AuthStatus : public Enumeration
{
    DECLARE_ENUMERATION(AuthStatus, AUTHSTATUS_ENUM)
};


class LivePage : public QWidget
{
    Q_OBJECT

private:
    AuthStatus authStatus_enum;

public:
    explicit LivePage(MainWindow * parent=0);
    Gallery * bottomGallery(void) const;
    QToolBar * toolBar(void) const;
    CancelAction * cancelAction(void) const;
    QAction * maintenanceAction(void) const;
    QAction * finishAction(void) const;
    QAction * quitAction(void) const;

signals:
    void stateName(QString newText);
    void confirmed(void);
    void rejected(void);
    void statusChange(AuthStatus auth);
    // actions
    void doManualSearch(void);
    void endManualSearch(void);
    void doAddEnroll(void);
    void endAddEnroll(void);
    void doEnrollNew(void);
    void endEnrollNew(void);
    void endPersonResult(void);
    // SDK notifications
    void gotSearchResult(void);
    void searchResultError(void);
    void gotAuthResult(void);
    void authResultError(void);
    void gotEnrollResult(void);
    void enrollError(void);
    void gotRetrieveResult(void);
    void retrieveError(void);

public slots:
    void setupUi(void);
    void removeAuthFace(GalleryItem * gi);
    void updateActions(AuthStatus auth);
    // actions
    void manualSearch(bool checked);
    void addEnroll(bool checked);
    void enrollNew(bool checked);
    void confirm(void);
    void reject(void);
    // SDK
    void gotMarkedFace(QImage image, QString id);
    void gotMarkedImage(QImage image, QString id);
    void searchResult(SdkCommand result);
    void enrollResult(SdkCommand result);
    void retrieveResult(SdkCommand result);
    void displayRetrieveResult(void);
    void personImageSelected(GalleryItem * item);
    void personResultClicked(void);
    void authRetrieveResult(SdkCommand result);
    void authSearchResult(SdkCommand result);
    void authUpdate(void);
    // state transitions
    void enterReady(void);
    void leaveReady(void);
    void enterManualSearch(void);
    void enterSearchResult(void);
    void leaveSearchResult(void);
    void enterPersonResult(void);
    void leavePersonResult(void);
    void enterAddEnroll(void);
    void leaveAddEnroll(void);
    void enterEnrollNew(void);
    void leaveEnrollNew(void);
    void enterEnrollPending(void);
    void leaveEnrollPending(void);
    void enterAuthenticate(void);
    void enterAuthPass(void);
    void enterAuthFail(void);
    void enterAuthCancel(void);
    void enterAuthFinish(void);
    void leaveAuthFinish(void);
    void enterConfirm(void);

private:
    MainWindow * mainw(void) const;
    SdkClient * gatherSdk(void) const;
    SdkClient * searchSdk(void) const;
    IdPane * idPane(void) const;

private:
    MainWindow * mw_ptr;
    SearchCommand search_result;
    int personResult_rank;
    SdkCommand enroll_result;
    RetrieveCommand retrieve_result;
    QString resultPersonId;
    QQDir authenticationFace_dir;

    GalleryParameters top_galleryParameters;
    GalleryParameters bot_galleryParameters;
    Gallery * top_gallery;
    Gallery * bot_gallery;
    ImageLabel * top_image;
    ImageLabel * bot_image;
    QToolBar * live_toolBar;
    CheckAction * manualSearch_action;
    CheckAction * addEnroll_action;
    CheckAction * enrollNew_action;
    CheckAction * confirm_action;
    CheckAction * reject_action;
    CheckAction * finish_action;
    CancelAction * cancel_action;
    QAction * maintenance_action;
    QAction * quit_action;

};

#endif // LIVEPAGE_H
