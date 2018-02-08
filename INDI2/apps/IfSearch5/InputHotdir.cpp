/** @file InputHotdir.cpp
*	Definition of ImageSource class for DDT image library
*
*	@author	Dynamic DeZigns Technology
*
*/
#include <InputHotdir.h>

#include <QFileInfo>
#include <QList>
#include <QImageReader>
#include <QTimer>

#include <ImageCache.h>
#include <InfoMacros.h>



QStringList InputHotdir::defaultFilters;

InputHotdir::InputHotdir(QObject * parent)
    : QObject(parent), cache(0), dirRead(0), watcher(0)
    , timer(0), status(0), dirChanged(false), started(false)
{
    if (defaultFilters.isEmpty())
    {
        QList<QByteArray> lba = QImageReader::supportedImageFormats();
        foreach(QByteArray ba, lba)
            defaultFilters << "*." + ba;
    }

    SampleMsec = 0;
    MaxCache = 0;
    Jump = Skip = 0;
    NewOnly = false;
    OrderBy = 9;
    DeleteAfter = false;
    Loop = false;
    nameFilters = defaultFilters;
} // c'tor

InputHotdir::~InputHotdir()
{
    reset();
} // d'tor

void InputHotdir::reset(void)
{
    if (timer)		{	timer->stop();	delete timer; timer = 0; }
    if (dirRead)	{	delete dirRead;		dirRead = 0; }
    if (watcher)	{	delete watcher;		watcher = 0; }
    processed.clear();
    pending.clear();
} // reset()

QString InputHotdir::process(const QString & pathName)
{
    QString err;

    reset();
    dirRead = new QDir;

    if ( ! dirRead->exists(pathName))
        err = tr("%1 does not exist").arg(pathName);
    else if ( ! dirRead->cd(pathName))
        err = tr("Unable to change to %1").arg(pathName);
    else if ( ! dirRead->isReadable())
        err = tr("%1 is not readable").arg(dirRead->path());
    if ( ! err.isEmpty())
    {
        delete dirRead;
        dirRead = 0;
        return err;
    }

    if (NewOnly)
        processed = getCurrent();

    processDirChange();

    return QString();
} // process()

QString InputHotdir::start(const QUrl & url)
{
    QString dirName = url.path().mid(1);
    dirName.replace('|', ':');
    QString err = process(dirName);
    if ( ! err.isEmpty())
        return err;

    watcher = new QFileSystemWatcher(this);
    connect(watcher, SIGNAL(directoryChanged(QString)),
            this, SLOT(directoryChanged(QString)));
    watcher->addPath(dirRead->path());
    started = true;

    timer = new QTimer(this); NULLPTR(timer);
    timer->start(SampleMsec ? SampleMsec : 1000);
    connect(timer, SIGNAL(timeout()), this, SLOT(sample()));

    return QString();
} // start()


void InputHotdir::stop(void)
{
    if (timer)
    {
        timer->stop();
        delete timer;
        timer = 0;
    }

    if (watcher)
    {
        delete watcher;
        watcher = 0;
    }

    started = false;
} // stop()

void InputHotdir::sample(void)
{
    if (pending.isEmpty() && dirChanged)
        processDirChange();
    if (pending.isEmpty() && loop())
    {
        pending = processed;
        processed.clear();
    }
    if ( ! pending.isEmpty()
        && (0 == cache
            || 0 == MaxCache
            || cache->grabSize() < MaxCache))
        emit ready();
} // sample()

QString InputHotdir::takeFirst(void)
{
    int skipping = Skip;
    while (skipping-- && ! pending.isEmpty())
        processed.append(pending.takeFirst());
    if (pending.isEmpty())
    {
        return QString();
    }
    else
    {
        QString fileName = pending.takeFirst();
        processed.append(fileName);
        if (pending.isEmpty())
        {
            processed.clear();
            emit empty();
        }
        return dirRead->filePath(fileName);
    }
} // takeFirst()

