#include "bhttpPlugin.h"
#include <QtPlugin>

#include <eirExe/Log.h>
#include <eirFrameSource/FrameSourceBehavior.h>
#include <eirFrameSource/FrameSourceMachine.h>
#include <eirFrameSource/FrameSourceState.h>

#include "bhttpBehavior.h"

FrameSourcePlugin::FrameSourcePlugin(QObject * parent)
    : QObject(parent)
    , _behavior(0)
    , _machine(0)
{
    FUNCTION(OBJNAME(parent));
    setBehavior(new bhttpBehavior);
}

Q_EXPORT_PLUGIN2(bhttp, FrameSourcePlugin);
