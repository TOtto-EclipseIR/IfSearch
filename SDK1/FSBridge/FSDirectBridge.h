#ifndef FSDIRECTBRIDGE_H
#define FSDIRECTBRIDGE_H
#include "FSBridge.h"

#include <QObject>

#include <QtCore/QStringList>

#include <eirFile/QQDir.h>

#include "../IfSearch/QProperty.h"

class QTimer;

class FrameSourceManager;
class FrameSourceDirect;

namespace DDT
{
    class ImageCache;
}

#define FSDIRECTBRIDGE_QPROPERTIES(TVND) \
    TVND(int, Channel,   1) \
    TVND(int, SampleMsec,   0) \
    TVND(QString, Url, QString()) \
    TVND(QString, Begin, QString()) \
    TVND(QString, End, QString()) \
    TVND(int, MaxCache, 0) \
    TVND(bool, Paused, false) \
    TVND(QString, LogFileName, QString()) \


class FSBRIDGESHARED_EXPORT FSDirectBridge
        : public QObject
{
    Q_OBJECT
    DECLARE_QPROPERTIES(FSDIRECTBRIDGE_QPROPERTIES);

public:
    explicit FSDirectBridge(DDT::ImageCache * parent=0);
    QString initialize(const QQDir &pluginDir=QQDir());
    QString start(void);
    QString stop(void);
    QString pause(void);
    QString resume(void);

    QStringList schemes(void);

signals:
    void grabCached(QString imageId);
    
public slots:
    void takeGrab(int key);
    void checkResume(void);

private:
    DDT::ImageCache * _cache;
    FrameSourceManager * _manager;
    FrameSourceDirect * _interface;
    QTimer * resume_timer;
};

#endif // FSDIRECTBRIDGE_H
