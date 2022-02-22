/** @file ImageCache.cpp
*	Definition of ImageCache class for DDT image library
*
*	@author	Dynamic DeZigns Technology
*
*/

#include <QBuffer>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QImage>
#include <QLabel>
#include <QReadWriteLock>
#include <QStringList>

#include <QQRect.h>
#include <InfoMacros.h>
#include "iImageCache.h"


ImageCache::ImageCache(int MaxItems)
    : maxItems(MaxItems), curBytes(0), historyObject(this)
    , rwlMap(0), rwlGrab(0), rwlWrite(0), rwlUsed(0), maxHistory(0)
    , writer(0), cleaner(0), wgtStatus(0), historyMode(false)
{
    timeStampString = QDateTime::currentDateTime().toString("-DyyyyMMdd-Thhmm");
    historyObject.setObjectName("cacheHistory");
} // default c'tor

ImageCache::~ImageCache()
{
    // stop threads if started
    if (writer)
    {
        //		if (status)
	//		status->prog(tr("Image Cache Writer shutdown"));
        writer->done = true;
        writer->wait();
    }

    if (cleaner)
    {
        //		if (status)
	//		status->prog(tr("Image Cache Cleaner shutdown"));
        cleaner->done = true;
        cleaner->wait();
    }

    // delete map entries
    //	if (status)
    //	status->prog(tr("Deleting Image Cache items"));
    //	for (int i = 0; i < entries.size(); ++i)
    //		entries[i].free();
    foreach (ImageCacheEntry * ice, map)
    {
        continue;
        if (ice)
        {
#if 0
            if (status)
            {
                foreach(QObject * holder, ice->holds)
                    status->detail("hby ICE @0x%2 for %1 by %3",
                                   ice->imageId, QString::number((ulong)ice,16),
                                   holder->objectName().isEmpty()
                                   ? QString::number((ulong)holder,16)
                                       : holder->objectName());
                status->detail("del ICE @0x%2 for %1", ice->imageId, QString::number((ulong)ice,16));
            }
#endif
            delete ice;
        }
    }

    //	if (status)
    //	status->prog(tr("Image Cache destroyed"));
} // d'tor

/*
void ImageCache::setStatusHandler(StatusHandler * sts) 
{ 
	status = sts; 
	if (status)
		timeStampString = status->tsStart().toString("DyyyyMMdd-Thhmm");
}
*/
bool ImageCache::contains(const QString & ImageId) 
{ 
    if (rwlMap)		rwlMap->lockForRead();
    bool b = map.contains(ImageId);
    if (rwlMap)		rwlMap->unlock();
    return b;
}

bool ImageCache::isGrabEmpty(void) 
{
    if (rwlGrab)		rwlGrab->lockForRead();
    bool b = grabPending.isEmpty();
    if (rwlGrab)		rwlGrab->unlock();
    return b;
}

int ImageCache::size(void) 
{ 
    if (rwlMap)		rwlMap->lockForRead();
    int k = map.size();
    if (rwlMap)		rwlMap->unlock();
    return k;
}


int ImageCache::grabSize(void) 
{ 
    if (rwlGrab)		rwlGrab->lockForRead();
    int k = grabPending.size();
    if (rwlGrab)		rwlGrab->unlock();
    return k;
}

QString ImageCache::takeGrab(void) 
{ 
    if (rwlGrab)		rwlGrab->lockForWrite();
    QString imageId = grabPending.isEmpty() ? QString() : grabPending.takeFirst();
    if (rwlGrab)		rwlGrab->unlock();
    return imageId;
}

void ImageCache::flushGrab(void)
{
    foreach(QString id, grabPending)
        remove(id);
}


ImageCacheEntry * ImageCache::find(const QString & imageId) 
{ 
    if (rwlMap)		rwlMap->lockForRead();
    ImageCacheEntry * ice = map.contains(imageId) ? map.value(imageId) : 0;
    if (rwlMap)		rwlMap->unlock();
    return ice;
}

