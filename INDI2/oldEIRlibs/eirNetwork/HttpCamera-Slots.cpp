#include "HttpCamera.h"

#include <QBuffer>
#include <QByteArray>
#include <QHttp>
#include <QTimer>

#include "InfoMacros.h"

void HttpCamera::pause(void)
{
    FUNCTION();
}

void HttpCamera::resume(void)
{
    FUNCTION();
}

void HttpCamera::grab(void)
{
    FUNCTION();
}

void HttpCamera::httpDone(bool error)
{
    FUNCTION();
    // Kill Get timer
    get_timer->stop();

    // emit based on error
    if (error)
        emit doGetError();
    else
        emit doGot();
}

void HttpCamera::httpChanged(int state)
{
    DETAIL("HTTP State Changed to %1", state);
}

void HttpCamera::enterStarting(void)
{
    FUNCTION();
    // Set Connecting Image
    show("Connecting");

    // Setup HTTP Connection
    _url.setUrl(getURL());
    _http = new QHttp(this); NULLPTR(_http);
    _http->setObjectName("_http");
    _http->setHost(_url.host(), (_url.port() > 0) ? _url.port() : 80);
    _http->setUser(_url.userName(), _url.password());
    // TODO: if https scheme, set ConnectionModeHttps & handle SSL issues
    _array = new QByteArray; NULLPTR(_array);
    _buffer = new QBuffer(_array, this); NULLPTR(_buffer);
    _buffer->open(QIODevice::WriteOnly);
    connect(_http, SIGNAL(done(bool)), this, SLOT(httpDone(bool)));
    connect(_http, SIGNAL(stateChanged(int)), this, SLOT(httpChanged(int)));
}

void HttpCamera::enterStarted(void)
{
    FUNCTION();
    // Set Connected Image

    lastGet_ems = 0;
    // Proceed based upon initial pause state
    if (getPause())
    {
        emit doPause();
        emit paused();
    }
    else
    {
        emit doGrab();
        emit resumed();
    }
}

void HttpCamera::enterGet(void)
{
    FUNCTION();
    // Start Get and Sample Timers
    if (getGetMsec())
        get_timer->start(getGetMsec());
    if (getSampleMsec())
        sample_timer->start(getSampleMsec());

    // Issue GET Request
    QString s(_url.toString(QUrl::RemoveScheme
                                 | QUrl::RemoveAuthority));
    _buffer->reset();
    int n = _http->get(s, _buffer);
    DETAIL("HTTP GET #%2 %1", s, n);
}

void HttpCamera::enterGetting(void)
{
    FUNCTION();
    qint64 now_ems = QDateTime::currentMSecsSinceEpoch();
    setGetEms(now_ems);
    if (lastGet_ems)
    {
        qint64 delta_ms = now_ems - lastGet_ems;
        setTotalDeltaMsec(delta_ms + getTotalDeltaMsec());
    }
}

void HttpCamera::enterGetAbort(void)
{
    FUNCTION();
    // Abort HTTP Request
    _http->abort();
    QString s("HTTP Error: GET Timeout");
    ERROR(s);
    setGetTimeoutCount(getGetTimeoutCount() + 1);
    emit grabError(s);
}

void HttpCamera::enterGetError(void)
{
    FUNCTION();
    get_timer->stop();

    // Report Error
    QString s = "HTTP Error:" + _http->errorString();
    ERROR(s);
    setHttpErrorCount(getHttpErrorCount() + 1);
    emit grabError(s);

    // Start Short Sample Timer
    if (getSampleMsec())
        sample_timer->start(getSampleMsec() / 10);
}

void HttpCamera::enterGot(void)
{
    FUNCTION();
    get_timer->stop();

    if (_array->isEmpty())
    {
        QString s("HTTP Error: Empty Response");
        ERROR(s);
        setHttpEmptyCount(getHttpEmptyCount() + 1);
        emit grabError(s);
        if (getSampleMsec())
            sample_timer->start(getSampleMsec() / 10);
    }
    else
    {
        // Add received data to queue, emit grabbed(), emit full() if queue full
        DETAIL("HTTP %1 byte image queued", _array->size());
        qint64 get_ems = getGetEms();
        qint64 getting_ms = QDateTime::currentMSecsSinceEpoch()
                                - get_ems;
        QByteArray grab_array(*_array);
        GrabImage gi(get_ems, grab_array);
        grab_queue.append(gi);
        emit grabbed();
        setFrameCount(1 + getFrameCount());
        setTotalGetMsec(getting_ms + getTotalGetMsec());
        setTotalFrameBytes(_array->size() + getTotalFrameBytes());
        lastGet_ems = get_ems;
        if (isFull())
            emit full();
        // Reset Restart Timer
        if (getRestartSecs())
            restart_timer->start(1000 * getRestartSecs());
    }
}

void HttpCamera::enterPaused(void)
{
    FUNCTION();
    // Cancel Sample and Restart timers
    sample_timer->start(getSampleMsec() / 10);
    restart_timer->stop();

    emit paused();
}

void HttpCamera::exitPaused(void)
{
    FUNCTION();
    setProperty("Paused", false);

    // Start Restart timer and short Sample timer
    if (getRestartSecs())
        restart_timer->start(1000 * getRestartSecs());
    if (getSampleMsec())
        sample_timer->start(getSampleMsec() / 10);
}

void HttpCamera::enterSample(void)
{
    FUNCTION();
    if (isFull())
        emit doFull();
    else
        emit doGrab();
}

void HttpCamera::enterQueueFull(void)
{
    FUNCTION();
    // Reset Restart timer and start short Sample timer
    if (getRestartSecs())
        restart_timer->start(1000 * getRestartSecs());
    if (getSampleMsec())
        sample_timer->start(getSampleMsec() / 10);
}

void HttpCamera::enterRestarting(void)
{
    FUNCTION();
    // Set Restarting Image

    // Reset HTTP connection
    reset();
    // if not MaxRestart, shoot start()
    QTimer::singleShot(1000, this, SLOT(start()));
}

void HttpCamera::enterQuitting(void)
{
    FUNCTION();
    // Reset HTTP connection
    reset();
}
