/** @file ImageCacheWriter.cpp
*	Definition of internal ImageCacheWriter class for DDT image library
*
*	@author	Dynamic DeZigns Technology
*
*/

#include "iImageCache.h"

#include <QBuffer>
#include <QDomDocument>
#include <QDomElement>
#include <QDir>
#include <QFile>
#include <QImageWriter>
#include <QReadWriteLock>



ImageCacheWriter::ImageCacheWriter(ImageCache * parent)
	: cache(parent), done(false), QThread((QObject *)parent)
{
}

ImageCacheWriter::~ImageCacheWriter()
{
}

void ImageCacheWriter::run(void)
{
	int msSleep = 1000;

	for (;;)
	{
		if (done)
			return;
	
		msleep(msSleep);

		int more = cache->writeNext();

		msSleep = more ? 5000 : 100;
	}
} // run()

// returns # pending, -1 if no writeDir, -2 if locked, -3 on error		
int ImageCache::writeNext(void) 
{
	int rtn;

	if (rwlWrite && ! rwlWrite->tryLockForRead())
		return -2;
	rtn = writePending.size();
	if (rwlWrite) rwlWrite->unlock();
	if (0 == rtn)
		return rtn;

	if (rwlWrite && ! rwlWrite->tryLockForWrite())
		return -2;
	QString imageId = writePending.takeFirst();
	rtn = writePending.size();
	if (rwlWrite) rwlWrite->unlock();

	ImageCacheEntry * ice = find(imageId);
	if (ice)
	{
		ImageCacheWriteProfile * profile = qobject_cast<ImageCacheWriteProfile *>(ice->profile);
		if ( ! profile)
			return -1;

		QString fmt = profile->format();
		if (fmt.isEmpty())
			fmt = ice->fileFormat;
		if (fmt.isEmpty())
            fmt = "jpg";

		bool ok = ! writeFile(imageId, profile->path(), fmt, profile->quality()).isEmpty();
		rtn = ok ? 0 : -3;

		release(imageId, ice->profile);
	}

	return rtn;
} // writeNext()

QString ImageCache::writeFile(const QString & imageId, 
						   QString dirName, 
						   const QString & suffix, 
						   const int quality)
{
	QString rtn;

	ImageCacheEntry * ice = find(imageId);
	if ( ! ice)
	{
		emit warn(tr("Image %1 not in cache").arg(imageId));
		return QString();
	}

	if ( ! overrideDir.isEmpty())
		dirName = overrideDir;
	if (dirName.isEmpty())
	{
//		if (status) status->detail("Output disabled for %1", imageId);
		return QString();
	}

	QString fmt = suffix;
	if (fmt.isEmpty())
		fmt = ice->fileFormat;
	if (fmt.isEmpty())
        fmt = "jpg";

	if ( ! ice->info.isEmpty() || ! ice->faceTemplate.isNull())
	{   
		QImageWriter iw;
		iw.setFormat(qPrintable(fmt));
		if ( ! iw.supportsOption(QImageIOHandler::Description))
			fmt = "PNG";

		QDomDocument doc("INDIinfo");
		QDomElement root = doc.createElement("INDIinfo");
		doc.appendChild(root);
		if ( ! ice->info.isEmpty())
			ice->info.writeDomElement(&root);
		if ( ! ice->faceTemplate.isNull())
			root.appendChild(ice->faceTemplate.toElement());

		QString xml = doc.toString();
		ice->orig.setText("INDIinfo", xml);

		ice->curBytes -= ice->baFileData.size();
		curBytes -= ice->baFileData.size();
		ice->baFileData.clear();
	}

	if (ice->baFileData.isEmpty() || 0 != fmt.compare(ice->fileFormat, Qt::CaseInsensitive))
	{
		ice->curBytes -= ice->baFileData.size();
		curBytes -= ice->baFileData.size();
		ice->baFileData.clear();
		QBuffer buffer(&ice->baFileData);
		buffer.open(QIODevice::WriteOnly);
		ice->orig.save(&buffer, qPrintable(fmt), quality ? quality : -1);
		ice->fileFormat = fmt;
		ice->curBytes += ice->baFileData.size();
		curBytes += ice->baFileData.size();
	}

	QStringList dirNameList = dirName.split(";");
	foreach (QString thisDir, dirNameList)
	{
		if (thisDir.contains("@"))
			thisDir.replace("@", timeStampString);
		QString fileName = thisDir;
		fileName += "/" + imageId;
		fileName += "." + fmt;
		if (fileName.contains("{K}"))
			fileName.replace("{K}", tr("-%1KB")
				.arg((int)(ice->baFileData.size() / 1024), 4, 10, QChar('0')));
		if (fileName.contains("@"))
			fileName.replace("@", QDateTime::currentDateTime().toString("DyyyyMMdd-Thhmm"));

		QDir dir;
		QFileInfo fi(fileName);
		dir.mkpath(fi.absolutePath());
        rtn = fi.completeBaseName();

		QFile file(fileName);
		if (file.open(QIODevice::WriteOnly))
		{
			if (ice->baFileData.size() != file.write(ice->baFileData))
			{
				emit warn(tr("Unable to write %1", "warning").arg(fileName));
				return QString();
			}
		}
		else
		{
			emit warn(tr("Unable to open %1", "warning").arg(fileName));
			return QString();
		}
	} // foreach()

	return overrideDir.isEmpty() ? rtn : QString();
} // WriteFile()



QDir ImageCacheWriteProfile::baseDir;
QList<ImageCacheWriteProfile *> ImageCacheWriteProfile::profiles;

ImageCacheWriteProfile::ImageCacheWriteProfile(QString filePath)
	: dir(baseDir), Quality(0)
{
	if (filePath.contains("@"))
	{
		QString timeStamp = "D" + QDateTime::currentDateTime().toString("yyyyMMdd")
						  + "-T" + QDateTime::currentDateTime().toString("hhmm");
		filePath.replace("@", timeStamp);
	}
	specifiedPath = filePath;
	dir.mkpath(filePath);
	dir.cd(filePath);
	profiles.append(this);
} // c'tor
		
ImageCacheWriteProfile::~ImageCacheWriteProfile() 
{
	profiles.removeAll(this);
} // d'tor

		
void ImageCacheWriteProfile::setBaseDir(QString filePath)
{
	baseDir = QDir::current();
	if (filePath.contains("@"))
	{
		QString timeStamp = "D" + QDateTime::currentDateTime().toString("yyyyMMdd")
						  + "-T" + QDateTime::currentDateTime().toString("hhmm");
		filePath.replace("@", timeStamp);
	}
	baseDir.mkpath(filePath);
	baseDir.cd(filePath);

	foreach(ImageCacheWriteProfile * p, profiles)
		p->newBaseDir(baseDir);
}
		
void ImageCacheWriteProfile::newBaseDir(QDir newBase)
{
	dir = newBase;
	dir.mkpath(specifiedPath);
	dir.cd(specifiedPath);
}