QByteArray ImageCache::getData(const QString & imageId) 
{
    ImageCacheEntry * ice = find(imageId);
    if (ice)
        return ice->baFileData;
    else
        return QByteArray();
}

QString ImageCache::format(const QString & imageId) 
{
    ImageCacheEntry * ice = find(imageId);
    if (ice)
        return ice->fileFormat;
    else
        return QString();
}

QString ImageCache::fileName(const QString & imageId)
{
    ImageCacheEntry * ice = find(imageId);
    if (ice)
        return ice->fileName;
    else
        return QString();
}

QDateTime ImageCache::timeStamp(const QString & imageId)
{
    ImageCacheEntry * ice = find(imageId);
    if (ice)
        return ice->timeStamp;
    else
        return QDateTime();
}

ImageCacheEntry * ImageCache::allocICE(const QString & imageId)
{
    if (rwlMap)		rwlMap->lockForWrite();
#if 1
    ImageCacheEntry * ice = new ImageCacheEntry(imageId, this);
    //	if (status) status->detail("new ICE @0x%2 for %1", imageId, QString::number((ulong)ice,16));
    map.insert(imageId, ice);
#else
    int x = indexMap.contains(imageId) ? indexMap[imageId] : -1;
    ImageCacheEntry * ice = (x < 0 || x >= entries.size()) ? 0 : &entries[x];
    if (ice)
    {
        ice->free();
        ice->imageId = imageId;
        ice->cache = this;
    }
    else
    {
        if (entries.isEmpty() || indexFree.isEmpty())
        {
            int n = entries.size() + maxItems / 4;
            while (entries.size() < n)
            {
                indexFree.append(entries.size());
                entries.append(ImageCacheEntry(this));
            }
        }
        x = indexFree.takeFirst();
        ice = &entries[x]; FATAL( ! ice);
        ice->cache = this;
        ice->imageId = imageId;
        indexMap[imageId] = x;
    }
#endif
    if (rwlMap)		rwlMap->unlock();
    return ice;
}

ImageCacheEntry * ImageCache::freeICE(const QString & imageId)
{
    if (rwlMap)		rwlMap->lockForWrite();
#if 1
    ImageCacheEntry * ice = map.contains(imageId) ? map[imageId] : 0;
    if (ice)
    {
        //		if (status) status->detail("del ICE @0x%2 for %1", imageId, QString::number((ulong)ice,16));
        curBytes -= ice->curBytes;
        delete ice;
        map.remove(imageId);
    }
#else
    int x = indexMap.contains(imageId) ? indexMap[imageId] : -1;
    ImageCacheEntry * ice = (x < 0 || x >= entries.size()) ? 0 : &entries[x];
    if (ice)
    {
        ice->free();
        indexMap.remove(imageId);
        indexFree.append(x);
    }
#endif
    if (rwlMap)		rwlMap->unlock();
    return ice;
}

QString ImageCache::add(const QString & imageId, const QImage & Image, const qreal Score)
{
    ImageCacheEntry * ice = allocICE(imageId);
    //	ImageCacheEntry * ice = new ImageCacheEntry(imageId, this);
    ice->orig = Image;
    ice->curBytes = Image.sizeInBytes();
    curBytes += ice->curBytes;
    ice->score = Score;
    use(ice->imageId);
    return QString();
} // add(Image)

QString ImageCache::addFile(const QString & fileName, const QString & imageId)
{
    QString iid = imageId.isEmpty() ? fileName : imageId;
    ImageCacheEntry * ice = allocICE(iid);
    if (ice)
    {
        QFileInfo fi(fileName);
        ice->fileName = fileName;
        ice->timeStamp = fi.lastModified();
        use(ice->imageId);
    }
    return (ice) ? iid : QString();
}

QString ImageCache::grabFile(const QString & fileName, const QString & imageId)
{
    QString iid = addFile(fileName, imageId);
    if ( ! iid.isEmpty())
        grabPending.append(iid);
    return iid;
}

