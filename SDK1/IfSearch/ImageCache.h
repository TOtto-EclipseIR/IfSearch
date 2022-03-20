/** @file ImageCache.h
*	Public declaration of ImageCache class for DDT image library
*
*	@author	Dynamic DeZigns Technology
*/

#pragma once

#include <qglobal.h>

#include <QDateTime>
#include <QDir>
#include <QHash>
#include <QImage>
#include <QList>
#include <QMap>
#include <QObject>
#include <QPoint>
#include <QQueue>
#include <QSize>
#include <QStringList>
#include <QVector>

class QDomElement;
class QLabel;
class QFile;
class QReadWriteLock;

//#include <MugBook.h>

class FeatureSet;
class ImageCacheEntry;
class ImageCacheWriter;
class ImageCacheCleaner;
class ImageInfo;

class ImageCacheWriteProfile : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString Format READ format WRITE setFormat);
    Q_PROPERTY(int Quality READ quality WRITE setQuality);

    friend class ImageCache;

public:
    QString format(void) { return Format; }
    void setFormat(QString v) { Format = v; }
    int quality(void) { return Quality; }
    void setQuality(int v) { Quality = v; }

private:
    QString Format;
    int Quality;

public:
    ImageCacheWriteProfile(QString filePath);
    ~ImageCacheWriteProfile();
    static void setBaseDir(QString filePath);
    QString path(void) { return dir.path(); }

private:
    void newBaseDir(QDir newBase);

private:
    static QDir baseDir;
    static QList<ImageCacheWriteProfile *> profiles;
    QDir dir;
    QString specifiedPath;
}; // ImageCacheWriteProfile

class ImageCache : public QObject
{
    Q_OBJECT

    friend class ImageCacheCleaner;
    friend class ImageCacheWriter;
    friend class ImageCacheEntry;

public:
    ImageCache(int MaxItems=64);
    ~ImageCache();

    QImage getImage(const QString & imageId, QSize szDesired=QSize());
    QImage getImage(const QString &imageId, int minSize, int maxSize=0);
    qreal getScore(const QString & imageId);
    ImageInfo getInfo(const QString & imageId);
    bool crop(const QString & imageId, const QPoint center, const QSize size, qreal scale=0.0);
    QString getImageFileName(const QString & imageId);
    QString grab(QFile * file, QString imageId=QString());
    QString grab(QByteArray * ba,
                 const QString & format,
                 const qint64 epochMsec,
                 const QString & cameraId=QString());
    QString grab(const QByteArray & ba,
                 const QString & format,
                 const QString & imageId,
                 const qint64 epochMsec);
    bool contains(const QString & ImageId);
    bool isGrabEmpty(void);
    int grabSize(void);
    int size(void);
    QString takeGrab(void);
    void flushGrab(void);
    void removeGrab(const QString & imageId);
    QString add(const QString & imageId, const QImage & image, const qreal Score=0.0);
    QString addFile(const QString & fileName, const QString & imageId=QString());
    QString grabFile(const QString & fileName, const QString & imageId=QString());
    void hold(const QString & imgageId, QObject * holder);
    void release(const QString & imageId, QObject * holder=0);
    bool start(void);
    bool scheduleWrite(const QString & ImageId, ImageCacheWriteProfile * Profile);
    bool deferredMove(const QString & ImageId, const QString & dirName);
    bool writeTo(const QString & ImageId, const QString & suffix,
                 ImageCacheWriteProfile * Profile);
    QString writeFile(const QString & ImageId, QString dirName,
                      const QString & suffix, const int quality);
    void setWriteToFormat(const QString & fnFormat) { writeToFormat = fnFormat; }
    void setStatusLabel(QLabel * wgt) { wgtStatus = wgt; }
    bool isHeld(const QString & imageId);
    QString heldBy(const QString & imageId);
    void compress(const QString & imageId);
    void doDeferred(const QString & imageId);
    void addFace(const QString & imageId, const FeatureSet & fSet);
    void setTemplate(const QString & imageId, const QDomElement & deTemplate);
    void clear(void);
    QByteArray getData(const QString & imageId);
    QString format(const QString & imageId);
    void setImageText(const QString & imageId,
                      const QString & key,
                      const QString &  text);
    void setHistoryMode(bool mode, const QString & overrideDirName=QString());
    void setMaxHistory(int max);
    void addToHistory(const QString & imageId);
    QString historyId(const QString & fileName);
    QImage historyImage(const QString & fileName)
    { return getImage(historyId(fileName)); }
    void addDerivative(const QString & imageId, const QString & derivativeId);
    void removeDerivatives(const QString & imageId);
    QString status(void);
    QDateTime timeStamp(const QString & imageId);
    QString fileName(const QString & imageId);

public slots:
    void remove(QString  imgageId);
    void remove(QStringList imageIds);
    int writeNext(void); // returns # pending, -1 if no writeDir
    int cleanNext(void);
    int cleanAll(void);

signals:
    void warn(QString warning);

private:
    ImageCacheEntry * find(const QString & imageId);
    //		void add(ImageCacheEntry * ice);
    //		void grab(ImageCacheEntry * ice);
    void use(const QString & imageId);
    ImageCacheEntry * allocICE(const QString & imageId);
    ImageCacheEntry * freeICE(const QString & imageId);
    QString timeStamp(void) { return timeStampString; }

private:
    QMap<QString,ImageCacheEntry *> map;
    //		QMap<QString, int> indexMap;
    //		QList<int> indexFree;
    //		QVector<ImageCacheEntry> entries;
    QList<QString> grabPending;
    QList<QString> writePending;
    QList<QString> used;
    QReadWriteLock * rwlMap;
    QReadWriteLock * rwlGrab;
    QReadWriteLock * rwlWrite;
    QReadWriteLock * rwlUsed;
    ImageCacheWriter * writer;
    ImageCacheCleaner * cleaner;
    int maxItems;
    int curBytes;
    QLabel * wgtStatus;
    QString writeToFormat;
    QString timeStampString;
    QObject historyObject;
    int maxHistory;
    QQueue<QString> historyQueue;
    QHash<QString,QString> historyDerivatives;
    QString overrideDir;
    bool historyMode;
}; // class ImageCache
