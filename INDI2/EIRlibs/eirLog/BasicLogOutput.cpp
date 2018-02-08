#include "BasicLogOutput.h"

#include <QFile>

#include <eirBase/BaseLog.h>

#include "LogFilter.h"
#include "LogItem.h"

BasicLogOutput::BasicLogOutput(void)
{
    set(BaseLog, BaseLog::url());
    for (int o = 0; o < nFiles; ++o)
    {
        OutputType otype = OutputType(o);
        QString name(sysEnvVarName(otype));
        if ( ! name.isEmpty())
        {
            QString env(eirBase::instance()->envVar(name));
            if ( ! env.isEmpty())
                set(otype, QUrl(env));
        }
    }
}

QString BasicLogOutput::sysEnvVarName(const enum OutputType otype) const
{
    switch (otype)
    {
    case LogFile:   return QString("EIRLOGFILE");
    case TraceFile: return QString("EIRTRACEFILE");
    case StdOut:    return QString("EIRLOGOUT");
    case StdErr:    return QString("EIRLOGERR");

    case nFiles:
    case BaseLog:
    case nMaximum:
        break;
    }
    return QString();
}

bool BasicLogOutput::set(const enum OutputType otype,
                         const QUrl & url)
{
    _filters[otype].clear();
    if (otype < nFiles && _file[otype])
    {
        _file[otype]->close();
        delete _file[otype];
        _file[otype] = 0;
    }

    QString fileName(url.path());
    QFile::OpenMode omode = QIODevice::Text
            | QIODevice::Truncate
            | QIODevice::WriteOnly;
    switch (otype)
    {
    case LogFile:
        _file[otype] = new QFile(fileName);
        _file[otype]->open(omode);
        break;

    case TraceFile:
        _file[otype] = new QFile(fileName);
        _file[otype]->open(omode);
        break;

    case StdOut:
        _file[otype] = new QFile("stdout");
        _file[otype]->open(stdout, omode);
        break;

    case StdErr:
        _file[otype] = new QFile("stderr");
        _file[otype]->open(stderr, omode);
        break;

    case BaseLog:
        break;

    case nFiles:
    case nMaximum:
    default:
        return false;
    }

    if (otype < nFiles
            && _file[otype]
            && ! _file[otype]->isWritable())
    {
        delete _file[otype];
        _filters[otype].clear();
        return false;
    }

    QUrlQuery query(url);
    _filters[otype].set(query);

    return true;
}

void BasicLogOutput::write(LogItem & li)
{
    if (_filters[BaseLog].evaluate(li))
        BaseLog::write(li.getSeverity(),
                       li.toString(optionMask_i[BaseLog]));
    for (int o = 0; o < nFiles; ++o)
    {
        OutputType otype = OutputType(o);
        if (_file[otype] && _filters[otype].evaluate(li))
        {
            _file[otype]->write(qPrintable(li.toString(optionMask_i[otype])));
            _file[otype]->write("\r\n");
        }
    }
}

void BasicLogOutput::flush(void)
{
    for (int o = 0; o < nFiles; ++o)
    {
        OutputType otype = OutputType(o);
        if (_file[otype])
            _file[otype]->flush();
    }
}
