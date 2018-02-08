#include "MainWindow.h"

#include <QtSvg>
#include <QtCore/QProcess>
#include <QtCore/QSettings>
#include <QtCore/QTimer>
#include <QtGui/QApplication>
#include <QtGui/QBoxLayout>
#include <QtGui/QDesktopWidget>
#include <QtGui/QKeyEvent>
#include <QtGui/QLabel>
#include <QtGui/QStackedLayout>
#include <QtGui/QStatusBar>

#include <eirBase/eirBase.h>
#include <eirBase/VersionInfo.h>
#include <eirTypes/QQSizeF.h>
#include <eirCore/eirCore.h>
//#include <eirCore/SettingsVariableGroup.h>
#include <eirCore/VariableTable.h>
#include <eirExe/Log.h>
#include <eirFile/TemporaryDir.h>
#include <eirGUI/IOWidget.h>
#include <eirGUI/TimeoutBar.h>
#include <eirSDK1/ImageDirSink.h>
#include <eirSDK1/PersonReportRow.h>
#include <eirSDK1/SdkClient.h>
#include <eirSDK1/SdkCommand.h>
#include <eirSDK1/SdkClientParameters.h>
#include <eirSDK1/SdkMachine.h>

#include "IdPane.h"
#include "LivePage.h"
#include "MaintenancePage.h"
#include "StateMachine.h"

const QString MainWindow::tokenPrefix("TokenAuth-");

MainWindow::MainWindow(QWidget *parent)
    : GuiApplication(parent)
    , _machine(new StateMachine(this))
    , gather_sdk(new SdkClient(&gather_parms, this))
    , search_sdk(new SdkClient(&search_parms, this))
    , markedFace_dir(0)
    , marked_dir(0)
    , tokenKey_bool(false)
    , sourceKey_bool(false)
    , mainState_statusLabel(new QLabel(this))
    , searchState_statusLabel(new QLabel(this))
    , gatherState_statusLabel(new QLabel(this))
    , _idPane(new IdPane(this))
    , gather_iow(new IOWidget(gather_sdk->process()))
    , search_iow(new IOWidget(search_sdk->process()))
    , _stack(new QStackedLayout)
    , iow_layout(0)
    , livePage_wgt(new LivePage(this))
    , maintPage_wgt(new MaintenancePage(this))
{
    FUNCTION();
    OBJPOINTER(SdkClient, gather_sdk);
    OBJPOINTER(SdkClient, search_sdk);
    setObjectName("TokenAuth:MainWindow");
    gather_sdk->setObjectName("gather_sdk");
    search_sdk->setObjectName("search_sdk");
    QTimer::singleShot(10, this, SLOT(start()));
}

LivePage * MainWindow::live(void) const
{
    OBJPOINTER(LivePage, livePage_wgt);
    return livePage_wgt;
}

MaintenancePage * MainWindow::maint(void) const
{
    OBJPOINTER(MaintenancePage, maintPage_wgt);
    return maintPage_wgt;
}

SdkClient * MainWindow::gatherSdk(void) const
{
    OBJPOINTER(SdkClient, gather_sdk);
    return gather_sdk;
}

SdkClient * MainWindow::searchSdk(void) const
{
    OBJPOINTER(SdkClient, search_sdk);
    return search_sdk;
}

IdPane * MainWindow::idPane(void) const
{
    OBJPOINTER(IdPane, _idPane);
    return _idPane;
}

ImageDirSink * MainWindow::markedFaceDirSink(void) const
{
    OBJPOINTER(ImageDirSink, markedFace_dir);
    return markedFace_dir;
}

QString MainWindow::authenticatePersonId(void) const
{
    return authenticatePersonId_;
}

QString MainWindow::authenticateToken(void) const
{
    return authenticateToken_;
}


void MainWindow::setPage(QWidget * page)
{
    OBJPOINTER(QWidget, page);
    OBJPOINTER(QStackedLayout, _stack);
    _stack->setCurrentWidget(page);
}

QChar MainWindow::tokenChar(const QString & s)
{
    if (s.startsWith(tokenPrefix)
            && s.length() > tokenPrefix.length())
        return QChar(s.at(tokenPrefix.length()));
    else
        return QChar();
}

QString MainWindow::tokenString(const QChar c)
{
    FUNCTION(c);
    QChar u(c.toUpper());
    if (u >= QChar('A') && u <= QChar('Z'))
    {
        FNRETURN(tokenPrefix + u);
        return tokenPrefix + u;
    }
    else
        return QString();
}

void MainWindow::setTokenKeyEnable(const bool enable)
{
    FUNCTION(enable);
    tokenKey_bool = enable;
}

void MainWindow::setSourceKeyEnable(const bool enable)
{
    FUNCTION(enable);
    sourceKey_bool = enable;
}


