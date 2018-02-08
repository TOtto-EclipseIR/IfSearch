#include <FileWriter.h>
#include <FileWriteProfile.h>

#include <QTimer>

#include <InfoMacros.h>
#include <Return.h>
#include <Settings.h>

/* USAGE: (.h)
	StatusHandler  sts;
	ImageCache cache;
	Settings * appSettings;
	FileWriter * writer;
	FileWriteProfile * fwpCapture;
	FileWriteProfile * fwpFaceCache;
	...

(.cpp)
	appSettings = new Settings(...);
	writer = new FileWriter(appSettings, QString(), this);
	writer->setImageCache(&cache);
	writer->addLogFile(&sts)
	fwpCapture = writer->newProfile("Capture", FileWriter::CaptureImage);
	fwpFaceCache = writer->newProfile("FaceCache", FileWriter::Cache | FileWriter::FaceImage);
	...
	connect(appSettings, SIGNAL(changed(QString)), writer, SLOT(settingChanged(QString)));
	writer->start();

(later)
	fwpCapture->write(imageId);		// from ImageCache
	..
	QString faceId = ...
	QImage normImage = ...
	fwpCapture->write(normImage, faceId);
*/



FileWriter::FileWriter(Settings * settings, const QString & keyFormat, QObject * parent)
    : QObject(parent), _settings(settings)
{
    FUNCTION();
    setObjectName("FileWriter");
    _dateTime = QDateTime::currentDateTime();
    _settings->setValue("Output/BaseEms", _dateTime.toMSecsSinceEpoch());
    _imageCache = 0;
    _keyFormat = keyFormat.isEmpty() ? "Output/$Dir" : keyFormat;
    QString prefix = _keyFormat.left(1 + _keyFormat.lastIndexOf(QChar('/')));
    _optQuality = new Setting(settings, prefix + "Quality", -1, Settings::Volatile);
    _optFormat = new Setting(settings, prefix + "Format", "JPG", Settings::Volatile);
    _optFaceQuality = new Setting(settings, prefix + "FaceQuality", -1, Settings::Volatile);
    _optFaceFormat = new Setting(settings, prefix + "FaceFormat", "PNG", Settings::Volatile);
    _optBaseDir = new Setting(settings, prefix + "BaseDir", "../Output", Settings::Volatile);
    _optMaxCache = new Setting(settings, prefix + "MaxCache", 64, Settings::Volatile);
    Return::add(ReturnNoCache, "No Image Cache for %1", Warning);
    Return::add(ReturnNullImage, "Null Image for %2: %1", Warning);
}

FileWriter::~FileWriter()
{
    FUNCTION();
    pumpAll();
}

FileWriteProfile * FileWriter::newProfile(const QString & name, QString key)
{
    return newProfile(name, 0, key);
}

FileWriteProfile * FileWriter::newProfile(const QString & name, Flags f, QString key)
{
    FUNCTION();

    bool isCache = cacheDirNames_qsl.contains(name, Qt::CaseInsensitive);
    if (isCache) f |= Cache;
    FileWriteProfile * profile = new FileWriteProfile(name, f, this);
    if ( ! profile)
    {
        FNRETURN("Null new FileWriteProfile");
        return 0;
    }

    if (key.isEmpty())
    {
        key = _keyFormat;
        key.replace("$", name);
    }

    _mapKeyToProfile.insert(key, profile);
    profile->_opt = new Setting(_settings, key, QString(), Settings::Volatile);
    if (XmlText & profile->_flags) profile->_format = "xml";

    FNRETURN("success");
    return profile;
} // newProfile()

FileWriteProfile * FileWriter::profile(const QString & name)
{
    return _mapKeyToProfile.contains(name)
            ? _mapKeyToProfile.value(name) : 0;
}

void FileWriter::setCacheDirs(const QString & cacheDirs)
{
    cacheDirNames_qsl = cacheDirs.simplified().split(' ');
}


