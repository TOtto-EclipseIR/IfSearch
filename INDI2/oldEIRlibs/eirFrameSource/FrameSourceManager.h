#ifndef FRAMESOURCEMANAGER_H
#define FRAMESOURCEMANAGER_H
#include "eirFrameSource.h"

#include <QtCore/QObject>

#include <QtCore/QMultiMap>
#include <QtCore/QString>

#include "../eirFile/QQDir.h"

class QPluginLoader;
class QUrl;

class ResultSet;

class FrameSourceBehavior;
class FrameSourcePlugin;

class EIRFRAMESOURCESHARED_EXPORT FrameSourceManager : public QObject
{
    Q_OBJECT

public:
    explicit FrameSourceManager(QObject * parent=0);
    ResultSet initialize(QQDir pluginDir=QQDir());
    void reset(void);
    FrameSourcePlugin * plugin(const QUrl & url,
                               QObject * parent=0);
    QStringList schemes(void) const;

private:

private:
    QPluginLoader * _loader;
    QMultiMap <QString, QString> scheme_pluginFilename_map;
    QList<FrameSourceBehavior *> builtin_behavior_list;
};

#endif // FRAMESOURCEMANAGER_H
