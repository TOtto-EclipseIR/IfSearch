#include "FrameSourceInstance.h"

#include "../eirExe/Log.h"

#include "HttpBuiltin.h"
#include "LiveDirBuiltin.h"
#include "StaticDirsBuiltin.h"


FrameSourceInstance::FrameSourceInstance(void)
    : _plugin(0)
{
    FUNCTION();
}

FrameSourceInstance::FrameSourceInstance(
                    const int type,
                    const QString & metaName,
                    const QFileInfo & fi,
                    FrameSourcePlugin * fsp)
    : metaType_i(type)
    , metaClassName_s(metaName)
    , plugin_file(fi)
    , _plugin(fsp)
    , _behavior(fsp->behavior())
{
    FUNCTION((int)type, metaName,
             fi.absoluteFilePath(),
             OBJNAME(fsp));
}

int FrameSourceInstance::type(void) const
{
    return metaType_i;
}

QString FrameSourceInstance::name(void) const
{
    return metaClassName_s;
}

QFileInfo FrameSourceInstance::file(void) const
{
    return plugin_file;
}

FrameSourcePlugin * FrameSourceInstance::plugin(void) const
{
    return _plugin;
}

FrameSourcePlugin * FrameSourceInstance::instance(QObject * parent)
{
    FUNCTION(OBJNAME(parent));
    if (plugin_file.isFile())
    {
        TODO("load from plugin file");
    }
    else
    {
        FrameSourcePlugin * fsp = new FrameSourcePlugin(parent);
        OBJPOINTER(FrameSourcePlugin, fsp);
        fsp->setBehavior(_behavior);
        FNRETURN(name());
        return fsp;
    }

    FNRETURN(0);
    return 0;
}
