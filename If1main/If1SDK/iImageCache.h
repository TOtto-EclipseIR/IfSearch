/** @file iImageCache.h
*	Internal declaration of ImageCache class for DDT image library
*
*	@author	Dynamic DeZigns Technology
*/

#include <qglobal.h>

#ifdef DDTIMG_LIB
# define DDTIMG_EXPORT Q_DECL_EXPORT
#else
# define DDTIMG_EXPORT Q_DECL_IMPORT
#endif

#include <ImageCache.h>
#include <ImageInfo.h>

#include <QByteArray>
#include <QDateTime>
#include <QImage>
#include <QList>
#include <QMutex>
#include <QString>
#include <QThread>

namespace DDT
{
	class ImageCacheEntry : public QObject
	{
		Q_OBJECT
		friend class ImageCache;
		friend class ImageCacheWriter;

	public:
		ImageCacheEntry(ImageCache * parent=0);
		ImageCacheEntry(QString ImageId, ImageCache * parent);
		ImageCacheEntry(const ImageCacheEntry & that);
		~ImageCacheEntry();
		ImageCacheEntry & operator=(const ImageCacheEntry & that);
		ImageCacheEntry * copy(const QString & newId);

	private:
		void doDeferredMove(void);
		void ctor(void);
		void copyCtor(const ImageCacheEntry & that);
		void free(void);

	private:
		ImageCache *		cache;
		QString				imageId;
		QByteArray			baFileData;
		QString				fileFormat;
		QString				fileName;
		QImage				orig;
		QList<QImage>		others;
		int					curBytes;
		QDateTime			timeStamp;
		QList<QObject *>	holds;
		qreal				score;
		ImageCacheWriteProfile * profile;
		QString				deferredMove;
		ImageInfo			info;
		QDomElement			faceTemplate;
		QMutex				mutex;
	}; // class ImageCacheEntry

	class ImageCacheCleaner : public QThread
	{
		Q_OBJECT
		friend class ImageCache;

	public:
		ImageCacheCleaner(ImageCache * parent);
		~ImageCacheCleaner();

	protected:
		virtual void run();

	signals:
		void remove(QString  imgageId);
		void remove(QStringList imageIds);

	private:
		ImageCache * cache;
		bool done;
		QThread::Priority normalPriority;
		QThread::Priority highPriority;
	}; // class ImageCacheCleaner

	class ImageCacheWriter : public QThread
	{
		Q_OBJECT
		friend class ImageCache;
	public:
		ImageCacheWriter(ImageCache * parent);
		~ImageCacheWriter();
	protected:
		virtual void run();
	private:
		ImageCache * cache;
		bool done;
	}; // class ImageCacheWriter
} // namespace DDT