void FileWriter::start(int longMsec, int shortMsec, int cacheMsec)
{
    FUNCTION();
    _longMsec = longMsec, _shortMsec = shortMsec, _cacheMsec = cacheMsec;

    enqueue(0, _optQuality->keyName());
    enqueue(0, _optFormat->keyName());
    enqueue(0, _optFaceQuality->keyName());
    enqueue(0, _optFaceFormat->keyName());
    enqueue(0, _optMaxCache->keyName());
    enqueue(0, _optQuality->keyName());
    foreach (FileWriteProfile * prof, _mapKeyToProfile)
        enqueue(prof, QString());

    QTimer::singleShot(_shortMsec, this, SLOT(pulse()));
    QTimer::singleShot(_cacheMsec, this, SLOT(cacheClean()));
} // start()

QString FileWriter::timeStampString(void)
{
    if (_timeStampString.isEmpty())
        _timeStampString = _dateTime.toString("DyyyyMMdd-Thhmm");
    return _timeStampString;
} // timeStampString()

void FileWriter::pulse(void)
{
    // TODO: investigate worker QThread to service queue
//    FUNCTION();
    QTimer::singleShot(pumpFirst() ? _shortMsec : _longMsec, this, SLOT(pulse()));
} // pulse()


void FileWriter::settingChanged(QString key)
{
    FUNCTION();
    if (_optQuality->keyName() == key)
        enqueue(0, key);
    else if (_optFormat->keyName() == key)
        enqueue(0, key);
    else if (_optFaceQuality->keyName() == key)
        enqueue(0, key);
    else if (_optFaceFormat->keyName() == key)
        enqueue(0, key);
    else if (_optMaxCache->keyName() == key)
        enqueue(0, key);
    else if (_optBaseDir->keyName() == key)
        enqueue(0, key);
    else if (_mapKeyToProfile.contains(key))
        enqueue(_mapKeyToProfile.value(key), QString());
    pumpAll();
} // settingChanged()

void FileWriter::setImageCache(ImageCache * cache)
{
    FUNCTION();
    _imageCache = cache;
} // setImageCache()

bool FileWriter::isQueueValid(void)
{
//    FUNCTION();
    QReadLocker lock(&_queueLock);
    int nProfile	= _queueProfile.size();
    int nName		= _queueName.size();
    int nData		= _queueData.size();
    bool valid		= (nProfile == nName && nProfile == nData);
    if ( ! valid)
        emit error(tr("FileWriter Queue is not valid %1 %2 %3").arg(nProfile).arg(nName).arg(nData));
    //FNRETURN(valid);
    return valid;
} // isQueueValid()

bool FileWriter::isQueueEmpty(void)
{
//    FUNCTION();
    QReadLocker lock(&_queueLock);
    int nProfile	= _queueProfile.size();
    int nName		= _queueName.size();
    int nData		= _queueData.size();
    bool empty		= (0 == nProfile && 0 == nName && 0 == nData);
    //FNRETURN(empty);
    return empty;
} // isQueueEmpty()

void FileWriter::clearQueue(void)
{
//    FUNCTION();
    QWriteLocker lock(&_queueLock);
    _queueProfile.clear();
    _queueName.clear();
    _queueData.clear();
} // clearQueue()

void FileWriter::enqueue(FileWriteProfile * profile,
                         const QString & name,
                         const QByteArray & ba)
{
//    FUNCTION();
    if ( ! isQueueValid())
        clearQueue();
    QWriteLocker lock(&_queueLock);
    _queueProfile.enqueue(profile);
    _queueName.enqueue(name);
    _queueData.enqueue(ba);
    if (profile && ! name.isEmpty() && ! ba.isEmpty())
        profile->_filesPending.append(name);
} // enqueue()

bool FileWriter::dequeue(FileWriteProfile ** profile,
                         QString * name,
                         QByteArray * ba)
{
//    FUNCTION();
    if (isQueueEmpty())
    {
        //FNRETURN("empty");
        return false;
    }
    if ( ! isQueueValid())
    {
        clearQueue();
        //FNRETURN("invalid");
        return false;
    }
    QWriteLocker lock(&_queueLock);
    if (profile)
        *profile = _queueProfile.dequeue();
    if (name)
        *name = _queueName.dequeue();
    if (ba)
        *ba = _queueData.dequeue();
    //FNRETURN("success");
    return true;
} // dequeue()

void FileWriter::pumpAll(void)
{
//    FUNCTION();
    while (pumpFirst())
        ;
} // pumpAll()

