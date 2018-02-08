/** @file InputHotdir.h
*	Public declaration of ImageSource class for DDT image library
*
*	@author	Dynamic DeZigns Technology
*/

#include <qglobal.h>

#include <QDir>
#include <QFileSystemWatcher>
#include <QImage>
#include <QStringList>
#include <QTime>
#include <QUrl>
class QByteArray;
class QBuffer;
class QHttp;
class QTimer;

class ImageCache;
class StatusHandler;

class InputHotdir : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int SampleMsec READ sampleMsec WRITE setSampleMsec)
    Q_PROPERTY(int MaxCache READ maxCache WRITE setMaxCache)
    Q_PROPERTY(int Jump READ jump WRITE setJump)
    Q_PROPERTY(int Skip READ skip WRITE setSkip)
    Q_PROPERTY(bool Loop READ loop WRITE setLoop)
    Q_PROPERTY(bool NewOnly READ newOnly WRITE setNewOnly)
    Q_PROPERTY(int OrderBy READ orderBy WRITE setOrderBy)
    Q_PROPERTY(QString ImageId READ imageId WRITE setImageId)
    Q_PROPERTY(bool DeleteAfter READ deleteAfter WRITE setDeleteAfter)
    Q_PROPERTY(QString MoveAfter READ moveAfter WRITE setMoveAfter)
    Q_PROPERTY(QString BeginDateTime READ beginDateTime WRITE setBeginDateTime)
    Q_PROPERTY(QString EndDateTime READ endDateTime WRITE setEndDateTime)

public: // property write
    void setSampleMsec(int v);
    void setMaxCache(int v) { MaxCache = v; }
    void setJump(int v) { Jump = v; }
    void setSkip(int v) { Skip = v; }
    void setLoop(bool v) { Loop = v; }
    void setNewOnly(bool v) { NewOnly = v; }
    void setOrderBy(int v) { OrderBy = v; }
    void setDeleteAfter(bool v) { DeleteAfter = v; }
    void setMoveAfter(QString v) { MoveAfter = v; }
    void setBeginDateTime(QString v) { BeginDateTime = v; }
    void setEndDateTime(QString v) { EndDateTime = v; }
    void setImageId(QString v) { ImageId = v; }

public: // property read
    int sampleMsec(void) { return SampleMsec; }
    int maxCache(void) { return MaxCache; }
    int jump(void) { return Jump; }
    int skip(void) { return Skip; }
    bool loop(void) { return Loop; }
    bool newOnly(void) { return NewOnly; }
    int orderBy(void) { return OrderBy; }
    bool deleteAfter(void) { return DeleteAfter; }
    QString moveAfter(void) { return MoveAfter; }
    QString beginDateTime(void) { return BeginDateTime; }
    QString endDateTime(void) { return EndDateTime; }
    QString imageId(void) { return ImageId; }

private: // properties
    int SampleMsec;
    int MaxCache;
    int Jump;
    int Skip;
    bool Loop;
    bool NewOnly;
    int OrderBy;
    bool DeleteAfter;
    QString MoveAfter;
    QString BeginDateTime;
    QString EndDateTime;
    QString ImageId;



public:
    InputHotdir(QObject * parent=0);
    ~InputHotdir();

    void setCache(ImageCache * Cache) { cache = Cache; }
    void setStatusHandler(StatusHandler * Status) { status = Status; }
    QString start(const QUrl & url);
    void stop(void);
    QString process(const QString & pathName);
    QStringList filters(void) { return nameFilters; }
    QString takeFirst(void);
    int numPending(void) const { return pending.size(); }
    QString absolutePath(void) const
    { return dirRead ? dirRead->absolutePath() : QString(); }

private:
    QStringList getCurrent(void);
    void processDirChange(void);

public slots:
    void reset(void);
    bool cacheFirst(void);

private slots:
    void sample(void);
    void directoryChanged(QString path);

signals:
    void ready(void);
    void grabFile(QString fileName);
    void grabCached(QString imageId);
    void grabError(QString error);
    void grabWarn(QString warning);
    void empty(void);
    void notEmpty(void);

private:
    ImageCache * cache;
    StatusHandler * status;
    QTimer * timer;
    bool started;
    bool dirChanged;
    QDir * dirRead;
    QFileSystemWatcher * watcher;
    QString dirName;
    QStringList processed;
    QStringList pending;
    QStringList nameFilters;
    static QStringList defaultFilters;
}; // class InputHotdir
