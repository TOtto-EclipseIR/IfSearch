#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#define __MODULE__ "TokenAuth"

#include <eirExe/GuiApplication.h>

#include <QtCore/QDir>
#include <QtCore/QStringList>
class QBoxLayout;
class QLabel;
class QSettings;
class QStackedLayout;

#include <eirGui/GalleryParameters.h>
#include <eirSDK1/SdkClientParameters.h>
#include <eirSDK1/SdkCommand.h>

class Gallery;
class GalleryItem;
class ImageLabel;
class SdkClient;
class ImageDirSink;

class IdPane;
class IOWidget;
class StateMachine;

class LivePage;
class MaintenancePage;

class MainWindow : public GuiApplication
{
    Q_OBJECT

public:
    MainWindow(QWidget * parent=0);
    LivePage * live(void) const;
    MaintenancePage * maint(void) const;
    SdkClient * gatherSdk(void) const;
    SdkClient * searchSdk(void) const;
    IdPane * idPane(void) const;
    ImageDirSink * markedFaceDirSink(void) const;
    void setPage(QWidget * page);
    void addToken(const QString & token,
                  const QString & personId);
    QString tokenFor(const QString & personId);
    QChar tokenChar(const QString & s);
    QString tokenString(const QChar c);
    void setTokenKeyEnable(const bool enable=true);
    void setSourceKeyEnable(const bool enable=true);
    QString authenticatePersonId(void) const;
    QString authenticateToken(void) const;

public slots:
    void start(void);
    void setupUi(void);
    void setupStatusBar(void);
    void initialize(void);
    void initError(void);
    void started(void);
    void personReportResult(SdkCommand result);

    // state transitions
    void enterInitialize(void);
    void leaveInitialize(void);
    void enterShutdown(void);
    void enterError(void);

signals:
    void allStarted(void);
    void gotPersonReport(void);
    void initializeError(void);
    void stateName(QString newText);
    void keyToken(QString token);
    void shiftPersonId(QString personId);
    void altPersonId(QString personId);
    void gotToken(void);

protected:
    void keyPressEvent(QKeyEvent * e);
    void resizeEvent(QResizeEvent * e);

private slots:
    void areAllStarted(void);
    void readPersonReport(void);
    void checkToken(QString token);
    void personVideoKey(QString personId);
    void startPersonVideo(void);

public:
    const static QString tokenPrefix;

private:
    StateMachine * _machine;
    SdkClientParameters gather_parms;
    SdkClientParameters search_parms;
    SdkClient * gather_sdk;
    SdkClient * search_sdk;
    QObjectList startPending_list;
    ImageDirSink * markedFace_dir;
    ImageDirSink * marked_dir;
    QMap<QString, QString> token_personId_map;
    QMap<QString, QString> personId_token_map;
    QString authenticateToken_;
    QString authenticatePersonId_;
    QDir personVideo_dir;
    bool tokenKey_bool;
    bool sourceKey_bool;

    QLabel * mainState_statusLabel;
    QLabel * searchState_statusLabel;
    QLabel * gatherState_statusLabel;
    IdPane * _idPane;
    IOWidget * gather_iow;
    IOWidget * search_iow;
    QStackedLayout * _stack;
    QBoxLayout * iow_layout;
    LivePage * livePage_wgt;
    MaintenancePage * maintPage_wgt;
};

#endif // MAINWINDOW_H
