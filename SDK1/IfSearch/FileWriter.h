/** @file FileWriter.h
*	Declaration of FileWriter class for DDT core library
*
*	@author	Dynamic DeZigns Technology
*/

#pragma once
#include <qglobal.h>
#ifdef DDTIMG_LIB
# define DDTIMG_EXPORT Q_DECL_EXPORT
#else
# define DDTIMG_EXPORT Q_DECL_IMPORT
#endif

class QDir;
class QFile;
#include <QByteArray>
#include <QDateTime>
#include <QMap>
#include <QQueue>
#include <QString>
#include <QStringList>
#include <QReadWriteLock>

#include <DDTcore.h>

class Setting;
class Settings;
class StatusHandler;
class ImageCache;
class FileWriteProfile;

class DDTIMG_EXPORT FileWriter : public QObject
{
    Q_OBJECT

    friend class FileWriteProfile;
public:
    enum Flag
    {
        $nullFlag           =  0,
        Cache               =  1,	// Manage cache size
        FaceImage           =  2,	// Use FaceQuality/Format
        XmlText             =  4,	// xml file extension
        CaptureImage        =  8,	// Copy from cache BA data
        Copy                = 16,	// Copy existing file
        TempAndRename       = 32,   // Create TempFile then Rename to Target
        OtherImage          = 64,   // Unused TOTO: Remove
    };
    Q_DECLARE_FLAGS(Flags, Flag)

    enum
    {
        ReturnNoCache = 0x88010001,
        ReturnNullImage,
    };

public:
    FileWriter(Settings * settings, const QString & keyFormat=QString(), QObject * parent=0);
    ~FileWriter(void);
    void setCacheDirs(const QString & cacheDirs);
    FileWriteProfile * newProfile(const QString & name, Flags f=$nullFlag, QString key=QString());
    FileWriteProfile * newProfile(const QString & name, QString key);
    FileWriteProfile * profile(const QString & name);
    void setImageCache(ImageCache * cache);
    bool addLogFile(StatusHandler * sts);
    void start(int longMsec=500, int shortMsec=50, int cacheMsec=60000);
    bool isQueueValid(void);
    bool isQueueEmpty(void);
    int pumpFirst(void);
    void pumpAll(void);
    QString timeStampString(void);
    QDir baseDir(void) const;
    int size(void) const { return _queueProfile.size(); }
    void dump(void) const;

public slots:
    void settingChanged(QString key);

signals:
    void directorySet(QString profileName, QString absoluteDirName);
    void fileWritten(QString absoluteFilePath);
    void error(QString message);

private:	// for friends
    void enqueue(FileWriteProfile * profile,
                 const QString & name,
                 const QByteArray & ba=QByteArray());

private:
    void clearQueue(void);
    bool dequeue(FileWriteProfile ** profile,
                 QString * name,
                 QByteArray * ba);
    void queuedChange(const QString & key);
    void setupDirs(FileWriteProfile * profile);
    void queuedWrite(FileWriteProfile * profile,
                     const QString & name,
                     const QByteArray & ba);

private slots:
    void pulse(void);
    void cacheClean(void);

private:
    QDateTime _dateTime;
    QString _timeStampString;
    Settings * _settings;
    QString _keyFormat;
    ImageCache * _imageCache;
    Setting * _optQuality;
    Setting * _optFormat;
    Setting * _optFaceQuality;
    Setting * _optFaceFormat;
    Setting * _optBaseDir;
    Setting * _optMaxCache;
    int	_longMsec;
    int	_shortMsec;
    int	_cacheMsec;

    QStringList cacheDirNames_qsl;
    QMap<QString, FileWriteProfile *> _mapKeyToProfile;

    // pending queue
    QReadWriteLock _queueLock;
    QQueue<FileWriteProfile *> _queueProfile;
    QQueue<QString> _queueName;
    QQueue<QByteArray> _queueData;
    QQueue<QDir> cache_dir_q;

}; // class FileWriter

Q_DECLARE_OPERATORS_FOR_FLAGS(FileWriter::Flags);
