#ifndef VIGILPLUGIN_H
#define VIGILPLUGIN_H
#include "vigil_global.h"

#include <QObject>
#include <eirFrameSource/FrameSourceInterface.h>

#include <QtPlugin>

#include <QtCore/QList>
#include <QtCore/QStringList>
#include <QtXml/QDomElement>

#include <eirTypes/MillisecondTime.h>
#include <eirCore/VariableSet.h>
#include <eirExe/Results.h>
#include <eirImage/ImageEntity.h>

class HttpManager;
class FrameSourceBehavior;
class FrameSourceMachine;
class FrameSourceState;
class FrameSourceStatusIndex;

#define FSPPOINTER(typ, ptr) { OBJPOINTER(QObject, ptr); \
                               EXPECT(ptr->objectName().endsWith("FrameSourcePlugin")); \
                               TODO("Why wont FrameSourcePlugin qobject_cast?"); }

class VIGILSHARED_EXPORT FrameSourcePlugin
        : public QObject
        , public FrameSourceInterface
{
    Q_OBJECT
    Q_INTERFACES(FrameSourceInterface)

public:
    FrameSourcePlugin(QObject * parent=0);
    ~FrameSourcePlugin();

#include <eirFrameSource/FrameSourcePlugin-common.h>
};

#endif // VIGILPLUGIN_H
