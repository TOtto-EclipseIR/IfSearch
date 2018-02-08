/** @file ImageCacheCleaner.cpp
*	Definition of internal ImageCacheCleaner class for DDT image library
*
*	@author	Dynamic DeZigns Technology
*
*/

#include <ImageCache.h>
#include "iImageCache.h"
#include <InfoMacros.h>

#include <QApplication>
#include <QReadWriteLock>
#include <QLabel>

using namespace DDT;

ImageCacheCleaner::ImageCacheCleaner(ImageCache * parent)
    : cache(parent), done(false), QThread((QObject *)parent)
{
    normalPriority = QThread::NormalPriority;
    highPriority = QThread::NormalPriority;
}

ImageCacheCleaner::~ImageCacheCleaner()
{
}

void ImageCacheCleaner::run(void)
{
    QString imageId;
    int msSleep = 1000;
    setPriority(normalPriority);

    for (;;)
    {
        if (done)
            return;

        msleep(msSleep);
        int k = cache->cleanNext();

        if (k > cache->maxItems)
        {
            setPriority(highPriority);
            k = cache->cleanAll();
            setPriority(normalPriority);
        }
        msSleep = k ? 50 : 1000;
    } // forever
}


QString ImageCache::status(void)
{
    QString sts = tr("%1 cached, %2MB, %4 grabbed, %3 pending")
                  .arg(size())
                  .arg(curBytes / 1024 / 1024)
                  .arg(writePending.size())
                  .arg(grabPending.size());
    DETAIL(sts);
    return sts;
} // status()


int ImageCache::cleanNext(void)
{
    QString removeId;

    if (rwlUsed && ! rwlUsed->tryLockForRead(10))
        return -2;
    for (int i = 0; i < used.size(); i++)
        if ( ! isHeld(used.at(i)))
        {
            removeId = used.at(i);
            break;
        } // if (!held)
    if (rwlUsed) rwlUsed->unlock();

    if (removeId.isEmpty())
        return 0;
    else
    {
        remove(removeId);
        return used.size();
    }
} // cleanNext()


int ImageCache::cleanAll(void)
{
    QStringList removeIds;

    if (rwlUsed && ! rwlUsed->tryLockForRead(10))
    {
        TRACE("ImageCache::cleanAll() can't lock");
        return -2;
    }

    for (int i = 0; i < used.size(); i++)
    {
        if (isHeld(used.at(i)))
            DETAIL("%1 held by %2", used.at(i), heldBy(used.at(i)));
        else
            removeIds << used.at(i);
    }
    if (rwlUsed) rwlUsed->unlock();

    if ( ! removeIds.isEmpty())
        emit remove(removeIds);
    return used.size();
} // cleanAll()
