#include "VigilDvr.h"

#include "../../EIRlibs/eirCore/VariableSet.h"
#include "../../EIRlibs/eirExe/Log.h"

#include "VigilResult.h"

VigilDvr::VigilDvr(QObject *parent)
    : QObject(parent)
    , _dvr(new VigilSDK::CVigilDVR(this))
{
    FUNCTION();
    setObjectName("VigilDvr");
    if (_dvr->isNull())
    {
        error_s = "Unable to create CVigilDVR";
    }
}

VigilDvr::~VigilDvr()
{
    FUNCTION();
    // if _dvr
    //      if logged in, logout
    //      delete _dvr
}

bool VigilDvr::login(const QUrl & url)
{
    FUNCTION();
    if (isError()) return false;

    QString scheme(url.scheme());
    QString host(url.host());
    QString userName(url.userName());
    QString password(url.password());
    int port = url.port();
    if ( ! port) port = 22801;

    _dvr->SetRemoteHost(host);
    _dvr->SetRemotePort(port);
    VigilResult login_result(_dvr->Login(userName, password));

    if (login_result.isOk())
    {
        serverName_s = login_result.siteName();
        emit loggedin(true);
    }
    else
    {
        error_s = "Login Failed: " + url.toString();
        ERROR(error_s);
        emit loggedin(false);
        return false;
    }
    login_result.loginResult().dump();

    int nChannel = login_result.cameraCount();
    if ( ! nChannel)
    {
        error_s = "No Cameras on " + url.host();
        ERROR(error_s);
        emit loggedin(false);
        return false;
    }

    VigilResult channel_result(_dvr->GetCameraNames());
    for (int x = 0; x < nChannel; ++x)
    {
        QString s(channel_result.cameraName(x));
        channelName_map.insert(x, s);
        INFO("Channel %1 = [%2]", x, s);
    }
    return true;
}

void VigilDvr::updateInfo(void)
{

}

bool VigilDvr::isError(void) const
{
    return ( ! error_s.isEmpty()) || ( ! _dvr) || _dvr->isNull();
}

QString VigilDvr::errorString(void) const
{
    return error_s;
}

bool VigilDvr::isLoggedIn(void) const
{
    if (isError())  return false;
    return _dvr->LoggedOn();
}


VigilCameraInfoList VigilDvr::cameraList(void)
{
    return _vcil;
}

QString VigilDvr::serverName(void)
{
    return serverName_s;
}

QString VigilDvr::channelName(const int channelIndex)
{
    return _vcil.channelName(channelIndex);
}

VigilSDK::_CVigilLiveFeed * VigilDvr::createLiveFeed(void)
{
    return _dvr->CreateLiveFeedObject();
}

VigilSDK::_CVigilPlayback * VigilDvr::createPlayback(void)
{
    return _dvr->CreatePlaybackObject();
}