/*
void ImageCache::grab(ImageCacheEntry * ice)
{
	Q_ASSERT(ice == qobject_cast<ImageCacheEntry *>(ice));
	add(ice);
	if (rwlGrab)		rwlGrab->lockForWrite();
	Q_ASSERT(ice == qobject_cast<ImageCacheEntry *>(ice));
	grabPending.append(ice->imageId);
	if (rwlGrab)		rwlGrab->unlock();
} // grab(ice)
*/
void ImageCache::removeGrab(const QString & imageId)
{
    if (rwlGrab)		rwlGrab->lockForWrite();
    grabPending.removeAll(imageId);
    if (rwlGrab)		rwlGrab->unlock();
} // removeGrab()
/*
void ImageCache::add(ImageCacheEntry * ice)
{
	Q_ASSERT(ice == qobject_cast<ImageCacheEntry *>(ice));
	if (rwlMap)		rwlMap->lockForRead();
	indexMap[ice->imageId] = ice;
	if (rwlMap)		rwlMap->unlock();
	curBytes += ice->curBytes;
	use(ice->imageId);
	if (status) status->detail("ImageCache::add(): ICE pointer added for %1 0x%2", ice->imageId, QString::number((ulong)ice, 16));
} // add(ice)
*/
void ImageCache::use(const QString & imageId)
{
    if (rwlUsed)		rwlUsed->lockForWrite();
    used.removeAll(imageId);
    used.append(imageId);
    if (rwlUsed)		rwlUsed->unlock();
} // use()

void ImageCache::hold(const QString & imageId, QObject * holder)
{
    ImageCacheEntry * ice = find(imageId);
    Q_ASSERT(ice == qobject_cast<ImageCacheEntry *>(ice));

    if (rwlMap)		rwlMap->lockForWrite();
    if (ice && holder)
    {
#ifndef NDEBUG
        ImageCacheEntry * p = qobject_cast<ImageCacheEntry *>(ice);
        if (p != ice)
            return;
#endif
        use(ice->imageId);
#ifndef NDEBUG
        p = qobject_cast<ImageCacheEntry *>(ice);
        if (p != ice)
            return;
#endif
        ice->holds.append(holder);
        DETAIL("ICEhold %1[0x%2] for %3; %4 holds",
               holder->objectName(),
               QString::number((ulong)holder,16),
               imageId,
               ice->holds.size());
        foreach(QObject * obj, ice->holds)
            DETAIL("ICEheld %1", obj->objectName());
    }
    if (rwlMap)		rwlMap->unlock();

} // hold()

void ImageCache::release(const QString & imageId, QObject * holder)
{
    int holdSize = 0;
    ImageCacheEntry * ice = find(imageId);
    if (ice)
    {
        if (rwlMap)		rwlMap->lockForWrite();
        if (holder)
        {
            ice->holds.removeOne(holder);
            DETAIL("ICErlse %1[0x%2] for %3; %4 holds",
                   holder->objectName(),
                   QString::number((ulong)holder,16),
                   imageId,
                   ice->holds.size());
            holdSize = ice->holds.size();
        }
        else
        {
            DETAIL("ICErlse all holds %1",
                   ice->holds.size());
            ice->holds.clear();
            holdSize = ice->holds.size();
        }
        foreach(QObject * obj, ice->holds)
            DETAIL("ICEheld %1", obj->objectName());
        if (rwlMap)		rwlMap->unlock();
    }
    if ( ! holdSize)
        remove(imageId);
} // release()

bool ImageCache::isHeld(const QString & imageId)
{
    bool held = false;
    ImageCacheEntry * ice = find(imageId);
    if (ice)
    {
        if (rwlMap)		rwlMap->lockForRead();
        held = ! ice->holds.isEmpty();
        if (rwlMap)		rwlMap->unlock();
    }
    return held;
} // isHeld()

QString ImageCache::heldBy(const QString & imageId)
{
    QString objectNames;
    ImageCacheEntry * ice = find(imageId);
    if (ice)
    {
        if (rwlMap)		rwlMap->lockForRead();
        foreach (QObject * obj, ice->holds)
            objectNames += obj->objectName() + " ";
        if (rwlMap)		rwlMap->unlock();
    }
    return objectNames;
}

