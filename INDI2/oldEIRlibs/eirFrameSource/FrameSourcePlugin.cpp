#include "FrameSourcePlugin.h"

#include <QtCore/QUrl>

#include "../eirBase/Severity.h"
#include "../eirExe/Results.h"
#include "../eirExe/Log.h"

#include "FrameSourceBehavior.h"
#include "FrameSourceMachine.h"
#include "FrameSourceState.h"

FrameSourcePlugin::FrameSourcePlugin(QObject *parent)
    : QObject(parent)
    , _behavior(0)
    , _machine(0)
{
    FUNCTION(OBJNAME(parent));
    setObjectName("FrameSourcePlugin");
}

FrameSourcePlugin::~FrameSourcePlugin()
{
    FUNCTION();
    TRACE(OBJNAME(this));
    config_vars = VariableSet();
    if (_behavior) delete _behavior;
    _behavior = 0;
}


