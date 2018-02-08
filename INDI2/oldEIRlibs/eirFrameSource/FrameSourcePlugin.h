#ifndef FRAMESOURCEPLUGINBASE_H
#define FRAMESOURCEPLUGINBASE_H
#include "eirFrameSource.h"

#include <QObject>
#include "FrameSourceInterface.h"

#include <QtCore/QList>
#include <QtCore/QStringList>
#include <QtXml/QDomElement>

#include "../eirTypes/MillisecondTime.h"
#include "../eirCore/VariableSet.h"
#include "../eirExe/Log.h"
#include "../eirExe/Results.h"
#include "../eirImage/ImageEntity.h"

class HttpManager;
class FrameSourceBehavior;
class FrameSourceMachine;
class FrameSourceState;
class FrameSourceStatusIndex;

#define FSPPOINTER(typ, ptr) { OBJPOINTER(QObject, ptr); \
                               EXPECT(ptr->objectName().endsWith("FrameSourcePlugin")); \
                               TODO("Why wont FrameSourcePlugin qobject_cast?"); }

class EIRFRAMESOURCESHARED_EXPORT FrameSourcePlugin
        : public QObject, public FrameSourceInterface
{
    Q_OBJECT
    Q_INTERFACES(FrameSourceInterface)

public:
    FrameSourcePlugin(QObject * parent=0);
    ~FrameSourcePlugin();

#include "FrameSourcePlugin-common.h"
};

#endif // FRAMESOURCEPLUGINBASE_H