bool InputHotdir::cacheFirst(void)
{
    if ( ! cache)
    {
        ERRMSG("No cache for InputHotdir::cacheFirst()");
        return false;
    }

    if (MaxCache && MaxCache < cache->grabSize())
    {
        DETAIL("Hotdir cache at max");
        return false;
    }

    QString filename = takeFirst();
    if (filename.isEmpty())
    {
        DETAIL("Nothing for InputHotdir::cacheFirst()");
        return false;
    }

    QFileInfo fi(filename);
    QString grabId = imageId();
    if (grabId.isEmpty())
        grabId = fi.completeBaseName();
    else if (grabId.contains("{filetime}", Qt::CaseInsensitive))
    {
        QDateTime dt(fi.lastModified());
        grabId.replace("{filetime}", dt.toString("DyyyyMMdd-Thhmmsszzz"), Qt::CaseInsensitive);
    }

    grabId = cache->grabFile(filename, grabId);

    if ( ! MoveAfter.isEmpty())
        cache->deferredMove(grabId, MoveAfter);
    else if (DeleteAfter)
        cache->deferredMove(grabId, "~");
    cache->hold(grabId, this);
    emit grabCached(grabId);
    return true;
} // cacheFirst()

QStringList InputHotdir::getCurrent(void)
{
    if (dirRead)
        return dirRead->entryList(nameFilters,
                                  QDir::Files | QDir::Readable,
                                  QDir::SortFlags(orderBy()));
    else
        return QStringList();
} // getCurrent();


void InputHotdir::directoryChanged(QString path)
{
    (void)path;
    dirChanged = true;
    if (pending.isEmpty())
        processDirChange();
} // directoryChanged

void InputHotdir::processDirChange(void)
{
    QDateTime begin, end;
    if ( ! BeginDateTime.isEmpty())
    {
        QString s = BeginDateTime;
        qint64 i64 = BeginDateTime.toLongLong();
        if (i64 > 3000)
            begin = QDateTime::fromMSecsSinceEpoch(i64);
        else
            begin = QDateTime::fromString(s, Qt::ISODate);
        INFO("Start Time: %1 [%2] %3", begin.toString(Qt::ISODate),
             begin.toMSecsSinceEpoch(), s);
    }
    if ( ! EndDateTime.isEmpty())
    {
        QString s = EndDateTime;
        qint64 i64 = EndDateTime.toLongLong();
        if (i64 > 3000)
            end = QDateTime::fromMSecsSinceEpoch(i64);
        else
            end = QDateTime::fromString(s, Qt::ISODate);
        INFO("End Time: %1 [%2] %3", end.toString(Qt::ISODate),
             end.toMSecsSinceEpoch(), s);
    }
    bool checkTime = dirRead && (begin.isValid() || end.isValid());
    INFO("Date/Time checking %1", checkTime);
    bool wasEmpty = pending.isEmpty();
    QStringList current = getCurrent();
    int jumped = Jump;

    foreach(QString fn, processed)
        if ( ! current.contains(fn))
            processed.removeAll(fn);
    foreach(QString fn, current)
        if (processed.contains(fn))
            current.removeAll(fn);
    foreach(QString fn, current)
        if ( ! pending.contains(fn))
        {
            if (Jump && jumped)
            {
                DETAIL("Jumped %1 over %2", jumped, fn);
                --jumped;
                continue;
            }
            if (checkTime)
            {
                QDateTime modTime = QFileInfo(dirRead->absoluteFilePath(fn))
                                              .lastModified();
                bool skip = false;
                if (begin.isValid() && modTime < begin)
                    skip = true;
                else if (end.isValid() && modTime > end)
                    skip = true;
                if (skip)
                    DETAIL("Skipping %1 modified %2", fn, modTime.toString("yyyyMMddThhmmsszzz"));
                else
                {
                    DETAIL("QUEUEING %1 modified %2", fn, modTime.toString("yyyyMMddThhmmsszzz"));
                    pending.append(fn);
                }
            }
            else
            {
                DETAIL("QUEUEING %1 unconditional", fn);
                pending.append(fn);
            }
        }
    if (wasEmpty && ! pending.isEmpty())
        emit notEmpty();

    dirChanged = false;
} // processDirChange


void InputHotdir::setSampleMsec(int v)
{
    SampleMsec = v;

    if ( ! started)
        return;

    if (timer && SampleMsec)
        timer->setInterval(SampleMsec);
    else if ( ! timer && SampleMsec)
    {
        timer = new QTimer(this);
        timer->start(SampleMsec);
        connect(timer, SIGNAL(timeout()), this, SLOT(sample()));
    }
    else if (timer && ! SampleMsec)
    {
        timer->stop();
        delete timer;
        timer = 0;
    }
}
