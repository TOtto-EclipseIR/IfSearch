/** @file ImageCacheEntry.cpp
*	Definition of ImageCache class for DDT image library
*
*	@author	Dynamic DeZigns Technology
*
*/

#include "iImageCache.h"

#include <QApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QMutexLocker>


#include <InfoMacros.h>

ImageCacheEntry::ImageCacheEntry(ImageCache * parent)
    : QObject(parent), cache(parent)
{
    ctor();
}

ImageCacheEntry::ImageCacheEntry(QString ImageId, ImageCache * parent)
    : QObject(parent), cache(parent), imageId(ImageId)
{
    ctor();
}

ImageCacheEntry::~ImageCacheEntry()
{
    free();
}

ImageCacheEntry::ImageCacheEntry(const ImageCacheEntry & that)
    : QObject(that.parent()), cache(that.cache), imageId(that.imageId)
{
    this->copyCtor(that);
}

ImageCacheEntry & ImageCacheEntry::operator=(const ImageCacheEntry & that)
                                            {
    this->copyCtor(that);
    return *this;
}

void ImageCacheEntry::free(void)
{
    doDeferredMove();
    //	if (cache && cache->status)
    //	cache->status->detail("ICE free() for %1 %2", imageId, QString::number((ulong)this, 16));
    imageId.clear();
    fileName.clear();
    fileFormat.clear();
    deferredMove.clear();
    baFileData.clear();
    orig = QImage();
    foreach(QImage img, others)
        img = QImage();
    if (cache) cache->curBytes -= curBytes;
    curBytes = 0;
    timeStamp = QDateTime();
    holds.clear();
    score = 0.0;
    profile = 0;
    info.clear();
} // free()

ImageCacheEntry * ImageCacheEntry::copy(const QString & newId)
{
    ImageCacheEntry * rtn = new ImageCacheEntry(newId, cache);
    rtn->copyCtor(*this);
    return rtn;
}

void ImageCacheEntry::ctor(void)
{
    curBytes = 0,
    score = 0.0,
    profile = 0;
}

void ImageCacheEntry::copyCtor(const ImageCacheEntry & that)
{
    baFileData = that.baFileData;
    fileFormat = that.fileFormat;
    fileName = that.fileName;
    orig = that.orig;
    curBytes = that.baFileData.size() + that.orig.numBytes();
    timeStamp = that.timeStamp;
    score = that.score;
    info = that.info;
    deferredMove = that.deferredMove;
    profile = that.profile;
} // copyCtor()

void ImageCacheEntry::doDeferredMove(void)
{
    QFileInfo fi(fileName);
    if ( ! fi.exists() && ! deferredMove.isEmpty())
    {
        WARNING("Move file %1 no longer exists", fileName);
        return;
    }
    if ( ! deferredMove.isEmpty() && fileName.isEmpty())
    {
        WARNING("No file name for deferred move to %1: %2",
             deferredMove, imageId);
        return;
    }

    if ("~" == deferredMove && ! fileName.isEmpty())
    {
        QMutexLocker lock(&mutex);
        if (QFile::remove(fileName))
            TRACE("Deleted %1", fileName);
        else
            ERRMSG("Error deleting %1", fileName);
        deferredMove.clear();
    }
    else if (! deferredMove.isEmpty() && ! fileName.isEmpty())
    {
        QMutexLocker lock(&mutex);
        QDir dir(fi.absolutePath());
        dir.mkdir(deferredMove);
        dir.cd(deferredMove);
        QString newName = dir.absoluteFilePath(fi.fileName());
        if (QFile::exists(newName))
        {
            if (QFile::remove(newName))
                TRACE("Removed existing %1", newName);
            else
                ERRMSG("Error removing existing %1", newName);
        }
        if (QFile::rename(fileName, newName))
            TRACE("Moved %1 to %2", fileName, newName);
        else
            ERRMSG("Error moving %1 to %2", fileName, newName);
        if (QFile::exists(fileName))
        {
            WARNING("%1 still exists", fileName);
            if (QFile::remove(fileName))
                TRACE("Removed existing %1", fileName);
            else
                ERRMSG("Error removing existing %1", fileName);
        }
        deferredMove.clear();
    }
} // moveDir()
