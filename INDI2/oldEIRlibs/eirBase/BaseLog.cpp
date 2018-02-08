#include "baselog.h"
#include <QSharedData>

#include <QtDebug>
#include <QtCore/QCoreApplication>
#include <QtCore/QDateTime>
#include <QtCore/QFile>
#include <QtCore/QTemporaryFile>

Severity BaseLog::min_severity;
Severity BaseLog::fatal_severity;
QFile * BaseLog::blog_file = 0;

class BaseLogData : public QSharedData
{
public:
    BaseLogData(void);
    BaseLogData(const Severity severity,
                const QString & format,
                const QVariant & value1,
                const QVariant & value2,
                const QVariant & value3,
                const QVariant & value4);
    BaseLogData(const Severity severity,
                 const QString & format,
                 const QVariantList & qvl);

public:
    void write(void) const;

    qint64 time_ems;
    Severity _severity;
    QString format_s;
    QVariantList _varList;
};

BaseLogData::BaseLogData(void)
    : time_ems(0)
    , _severity(Severity())
{
}

BaseLogData::BaseLogData(const Severity severity,
                         const QString & format,
                         const QVariant & value1,
                         const QVariant & value2,
                         const QVariant & value3,
                         const QVariant & value4)
    : time_ems(QDateTime::currentMSecsSinceEpoch())
    , _severity(severity)
    , format_s(format)
{
    _varList << value1 << value2 << value3 << value4;
}

BaseLogData::BaseLogData(const Severity severity,
             const QString & format,
             const QVariantList & qvl)
    : time_ems(QDateTime::currentMSecsSinceEpoch())
    , _severity(severity)
    , format_s(format)
    , _varList(qvl)
{
}

void BaseLogData::write(void) const
{
    QDateTime dt(QDateTime::fromMSecsSinceEpoch(time_ems));
    QString s = QString("%1%2 %3")
            .arg(_severity.toChar())
            .arg(dt.toString("hh:mm:ss.zzz"))
            .arg(eirBase::formatMessage(format_s, _varList));
    if (BaseLog::isFatal(_severity))
        qFatal(qPrintable(s));
    else if (_severity.isError())
        qCritical(qPrintable(s));
    else if (_severity.isWarning())
        qWarning(qPrintable(s));
    else if (BaseLog::isMinimum(_severity))
        qDebug(qPrintable(s));
}


BaseLog::BaseLog(void)
    : data(new BaseLogData)
{
    staticCtor();
}

BaseLog::BaseLog(const Severity severity,
                 const QString & format,
                 const QVariant & value1,
                 const QVariant & value2,
                 const QVariant & value3,
                 const QVariant & value4)
    : data(new BaseLogData(severity, format, value1, value2, value3, value4))
{
    staticCtor();
    data->write();
}

BaseLog::BaseLog(const Severity severity,
             const QString & format,
             const QVariantList & qvl)
    : data(new BaseLogData(severity, format, qvl))
{
    staticCtor();
    data->write();
}

BaseLog::BaseLog(const BaseLog & rhs)
    : data(rhs.data)
{
}

BaseLog & BaseLog::operator=(const BaseLog & rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

BaseLog::~BaseLog()
{
}

void BaseLog::staticCtor(void)
{
    if (min_severity.isNull())      min_severity    = Severity::Data;
    if (fatal_severity.isNull())    fatal_severity  = Severity::Pointer;
}

void BaseLog::setMinimum(const Severity severity)
{
    min_severity = severity;
}

void BaseLog::setFatal(const Severity severity)
{
    fatal_severity = severity;
}

Severity BaseLog::fatalLevel(void)
{
    return fatal_severity;
}

bool BaseLog::isMinimum(const Severity severity)
{
    return severity >= min_severity;
}

bool BaseLog::isFatal(const Severity severity)
{
    qDebug() << "BaseLog::isFatal()";
    qDebug() << severity.name();
    qDebug() << fatal_severity.name();
    return severity >= fatal_severity;
}

QFile * BaseLog::blogFile(void)
{
    return blog_file;
}

void BaseLog::setFile(const QString & fileName)
{
    resetFile();

    if (fileName.isEmpty())
    {
        QString templateName("EclipseBlog.tmp");
        if (qApp && ! qApp->applicationName().isEmpty())
            templateName = qApp->applicationName() + "Blog.tmp";
        blog_file = new QTemporaryFile(templateName);
    }
    else
    {
        blog_file = new QFile(fileName);
    }
    if (blog_file)
    {
        blog_file->open(QIODevice::WriteOnly | QIODevice::Text);
        if (blog_file->isWritable())
            qInstallMsgHandler(&myMsgHandler);
        else
            resetFile();
    }
}

void BaseLog::resetFile(void)
{
    if (blog_file)
    {
        blog_file->flush();
        blog_file->close();
        delete blog_file;
        blog_file = 0;
    }
    qInstallMsgHandler(0);
}

void EIRBASESHARED_EXPORT myMsgHandler(QtMsgType t, const char * pc)
{
    if (BaseLog::blogFile())
    {
        BaseLog::blogFile()->write(pc);
        BaseLog::blogFile()->write("\n");
        BaseLog::blogFile()->flush();
    }
    if (QtFatalMsg == t)
        abort();
}