int FileWriter::pumpFirst(void)
{
//    FUNCTION();
    FileWriteProfile * profile;
    QString name;
    QByteArray ba;
    if ( ! dequeue(&profile, &name, &ba))
        return 0;

    if (profile)
    {
        if (ba.isEmpty())
        {
            // new set of directories
            setupDirs(profile);
        }
        else
        {
            // write file to directories
            queuedWrite(profile, name, ba);
        }
    }
    else
    {
        if (ba.isEmpty())
        {
            // parameter change
            queuedChange(name);
        }
        else
        {
        }
    }

    if ( ! isQueueValid())
        clearQueue();

    return _queueProfile.size();
} // pumpFirst()


void FileWriter::queuedChange(const QString & key)
{
//    FUNCTION();
    if (0 == key.compare(_optQuality->keyName(), Qt::CaseInsensitive))
    {
        // new quality
        int q = _optQuality->toInt();
        foreach(FileWriteProfile * prof, _mapKeyToProfile)
            if ( ! prof->_flags.testFlag(FaceImage))
                prof->_quality = q;
    }
    else if (0 == key.compare(_optFormat->keyName(), Qt::CaseInsensitive))
    {
        // new format
        QString fmt = _optFormat->toString();
        foreach(FileWriteProfile * prof, _mapKeyToProfile)
            if ( ! prof->_flags.testFlag(FaceImage) && ! prof->_flags.testFlag(XmlText))
                prof->_format = fmt;
    }
    else if (0 == key.compare(_optFaceQuality->keyName(), Qt::CaseInsensitive))
    {
        // new face quality
        int q = _optFaceQuality->toInt();
        foreach(FileWriteProfile * prof, _mapKeyToProfile)
            if (FaceImage & prof->_flags)
                prof->_quality = q;
    }
    else if (0 == key.compare(_optFaceFormat->keyName(), Qt::CaseInsensitive))
    {
        // new face format
        QString fmt = _optFaceFormat->toString();
        foreach(FileWriteProfile * prof, _mapKeyToProfile)
            if (FaceImage & prof->_flags)
                prof->_format = fmt;
    }
    else if (0 == key.compare(_optBaseDir->keyName(), Qt::CaseInsensitive))
    {
        // new base dir
        foreach(FileWriteProfile * prof, _mapKeyToProfile)
            setupDirs(prof);
    }
    else if (0 == key.compare(_optMaxCache->keyName(), Qt::CaseInsensitive))
    {
        // new max cache
        int m = _optMaxCache->toInt();
        foreach(FileWriteProfile * prof, _mapKeyToProfile)
            if (Cache & prof->_flags)
                prof->_maxCache = m;
    }
    else
        emit error(tr("Unhandled key: %1").arg(key));

} // queuedChange()

QDir FileWriter::baseDir(void) const
{
    QDir dir(QDir::current());

    QString baseName = _optBaseDir->toString();
    baseName.replace('@', timeStampString());
    if ( ! baseName.isEmpty())
    {
        if ( ! dir.mkpath(baseName))
            emit error("Unable to make path: " + baseName);
        if ( ! dir.cd(baseName))
            emit error("Unable to change to path: " + baseName);
    }

    return dir;
} // baseDir()

void FileWriter::setupDirs(FileWriteProfile * profile)
{
//    FUNCTION();
    NULLPTR(profile);
    profile->_dirs.clear();
    QString delimitedNames = profile->_opt->toString();
    if (delimitedNames.isEmpty())
    {
        TRACE("%1: No Output Directories", profile->name());
        emit directorySet(profile->name(), QString());
        return;
    }

    foreach (QString dirName, delimitedNames.split(';'))
    {
        dirName = dirName.simplified();
        if (dirName.isEmpty())
            continue;

        QDir dir = baseDir();

        dirName.replace('@', timeStampString());
        if ( ! dirName.isEmpty())
        {
            if ( ! dir.mkpath(dirName))
                emit error("Unable to make path: " + dirName);
            if ( ! dir.cd(dirName))
                emit error("Unable to change to path: " + dirName);
        }

        profile->_dirs.append(dir);

        if (Cache & profile->_flags)
            foreach (QString fileName, dir.entryList(QDir::Files, QDir::Unsorted))
                if ( ! dir.remove(fileName))
                    emit error("Error removing " + dir.absoluteFilePath(fileName));

        TRACE("%1: Output to %2", profile->name(), dir.absolutePath());
        emit directorySet(profile->name(), dir.absolutePath());
    }

    cache_dir_q.clear();
    foreach (FileWriteProfile * fwp, _mapKeyToProfile.values())
        foreach(QDir dir, fwp->dirs())
            if (fwp->_flags & FileWriter::Cache)
                cache_dir_q.enqueue(dir);
} // setupDirs()

