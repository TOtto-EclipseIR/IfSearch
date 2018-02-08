#include "LiveDirBehavior.h"

#include <QtCore/QString>
#include <QtCore/QUrl>

#include "../eirExe/Log.h"
#include "../eirExe/Result.h"
#include "../eirExe/ResultId.h"
#include "../eirExe/Results.h"

#include "FrameSourcePlugin.h"

LiveDirBehavior::LiveDirBehavior(void)
{
}

FrameSourceBehavior * LiveDirBehavior::brother(void)
{
    return new LiveDirBehavior();
}

QString LiveDirBehavior::name(void) const
{
    return "LiveDirBehavior";
}

QStringList LiveDirBehavior::schemeList(void)
{
    return QStringList() << "ldir";
}

bool LiveDirBehavior::supports(const QUrl & url)
{
    return "ldir" == url.scheme();
}

bool LiveDirBehavior::supportsScheme(const QString & scheme)
{
    return "ldir" == scheme;
}

bool LiveDirBehavior::canGrab(void)
{
    return ! filename_list.isEmpty();
}

void LiveDirBehavior::enterConnecting(void)
{
    FUNCTION();
    FSPPOINTER(FrameSourcePlugin, _plugin);
    ResultSet rset;

    base_dir.setPath(_plugin->url().path());
    if (base_dir.exists())
    {
        if ( ! _plugin->url().hasQueryItem("NoDelete"))
            base_dir.removeAll(QDir::Files);
    }
    else
    {
        if ( ! _plugin->url().hasQueryItem("NoCreate"))
            base_dir.mkpath(".");
    }

    if (base_dir.exists())
    {

    }
    else
    {
        rset << Result(ResultId("eir", "FrameSource", "LiveDir", "ConnectFailure"),
                      base_dir.path());
        _plugin->emitConnectError(rset);
    }

}