void ImageCache::clear(void)
{
    if (rwlUsed)		rwlUsed->lockForWrite();
    if (rwlWrite)		rwlWrite->lockForWrite();
    if (rwlGrab)		rwlGrab->lockForWrite();
    if (rwlMap)			rwlMap->lockForWrite();
#if 1
    foreach(ImageCacheEntry * ice, map)
    {
        if (ice)
        {
            used.removeAll(ice->imageId);
            writePending.removeAll(ice->imageId);
            grabPending.removeAll(ice->imageId);
            curBytes -= ice->curBytes;
            map.remove(ice->imageId);
            delete ice;
        }
    }
#else
    foreach(QString imageId, indexMap)
    {
        freeICE(imageId);
        used.removeAll(imageId);
        writePending.removeAll(imageId);
        grabPending.removeAll(imageId);
    }
#endif
    if (rwlUsed)		rwlUsed->unlock();
    if (rwlWrite)		rwlWrite->unlock();
    if (rwlGrab)		rwlGrab->unlock();
    if (rwlMap)			rwlMap->unlock();
}

void ImageCache::remove(QStringList imageIds)
{ 
    if (rwlUsed)		rwlUsed->lockForWrite();
    if (rwlWrite)		rwlWrite->lockForWrite();
    if (rwlGrab)		rwlGrab->lockForWrite();
    if (rwlMap)			rwlMap->lockForWrite();

    foreach(QString imageId, imageIds)
    {
#if 1
        if (map.contains(imageId))
        {
            ImageCacheEntry * ice = map[imageId];
            if (ice)
            {
                used.removeAll(ice->imageId);
                writePending.removeAll(ice->imageId);
                grabPending.removeAll(ice->imageId);
                curBytes -= ice->curBytes;
                delete ice;
            }
        }
#else
        if (indexMap.contains(imageId))
        {
            freeICE(imageId);
            used.removeAll(imageId);
            writePending.removeAll(imageId);
            grabPending.removeAll(imageId);
        }
#endif
    }

    if (rwlUsed)		rwlUsed->unlock();
    if (rwlWrite)		rwlWrite->unlock();
    if (rwlGrab)		rwlGrab->unlock();
    if (rwlMap)			rwlMap->unlock();
} // remove(list)

void ImageCache::remove(QString imageId)
{
    DETAIL("ImageCache::remove(%1)", imageId);

    if (rwlUsed)		rwlUsed->lockForWrite();
    used.removeAll(imageId);
    if (rwlUsed)		rwlUsed->unlock();

    if (rwlWrite)		rwlWrite->lockForWrite();
    writePending.removeAll(imageId);
    if (rwlWrite)		rwlWrite->unlock();

    if (rwlGrab)		rwlGrab->lockForWrite();
    grabPending.removeAll(imageId);
    if (rwlGrab)		rwlGrab->unlock();

    freeICE(imageId);

    if (rwlMap)			rwlMap->lockForWrite();
    map.remove(imageId);
    if (rwlMap)			rwlMap->unlock();
} // remove()

ImageInfo ImageCache::getInfo(const QString & imageId)
{
    ImageInfo ii;
    ImageCacheEntry * ice = find(imageId);
    if (ice)
        ii = ice->info;
    return ii;
} // getInfo()