void FileWriter::queuedWrite(FileWriteProfile * profile, const QString & baseName, const QByteArray & ba)
{
//    FUNCTION();
    NULLPTR(profile);
    bool tmp = false; // TODO-Test: TempAndRename & profile->_flags;
    foreach (QDir dir, profile->_dirs)
    {
        QString fileName;
        if (CaptureImage & profile->_flags)
            fileName = dir.absoluteFilePath(baseName);
        else if (Copy & profile->_flags)
            fileName = dir.absoluteFilePath(baseName + "." + QFileInfo((QString)ba).suffix());
        else
            fileName = dir.absoluteFilePath(baseName + "." + profile->_format);

        QFileInfo fi(fileName);
        QString path = fi.path();
        if ( ! dir.exists(path))
        {
            if ( ! dir.mkpath(path))
                emit error("Unable to make path: " + path);
            if ( ! dir.cd(path))
                emit error("Unable to change to path: " + path);
        }

        if (Copy & profile->_flags)
        {
            QFile file((QString)ba);
            if (tmp)
            {
                if ( ! file.copy(fileName + ".TMP"))
                    emit error(Return::qfileError(file).toString());
                else if ( ! QFile::rename(fileName + ".TMP", fileName))
                    emit error("Error renaming to " + fileName);
            }
            else if ( ! file.copy(fileName))
                emit error(Return::qfileError(file).toString());
        }
        else
        {
            QFile file(fileName + (tmp ? ".TMP" : ""));
            if ( ! file.open(QIODevice::WriteOnly))
                emit error(Return::qfileError(file).toString());
            else if (ba.size() != file.write(ba))
                emit error(Return::qfileError(file).toString());
            else if (tmp)
            {
                if ( ! QFile::rename(fileName + ".TMP", fileName))
                    emit error("Error renaming to " + fileName);
            }
        }

        profile->_filesPending.removeAll(baseName);
        TRACE("%2 file: %1 written", fileName, profile->name());
        emit fileWritten(fileName);
    }
} // queuedWrite()

void FileWriter::cacheClean(void)
{
    //    FUNCTION();
    int maxCache = _optMaxCache->toInt();
    if (maxCache && ! cache_dir_q.isEmpty())
    {
        QDir dir(cache_dir_q.dequeue());
        QStringList fileNames = dir.entryList(QDir::Files, QDir::Time);
        TRACE("cacheClean() %1 has %2 files > %3",
              dir.absolutePath(), fileNames.size(), ((5 * maxCache) / 4));
        if (fileNames.size() > ((5 * maxCache) / 4))
        {
            while (fileNames.size() > maxCache)
            {
                QString fileName = fileNames.takeLast();
                if ( ! dir.remove(fileName))
                    emit error("Error removing " + dir.absoluteFilePath(fileName));
                else
                    DETAIL("cacheClean() removed %1", dir.absoluteFilePath(fileName));
            }
            TRACE("cacheClean() %1 now has %2 files, max=%3",
                  dir.absolutePath(), fileNames.size(), maxCache);
        }
        cache_dir_q.enqueue(dir);
    }

    QTimer::singleShot(_cacheMsec, this, SLOT(cacheClean()));
} // cacheClean()

void FileWriter::dump(void) const
{
    PROGRESS("***FileWriter %1 profiles Base=%2",
             _mapKeyToProfile.size(), baseDir().absolutePath());
    foreach (QString name, _mapKeyToProfile.keys())
    {
        FileWriteProfile * fwp = profile(name);
        if (fwp)
        {
            if (fwp->isActive())
                fwp->dump();
            else
                DETAIL("---%1 inactive", fwp->name());
        }
        else
        {
            TRACE("null FileWriteProfile %1", name);
        }
    }
} // dump()

