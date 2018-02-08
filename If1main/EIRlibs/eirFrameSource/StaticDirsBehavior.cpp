#include "StaticDirsBehavior.h"

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QUrl>

#include "../eirExe/ExeSupport.h"

#include "FrameSourcePlugin.h"
#include "FrameSourceResult.h"

StaticDirsBehavior::StaticDirsBehavior(void)
{
}

FrameSourceBehavior * StaticDirsBehavior::brother(void)
{
    return new StaticDirsBehavior();
}

QString StaticDirsBehavior::name(void) const
{
    return "StaticDirs";
}

QStringList StaticDirsBehavior::schemeList(void)
{
    return QStringList() << "dirs";
}

bool StaticDirsBehavior::supports(const QUrl & url)
{
    return "dirs" == url.scheme();
}

bool StaticDirsBehavior::supportsScheme(const QString & scheme)
{
    return "dirs" == scheme;
}

bool StaticDirsBehavior::canGrab(void)
{
    return ! sort_fi_mmap.isEmpty();
}

void StaticDirsBehavior::enterConstruct(void)
{
    FUNCTION();
    FSPPOINTER(FrameSourcePlugin, _plugin);
    ResultSet rset;
    rset << Result(FrameSourceResult::Initialized, name());
    _plugin->emitInitialized(rset);
}

void StaticDirsBehavior::enterConnecting(void)
{
    FUNCTION();
    FSPPOINTER(FrameSourcePlugin, _plugin);

    QUrl url(_plugin->url());
    EXPECT(url.host().isEmpty());
    QString path(url.path().mid(1));
    DUMPVAR(path);
    base_dir.setPath(path);

    ResultSet rset;
    if (EXPECT(base_dir.exists()))
    {
        rset.add(Result(FrameSourceResult::Connected,
                       _plugin->url().toString()));
        _plugin->emitConnected(rset);
    }
    else
    {
        rset.add(Result(FrameSourceResult::ConnectError,
                       _plugin->url()));
        _plugin->emitConnectError(rset);
    }
}

void StaticDirsBehavior::enterConfiguring(void)
{
    FUNCTION();
    FSPPOINTER(FrameSourcePlugin, _plugin);

    VariableSet config = _plugin->configuration();
    DUMPVSET(config);

    TODO("Collect configuration: Resolution");
    _plugin->emitConfigured(ResultSet());
}

void StaticDirsBehavior::enterStarted(void)
{
    FUNCTION();
    FSPPOINTER(FrameSourcePlugin, _plugin);
/*
    QStringList dirnames(base_dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot));
    foreach (QString dirname, dirnames)
        processDir(dirname);
*/
    processDir(".");

    _plugin->emitStarted(ResultSet());
}

void StaticDirsBehavior::processDir(const QString & dirname,
                                    const int depth)
{
    FUNCTION(dirname, depth);
    if (0 == depth) return;

    QQDir dir(base_dir);
    if ( ! EXPECT(dir.cd(dirname))) return;
    DUMPVAR(dir.path());

    QStringList subdirnames(dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot));
    DUMPVAR(subdirnames.join(";"));
    foreach (QString subdirname, subdirnames)
        processDir(dir.filePath(subdirname), depth-1);

    QStringList filenames(dir.entryList(ExeSupport::imageFileNameFilters(),
                                        QDir::Files));
    DUMPVAR(filenames.join(";"));
    foreach (QString filename, filenames)
    {
        QFileInfo fi(dir.filePath(filename));
        sort_fi_mmap.insertMulti(sort(fi), fi);
        TRACE("Adding %1", fi.canonicalFilePath());
    }
}

QString StaticDirsBehavior::sort(const QFileInfo fi) const
{
    return fi.canonicalFilePath().toLower();
}

void StaticDirsBehavior::enterGrabbing(void)
{
    FUNCTION();
    FSPPOINTER(FrameSourcePlugin, _plugin);
    ResultSet rset;

    if (sort_fi_mmap.isEmpty())
    {
        rset << Result(FrameSourceResult::PlaybackComplete,
                       0,
                       _plugin->url(),
                       QString(),
                       QString());
        _plugin->emitComplete(rset);
    }
    else
    {
        QMultiMap<QString, QFileInfo>::iterator it;
        it = sort_fi_mmap.begin();
        QString key(it.key());
        QFileInfo fi(it.value());
        DUMPVAR(key);
        DUMPVAR(fi.fileName());
        TRACE("Removing %1,%2", key,
              fi.canonicalFilePath());
        sort_fi_mmap.remove(key, fi);

        TRACE("Opening %1", fi.canonicalFilePath());
        QFile file(fi.canonicalFilePath());
        if ( ! EXPECT(file.open(QIODevice::ReadOnly))) return;
        ImageEntity ie(ImageEntity::fromByteArray(file.readAll()));
        _plugin->enqueueGrab(ie);
        _plugin->emitGrabbed(rset);
    }
}
