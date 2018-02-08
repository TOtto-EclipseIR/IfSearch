#include "FrameSourceManager.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QPluginLoader>
#include <QtCore/QStringList>
#include <QtCore/QUrl>

#include "../eirExe/Log.h"
#include "../eirExe/Results.h"

#include "FrameSourcePlugin.h"

#include "HttpBehavior.h"
#include "LiveDirBehavior.h"
#include "StaticDirsBehavior.h"

FrameSourceManager::FrameSourceManager(QObject * parent)
    : QObject(parent)
    , _loader(new QPluginLoader(this))
{
    FUNCTION();
    OBJPOINTER(QPluginLoader, _loader);
}

ResultSet FrameSourceManager::initialize(QQDir pluginDir)
{
    FUNCTION(pluginDir.absolutePath());

    builtin_behavior_list << new HttpBehavior()
                          << new LiveDirBehavior()
                          << new StaticDirsBehavior();

    /* Iterate through library dir for framesource plugins" */
    if (pluginDir.isNull())
    {
        pluginDir = QDir::current();
        pluginDir.cd("framesource");
    }

    OBJPOINTER(QPluginLoader, _loader);
    QStringList filenames(pluginDir.entryList(QStringList() << "*.dll",
                                              QDir::Files));
    TRACE("%1 DLLs in %2", filenames.size(), pluginDir.absolutePath());

    foreach (QString filename, filenames)
    {
        QString afn(pluginDir.absoluteFilePath(filename));
        TRACE("Loading %1", afn);
        _loader->setFileName(afn);
        QObject * o = _loader->instance();
        //FrameSourcePlugin * p = qobject_cast<FrameSourcePlugin *>(o);
        if (o && o->objectName().endsWith("FrameSourcePlugin"))
        {
            FrameSourcePlugin * p = (FrameSourcePlugin *)o;
            QStringList schemes(p->schemeList());
            TRACE("Version %2 supports %1",
                  schemes.join("; "),
                  p->version().toString(true));
            foreach (QString scheme, schemes)
                scheme_pluginFilename_map.insert(scheme, afn);
        }
        else
            TRACE("Not a FrameSourcePlugin");
        _loader->unload();
    }

    return ResultSet();
}

void FrameSourceManager::reset(void)
{
    FUNCTION();
    scheme_pluginFilename_map.clear();
    builtin_behavior_list.clear();
}

FrameSourcePlugin * FrameSourceManager::plugin(const QUrl & url,
                                               QObject * parent)
{
    FUNCTION(url.toString(), OBJNAME(parent));
    FrameSourcePlugin * fsp = 0;

    /* Iterate through plugins */
    QStringList filenames = scheme_pluginFilename_map.values(url.scheme());
    foreach (QString filename, filenames)
    {
        _loader->setFileName(filename);
        QObject * o = _loader->instance();
        TRACE("plugin file %1 contains %2", filename, OBJNAME(o));
        //FrameSourcePlugin * p = qobject_cast<FrameSourcePlugin *>(o);
        if (o && o->objectName().endsWith("FrameSourcePlugin"))
        {
            fsp = (FrameSourcePlugin *)o;
            if (fsp && fsp->supports(url))
                fsp->startMachine();
        }
    }

    if ( ! fsp)
    {
        foreach (FrameSourceBehavior * fsb, builtin_behavior_list)
        {
            if (fsb->supports(url))
            {
                FNRETURN(fsb->name());
                fsp = new FrameSourcePlugin(parent);
                fsp->setBehavior(fsb->brother());
                fsp->startMachine();
            }
        }
    }

    FNRETURN(OBJNAME(fsp));
    return fsp;
}

QStringList FrameSourceManager::schemes(void) const
{
    return scheme_pluginFilename_map.keys();
}