void MainWindow::keyPressEvent(QKeyEvent * e)
{
    FUNCTION(e->key());
    int key = e->key();
    if (key >= Qt::Key_A && key <= Qt::Key_Z)
    {
        QChar c(key);
        Qt::KeyboardModifiers m = e->modifiers();
        QString s(tokenString(c));
        if (token_personId_map.contains(s))
        {
            QString pId(token_personId_map.value(s));
            if (Qt::NoModifier == m && tokenKey_bool)
            {
                TRACE("emit tokenKey() %1", s);
                emit keyToken(s);
                return;
            }
            else if (Qt::ShiftModifier & m && sourceKey_bool)
            {
                TRACE("emit shiftPersonId() %1", s);
                emit shiftPersonId(pId);
                return;
            }
            else if (Qt::AltModifier & m && sourceKey_bool)
            {
                TRACE("emit altPersonId() %1", s);
                emit altPersonId(pId);
                return;
            }
        }
    }
    QMainWindow::keyPressEvent(e);
}

void MainWindow::resizeEvent(QResizeEvent * e)
{
    FUNCTION(e->size());
    QQSizeF sz(e->size());
    if (iow_layout)
        iow_layout->setDirection((sz.width() > 1000 || sz.height() < 800)
                                ? QBoxLayout::LeftToRight
                                : QBoxLayout::TopToBottom);
}

void MainWindow::personReportResult(SdkCommand result)
{
    FUNCTION(result.key());
    OBJPOINTER(SdkClient, search_sdk);
    DISCONNECT(search_sdk, SIGNAL(retrieveResult(SdkCommand)),
            this, SLOT(personReportResult(SdkCommand)));
    CONNECT(search_sdk, SIGNAL(gotPersonReport()),
            this, SLOT(readPersonReport()));
    if ( ! search_sdk->getPersonReport(result))
    {
        _idPane->error("Error retrieving PersonReport");
        QTimer::singleShot(10, this, SLOT(enterError()));
        return;
    }
}

void MainWindow::readPersonReport(void)
{
    FUNCTION();
    OBJPOINTER(SdkClient, search_sdk);
    token_personId_map.clear();
    personId_token_map.clear();

    VariableTable * vt = search_sdk->personReport();
    PersonReportRow prr;
    int n = vt->rowCount();
    for (int r = 0; r < n; ++r)
    {
        vt->readRow(&prr, r);
        prr.dump();
        QString token(prr.getDescription());
        QString personId = prr.getPersonId();
        addToken(token, personId);
    }

    TRACE("emit gotPersonReport()")
    emit gotPersonReport();
}

void MainWindow::checkToken(QString token)
{
    FUNCTION(token);
    if (token_personId_map.contains(token))
    {
        authenticateToken_ = token;
        authenticatePersonId_ = token_personId_map.value(token);
        TRACE("emit gotToken()");
        emit gotToken();
    }
    else
    {
        _idPane->error(tr("Token %1 not enrolled").arg(token));
        _idPane->resetIn(5000);
    }
}

QString MainWindow::tokenFor(const QString & personId)
{
    FUNCTION(personId);
    FNRETURN(personId_token_map.value(personId));
    return personId_token_map.value(personId);
}

void MainWindow::addToken(const QString & token,
                          const QString & personId)
{
    FUNCTION(token, personId);
    token_personId_map.insert(token, personId);
    personId_token_map.insert(personId, token);
}

void MainWindow::personVideoKey(QString personId)
{
    FUNCTION(personId);
    gatherSdk()->interface()->write("Input/URL", QString());

    personVideo_dir.setPath("../TPIRdata/people");
    personVideo_dir.cd(personId);
    QStringList videoDirs(personVideo_dir.entryList(QStringList() << "??",
                                               QDir::Dirs | QDir::NoDotAndDotDot));
    if ( ! EXPECTNOT(videoDirs.isEmpty()))
    {
        int x = qrand() % videoDirs.size();
        personVideo_dir.cd(videoDirs.at(x));
        statusBar()->showMessage("Play: " + personVideo_dir.path(), 10000);
        QTimer::singleShot(10, this, SLOT(startPersonVideo()));
    }
}

void MainWindow::startPersonVideo(void)
{
    FUNCTION();
    gather_sdk->startUrl("dir:///" + personVideo_dir.absolutePath());
}

void MainWindow::enterShutdown(void)
{
    FUNCTION();
    emit stateName("Shutdown");
    livePage_wgt->toolBar()->setEnabled(true);
    livePage_wgt->quitAction()->setEnabled(true);
    gather_sdk->shutdown();
    search_sdk->shutdown();
}

void MainWindow::enterError(void)
{
    FUNCTION();
    emit stateName("Error");

    if ( ! gather_sdk->errorString().isEmpty())
        _idPane->error(gather_sdk->errorString());
    else if ( ! search_sdk->errorString().isEmpty())
        _idPane->error(search_sdk->errorString());
    else
        _idPane->error("Error");
}

