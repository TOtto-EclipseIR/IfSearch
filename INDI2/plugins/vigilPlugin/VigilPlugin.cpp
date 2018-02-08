#include "VigilPlugin.h"

#include <QtPlugin>

#include "version.h"
#include "../../common/version.h"

#include <eirExe/Log.h>
#include <eirFrameSource/FrameSourceBehavior.h>
#include <eirFrameSource/FrameSourceMachine.h>
#include <eirFrameSource/FrameSourceState.h>

#include "VigilBehavior.h"

FrameSourcePlugin::FrameSourcePlugin(QObject * parent)
    : QObject(parent)
    , _behavior(0)
    , _machine(0)
{
    FUNCTION(OBJNAME(parent));
    setBehavior(new VigilBehavior);
}

FrameSourcePlugin::~FrameSourcePlugin()
{
    FUNCTION();
    TRACE(OBJNAME(this));
    config_vars = VariableSet();
    if (_behavior) delete _behavior;
    _behavior = 0;
}


#include "../../EIRlibs/eirFrameSource/FrameSourcePlugin-common.cpp"

Q_EXPORT_PLUGIN2(vigil, FrameSourcePlugin);
