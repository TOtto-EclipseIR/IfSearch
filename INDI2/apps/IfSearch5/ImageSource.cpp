/** @file ImageSource.cpp
*	Definition of ImageSource class for DDT image library
*
*	@author	Dynamic DeZigns Technology
*
*/


#include <QBuffer>
#include <QByteArray>
#include <QFileInfo>
#include <QHostInfo>
#include <QList>
#include <QImageReader>
#include <QTimer>

#include <ImageSource.h>
#include <ImageCache.h>
#include <InfoMacros.h>
//#include <StatusHandler.h>

#include "QtHttp/QHttp.h"


ImageSource::ImageSource(QObject * parent)
    : QObject(parent), cache(0)
    , http(0), array(0), buffer(0), timer(0), status(0)
{
    SampleMsec = 0;
    MaxCache = 0;
    Paused = false;
    lastgrab_ems = -1;
} // c'tor

ImageSource::~ImageSource()
{
    reset();
} // d'tor


void ImageSource::reset(void)
{
    FUNCTION();
    stop();

    if (http)
    {
        disconnect(http, SIGNAL(done(bool)), this, SLOT(httpDone(bool)));
        http->abort();
        delete http;
        http = 0;
    }

    if (array)		{	delete array;		array = 0; }
    if (buffer)		{	delete buffer;		buffer = 0; }

    url.clear();
} // reset()


QString ImageSource::selectNetcam(QString Url, const QString & Format)
{
    FUNCTION(Url, Format);
    reset();
    url.setUrl(Url);

    http = new QHttp(this);
    http->setHost(url.host(), (url.port() > 0) ? url.port() : 80);
    http->setUser(url.userName(), url.password());
    array = new QByteArray;
    buffer = new QBuffer(array, this);
    buffer->open(QIODevice::WriteOnly);
    imageFormat = Format;
    DETAIL("HTTP Connecting to %1", url.toString());
    connect(http, SIGNAL(done(bool)), this, SLOT(httpDone(bool)));
    connect(http, SIGNAL(stateChanged(int)), this, SLOT(stateChanged(int)));
    lastgrab_ems = -1;
    return QString();
} // selectNetcam()

QString ImageSource::restart(void)
{
    FUNCTION();
    QString err = selectNetcam(url.toString(), imageFormat);
    if (err.isEmpty())
        err = start();
    if ( ! err.isEmpty())
        emit grabError(err);
    FNRETURN(err);
    return err;
}

QString ImageSource::start(void)
{
    FUNCTION();
    timer = new QTimer(this);
    timer->start(SampleMsec ? SampleMsec : 1000);
    connect(timer, SIGNAL(timeout()), this, SLOT(sample()));
    return QString();
} // start()

QString ImageSource::stop(void)
{
    FUNCTION();
    if (timer)
    {
        timer->stop();
        delete timer;
        timer = 0;
    }
    return QString();
} // stop()

void ImageSource::sample(void)
{
    FUNCTION();
    QString why = canGrab();
    if (why.isEmpty())
        emit grab();
    else
        DETAIL("HTTP can't grab: " + why);
} // sample()

void ImageSource::stateChanged(int state)
{
    DETAIL("HTTP #%1 State %2", http->currentId(), http->state());
}

QString ImageSource::canGrab(void)
{
    FUNCTION();
    QString result;
    if (Paused)
        result = "Paused";
    else if (cache && cache->grabSize() >= (MaxCache ? MaxCache : 5))
        result = tr("Cache has %1").arg(cache->grabSize());
    else if (http && QHttp::Unconnected != http->state() /*hasPendingRequests()*/)
    {
        DETAIL("HTTP %3 busy, state=%1, error=%2", http->state(),
              http->errorString(), http->currentId());
        result = "HTTP busy";
    }
    else if (http && (lastgrab_ems > 0))
    {
        int msSample = (SampleMsec ? SampleMsec : 1000) * 7 / 8;
        int msSince  = (lastgrab_ems < 0) ? 0 : (currentEpochMsec() - lastgrab_ems);
        if (msSince < (msSample * 7 / 8))
            result = "Too Soon";
    }

    FNRETURN(result);
    return result;
} // canGrab()

void ImageSource::grab(void)
{
    FUNCTION();
    if (http)
    {
        if ( ! buffer || ! array)
        {
            delete buffer;
            buffer = 0;
            delete array;
            array = 0;

            array = new QByteArray;
            buffer = new QBuffer(array, this);
            buffer->open(QIODevice::WriteOnly);
        }

        QHttp::State state =  http->state();
        if (http->hasPendingRequests())
        {
            WARNING("HTTP busy (%1)", state);
            return;
        }

        QString request(url.toString(QUrl::RemoveScheme
                                     | QUrl::RemoveAuthority));
        buffer->reset();
        grabGet_ems = QDateTime::currentMSecsSinceEpoch();
        int n = http->get(request, buffer);
        DETAIL("HTTP GET #%2 %1", request, n);
    }
} // grab()

void ImageSource::httpDone(bool Error)
{
    FUNCTION();
    if ( ! http)
        return;
    DETAIL("HTTP %1", Error ? "EEEEEEEEEEEERRORRRRRRRRRR" : "Done");
    if (Error)
    {
        DETAIL("HTTP Grab Error=%1 state=%2", http->errorString(),
              http->state());
        emit grabError(http->errorString());
    }
    else
    {
        if (array->isEmpty())
            return;

        if (cache)
        {
            QString format = imageFormat;
            if (format.isEmpty())
            {
                QFileInfo fi(url.path());
                format = fi.suffix();
            }
            QString imageId = cache->grab(array, format, grabGetEpochMsec(), SourceId);
            if (imageId.isEmpty())
            {
                ERRMSG("HTTP Null Image");
                emit grabError("HTTP Null Image");
            }
            else
            {
                DETAIL("HTTP %1 byte image cached as %2",
                      array->size(), imageId);
                cache->hold(imageId, this);
                lastgrab_ems = currentEpochMsec();
                emit grabCached(imageId);
            }
        }
        else
        {
            QImage image = QImage::fromData(*array);
            if (image.isNull())
                emit grabError("HTTP Null Image");
            else
            {
                lastgrab_ems = currentEpochMsec();
                emit grabDone(image);
            }
        }
    }
} // httpDone()


void ImageSource::setSampleMsec(int v)
{
    FUNCTION();
    bool wasStopped = ! timer;
    stop();
    SampleMsec = v;
    if ( ! wasStopped)
        start();
}
