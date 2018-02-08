#ifndef VIGILDVR_H
#define VIGILDVR_H
#include "ImVigil.h"

#include <QtCore/QObject>
#include <QtCore/QUrl>

#include "../../EIRlibs/eirBase/Named.h"
#include "VigilCameraInfoList.h"

class IMVIGILSHARED_EXPORT VigilDvr : public QObject, ImVigil
{
    Q_OBJECT
public:
    explicit VigilDvr(QObject * parent=0);
    ~VigilDvr();
    bool isError(void) const;
    QString errorString(void) const;
    bool login(const QUrl & url);
    bool isLoggedIn(void) const;
    VigilCameraInfoList cameraList(void);
    QString serverName(void);
    QString channelName(const int channelIndex);
    VigilSDK::_CVigilLiveFeed * createLiveFeed(void);
    VigilSDK::_CVigilPlayback * createPlayback(void);

signals:
    void loggedin(bool success);
    void updatedInfo(void);

public slots:
    void updateInfo(void);

private:
    VigilSDK::CVigilDVR * _dvr;
    QString error_s;
    QString serverName_s;
    Named<int> channelName_map;
    VigilCameraInfoList _vcil;
};

#endif // VIGILDVR_H
