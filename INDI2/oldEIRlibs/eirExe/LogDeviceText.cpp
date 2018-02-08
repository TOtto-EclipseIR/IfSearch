#include "LogDeviceText.h"

#include "../eirBase/BaseLog.h"
#include "../eirTypes/MillisecondTime.h"

LogDeviceText::LogDeviceText(const VariableSet & config,
                             Logger * parent)
    : LogDevice(config, parent)
{
    QString name(config.value("FileName").toString());
    LogDeviceType ldt(config.value("Device").toString());

    if (LogDeviceType::StdIo == ldt)
    {
        if (0 == name.compare("stderr", Qt::CaseInsensitive))
        {
            _file.setFileName("stderr");
            if (_file.open(stderr, QIODevice::WriteOnly))
                _stream.setDevice(&_file);
        }
        else
        {
            _file.setFileName("stdout");
            if (_file.open(stdout, QIODevice::WriteOnly))
                _stream.setDevice(&_file);
        }
    }
    else
    {
        name.replace('@', MillisecondTime::baseString("DyyyyMMdd-Thhmm"));
        QFileInfo fi(name);
        QDir dir(fi.absoluteDir());
        _file.setFileName(dir.absoluteFilePath(fi.fileName()));
        if (dir.mkpath("."))
            if (_file.open(QIODevice::WriteOnly))
                _stream.setDevice(&_file);
    }

    if (_stream.device() && _stream.device()->isWritable())
    {
        //BLOG(Severity::LogMsg, "Opened log to %s", qPrintable(_file.fileName()));
    }
    else
    {
        //BLOG(Severity::Warning, "Unable to write to %s: %s",
          //   qPrintable(_file.fileName()),
            // qPrintable(_file.errorString()));
    }
}

LogDeviceText::~LogDeviceText()
{
}

bool LogDeviceText::isValid(void) const
{
    return  _stream.device() && _stream.device()->isWritable();
}

void LogDeviceText::write(const Severity severity,
                          const QString & string)
{
    (void)severity;
    _stream << string << "\r\n";
}

void LogDeviceText::flush(void)
{
    _stream.flush();
}