/** Get an image from the cache of a specified size
*
* @todo	caching non-original sizes
*
*/
QImage ImageCache::getImage(const QString &imageId, QSize szDesired)
{
    ImageCacheEntry * ice = find(imageId);
    if ( ! ice || ice->imageId != imageId)
        return QImage();
    use(ice->imageId);

    if (ice->orig.isNull())
    {
        if (ice->baFileData.isEmpty())
        {
            QFile file(ice->fileName);
            file.open(QIODevice::ReadOnly);
            ice->baFileData = file.readAll();
            QFileInfo fi(ice->fileName);
            ice->fileFormat = fi.suffix();
            curBytes += ice->baFileData.size();
            ice->curBytes += ice->baFileData.size();
        }
        try
        {
            ice->orig = QImage::fromData(ice->baFileData,
                                         ice->fileFormat.isEmpty()
                                         ? 0
                                         : qPrintable(ice->fileFormat));
        }
        catch (...)
        {
            ERRMSG("Exception decoding %2 image data: %1",
                  ice->fileName, ice->fileFormat);
        }

        ice->info = ImageInfo(ice->orig);
        ice->curBytes += ice->orig.sizeInBytes();
        curBytes +=  ice->orig.sizeInBytes();
    }

    if (szDesired.isEmpty())
        return ice->orig;
    else
        return ice->orig.scaled(szDesired, Qt::KeepAspectRatio);
} // getImage()

QImage ImageCache::getImage(const QString &imageId, int minSize, int maxSize)
{
    QImage image = getImage(imageId);
    QSize szDesired = image.size();
    if (minSize && ((szDesired.width() < minSize * 3 / 4) || (szDesired.height() < minSize * 3 / 4)))
        szDesired = QSize(minSize, minSize);
    if (maxSize && ((szDesired.width() > maxSize * 5 / 4) || (szDesired.height() > maxSize * 5 / 4)))
        szDesired = QSize(maxSize, maxSize);
    return image.scaled(szDesired, Qt::KeepAspectRatio);
} // getImage()

qreal ImageCache::getScore(const QString & imageId)
{
    ImageCacheEntry * ice = find(imageId);
    if (ice)
        return ice->score;
    else
        return -1.0;
} // getScore()

QString ImageCache::getImageFileName(const QString & imageId)
{
    ImageCacheEntry * ice = find(imageId);
    if (ice)
        return ice->fileName;
    else
        return QString();
} // getImageFileName()

void ImageCache::addFace(const QString & imageId, const FeatureSet & fSet)
{
    ImageCacheEntry * ice = find(imageId);
    if (ice)
        ice->info.addFace(fSet);
} // addFace()

void ImageCache::setTemplate(const QString & imageId, const QDomElement & deTemplate)
{
    ImageCacheEntry * ice = find(imageId);
    if (ice)
        ice->faceTemplate = deTemplate;
}

void ImageCache::doDeferred(const QString & imageId)
{
    ImageCacheEntry * ice = find(imageId);
    if (ice)
        ice->doDeferredMove();
} // doDeferred()

void ImageCache::compress(const QString & imageId)
{
    ImageCacheEntry * ice = find(imageId);
    if (ice)
    {
        if (ice->baFileData.isEmpty())
        {
            QString fmt = ice->fileFormat.isEmpty() ? "PNG" : ice->fileFormat;
            QBuffer buffer(&ice->baFileData);
            buffer.open(QIODevice::WriteOnly);
            ice->orig.save(&buffer, qPrintable(fmt), -1);
        }
        ice->orig = QImage();
        ice->others.clear();
        curBytes -= ice->curBytes;
        ice->curBytes = ice->baFileData.size();
        curBytes += ice->curBytes;
    }
} // compress()

QString ImageCache::grab(QByteArray * ba, 
                         const QString & format,
                         const qint64 epochMsec,
                         const QString & cameraId)
{
    QImage img = QImage::fromData(*ba);
    if (img.isNull())
        return QString();

    QDateTime dt = QDateTime::fromMSecsSinceEpoch(epochMsec);
    QString imageId = "D" + dt.toString("yyyyMMdd") + "-T" + dt.toString("hhmmsszzz");
    if ( ! cameraId.isEmpty())
        imageId = cameraId + "-" + imageId;

    //	ImageCacheEntry * ice = new ImageCacheEntry(imageId, this);
    ImageCacheEntry * ice = allocICE(imageId);
    ice->timeStamp = dt;
    ice->baFileData = *ba;
    ice->fileFormat = format;
    ice->orig = img;
    ice->curBytes = ice->baFileData.size() + ice->orig.sizeInBytes();
    curBytes += ice->curBytes;
    grabPending.append(ice->imageId);
    use(ice->imageId);

    return imageId;
} // grab(ba)

