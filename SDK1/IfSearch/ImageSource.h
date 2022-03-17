/** @file ImageSource.h
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

class ImageSource : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int SampleMsec READ sampleMsec WRITE setSampleMsec)
    Q_PROPERTY(int MaxCache READ maxCache WRITE setMaxCache)
    Q_PROPERTY(bool Paused READ paused WRITE setPaused)
    Q_PROPERTY(QString SourceId READ sourceId WRITE setSourceId)

public: // property write
    void setSampleMsec(int v);
    void setMaxCache(int v) { MaxCache = v; }
    void setPaused(bool v) { Paused = v; }
    void setSourceId(QString v) { SourceId = v; }

public: // property read
    int sampleMsec(void) { return SampleMsec; }
    int maxCache(void) { return MaxCache; }
    bool paused(void) { return Paused; }
    QString sourceId(void) { return SourceId; }

private: // properties
    int SampleMsec;
    int MaxCache;
    bool Paused;
    QString SourceId;

public:
    ImageSource(QObject * parent=0);
    ~ImageSource();

    void setCache(ImageCache * Cache) { cache = Cache; }
    void setStatusHandler(StatusHandler * Status) { status = Status; }
    QString process(void);
    QString restart(void);
    QString start(void);
    QString stop(void);
    QString selectNetcam(QString Url, const QString & Format=QString());
    bool isHttp(void) { return 0 != mpHttp; }
    QString sourceName(void) const { return name; }
    static qint64 currentEpochMsec(void)
    { return QDateTime::currentMSecsSinceEpoch(); }
    qint64 msecsSinceLastGrab(void) const
    {
        return (lastgrab_ems < 0)
                ? 0
                : (currentEpochMsec() - lastgrab_ems);
    }
    qint64 grabGetEpochMsec(void) const
    { return grabGet_ems; }
    qint64 lastGrabEpochMsec(void) const
    { return lastgrab_ems; }

private:
    QString canGrab(void);

public slots:
    void grab(void);
    void reset(void);

private slots:
    void sample(void);
    void httpDone(bool Error);
    void stateChanged(int state);

signals:
    void grabDone(QImage image);
    void grabCached(QString imageId);
    void grabError(QString error);
    void grabWarn(QString warning);


private:
    qint64 grabGet_ems;
    qint64 lastgrab_ems;
    ImageCache * cache;
    StatusHandler * status;
    QUrl url;
    QString name;
    QTimer * timer;

    QHttp * mpHttp=nullptr;
    QByteArray * array;
    QBuffer * buffer;
    QString imageFormat;
};