QString ImageCache::grab(const QByteArray & ba,
                         const QString & format,
                         const QString & imageId,
                         const qint64 epochMsec)
{
    QImage img = QImage::fromData(ba, format.toLocal8Bit());
    if (img.isNull())
        return QString();

    QDateTime dt(QDateTime::fromMSecsSinceEpoch(epochMsec));
    ImageCacheEntry * ice = allocICE(imageId);
    ice->timeStamp = dt;
    ice->baFileData = ba;
    ice->fileFormat = format;
    ice->orig = img;
    ice->curBytes = ice->baFileData.size() + ice->orig.sizeInBytes();
    curBytes += ice->curBytes;
    grabPending.append(ice->imageId);
    use(ice->imageId);
    return imageId;
} // grab(ba,fmt,id,ems)

QString ImageCache::grab(QFile * file, QString imageId)
{
    QString err;
    ImageCacheEntry * ice = 0;

    QString fileName = file->fileName();
    QFileInfo fi(fileName);
    if (imageId.isEmpty())
        imageId = fi.completeBaseName();

    //	ice = new ImageCacheEntry(imageId, this);
    ice = allocICE(imageId);
    Q_ASSERT(ice == qobject_cast<ImageCacheEntry *>(ice));
    //	if (status) status->detail("ImageCache:: ICE pointer for %1 0x%2", imageId, QString::number((ulong)ice, 16));

    ice->fileName = fileName;
    ice->fileFormat = fi.suffix();
    if ( ! file->open(QIODevice::ReadOnly))
        err += tr("Error opening file %1").arg(fileName);
    else
    {
        ice->baFileData = file->read(file->size());
        if (ice->baFileData.size() != file->size())
            err += tr("Error reading file %1").arg(fileName);
        else
        {
            ice->orig = QImage::fromData(ice->baFileData, qPrintable(ice->fileFormat));
            if (ice->orig.isNull())
                err += tr("Null image read from %1").arg(fileName);
        }
    }
    use(ice->imageId);

    if (err.isEmpty())
    {
        ice->curBytes = ice->baFileData.size() + ice->orig.sizeInBytes();
        curBytes += ice->curBytes;
        grabPending.append(ice->imageId);
    }
    return err;
} // grab(file)

bool ImageCache::start(void)
{
    rwlMap = new QReadWriteLock;
    rwlGrab = new QReadWriteLock;
    rwlWrite = new QReadWriteLock;
    rwlUsed = new QReadWriteLock;

    cleaner = new ImageCacheCleaner(this);
    connect(cleaner, SIGNAL(remove(QString)), this, SLOT(remove(QString)));
    connect(cleaner, SIGNAL(remove(QStringList)), this, SLOT(remove(QStringList)));
    cleaner->start(QThread::NormalPriority);

    writer = new ImageCacheWriter(this);
    writer->start(QThread::LowPriority);

    return true;
} // start

bool ImageCache::scheduleWrite(const QString & ImageId, ImageCacheWriteProfile * Profile)
{
    ImageCacheEntry * ice = find(ImageId);
    if ( ! ice || ! Profile)
        return false;

    hold(ImageId, Profile);
    ice->profile = Profile;
    if (rwlWrite)		rwlWrite->lockForWrite();
    writePending.append(ImageId);
    if (rwlWrite)		rwlWrite->unlock();

    return true;
} // scheduleWrite()

bool ImageCache::deferredMove(const QString & ImageId, const QString & dirName)
{
    ImageCacheEntry * ice = find(ImageId);
    if ( ! ice)
        return false;

    ice->deferredMove = dirName;
    return true;
} // deferredMove()
/*
bool ImageCache::writeTo(const QString & ImageId, 
						 ImageCacheWriteProfile * Profile,
						 const int score, 
						 MugBookEntry & mbe)
{
	ImageCacheEntry * ice = find(ImageId);
	if ( ! ice || ! Profile)
		return false;

	QString newImageId;
	if ( ! writeToFormat.isEmpty())
	{
		newImageId = writeToFormat;
		if (newImageId.contains("{D}"))
			newImageId.replace("{D}", mbe.category());
		if (newImageId.contains("{P}"))
			newImageId.replace("{P}", mbe.personId());
		if (newImageId.contains("{B}"))
			newImageId.replace("{B}", ImageId);
		if (newImageId.contains("{C}"))
			newImageId.replace("{C}", (0==score) ? "" : tr("-C%1")
						.arg(score, 3, 10, QChar('0')));
		if (newImageId.contains("{K}"))
			newImageId.replace("{K}", tr("-%1KB")
						.arg((int)(ice->baFileData.size() / 1024), 4, 10, QChar('0')));
	}
	else
		newImageId = ImageId + Profile->specifiedPath;

	ImageCacheEntry * newIce = allocICE(newImageId);
	use(ice->imageId);
	*newIce = *ice;
	newIce->imageId = newImageId;
	return scheduleWrite(newImageId, Profile);
} // writeTo()
*/

bool ImageCache::writeTo(const QString & ImageId, 
                         const QString & suffix,
                         ImageCacheWriteProfile * Profile)
{
    ImageCacheEntry * ice = find(ImageId);
    if ( ! ice || ! Profile)
        return false;

    QString newImageId = ImageId + "-" + suffix;
    ImageCacheEntry * newIce = allocICE(newImageId);
    *newIce = *ice;
    newIce->imageId = newImageId;
    return scheduleWrite(newImageId, Profile);
}



bool ImageCache::crop(const QString & imageId, const QPoint center, const QSize size, qreal scale)
{
    ImageCacheEntry * ice = find(imageId);
    if ( ! ice)
        return false;

    curBytes -= ice->curBytes;
    ice->baFileData.clear();
    ice->others.clear();

    QQRect rect(size, center);
    QImage cropped = ice->orig.copy(rect);
    ice->orig = qIsNull(scale) ? cropped : cropped.scaled(size * scale, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    ice->curBytes = ice->orig.sizeInBytes();
    curBytes += ice->curBytes;

    return true;
} // crop()

QString ImageCache::historyId(const QString & fileName)
{
    QFileInfo fi(fileName);
    QString id = fi.completeBaseName();
    if (historyDerivatives.contains(id))
        id = historyDerivatives.value(id);
    if (contains(id))
        return id;
    else
        return QString();
} // historyId()

void ImageCache::addDerivative(const QString & imageId, const QString & derivativeId)
{
    if (historyMode || imageId.isEmpty() || derivativeId.isEmpty())
        return;
    historyDerivatives.insert(derivativeId, imageId);
}

void ImageCache::addToHistory(const QString & imageId)
{
    if (historyMode || imageId.isEmpty())
        return;

    compress(imageId);
    hold(imageId, &historyObject);
    historyQueue.enqueue(imageId);

    while (historyQueue.size() > maxHistory)
    {
        QString id = historyQueue.dequeue();
        release(id, &historyObject);
        remove(id);
        removeDerivatives(id);
    }
} // addToHistory()

void ImageCache::removeDerivatives(const QString & imageId)
{
    QMutableHashIterator<QString, QString> it(historyDerivatives);
    while (it.hasNext())
    {
        it.next();
        if (imageId == it.value())
            it.remove();
    }
} // removeDerivatives()

void ImageCache::setMaxHistory(int max)
{
    maxHistory = max;
    while (historyQueue.size() > maxHistory)
    {
        QString id = historyQueue.dequeue();
        release(id, &historyObject);
        remove(id);
        removeDerivatives(id);
    }
}

void ImageCache::setHistoryMode(bool mode, const QString & overrideDirName)
{ 
    historyMode = mode;
    overrideDir = historyMode ? overrideDirName : QString();
}
