#include "LogItem.h"

#include "../eirTypes/MillisecondDelta.h"
#include "../eirTypes/MillisecondTime.h"

#include "LogFunction.h"
#include "LogOptions.h"

LogItem::LogItem(const QString & name)
    : VariableSet(name)
{
    DEFINE_VARIABLESET_CTORS(LOGITEM_VARIABLESET);
}

QString LogItem::toString(quint32 options) const
{
    QString result;
    Severity severity = getSeverity();

    if (options & LogOptions::NoLevel)
        ;
    else if (options & LogOptions::LevelChar)
        result += severity.toChar();
    else if (options & LogOptions::LevelName)
        result += QString("%1 ").arg(getSeverity().name(), 8);

    if (options & LogOptions::NoTime)
        ;
    else if (options & LogOptions::TimeDelta)
    {
        MillisecondDelta msd = MillisecondTime::deltaBase(getTime());
        result += msd.toString("^d+hh:mm:ss.zzz ");
    }
    else if (options & LogOptions::TimeOfDay)
        result += getTime().toString("@hh:mm:ss.zzz ");

    if (options & LogOptions::ShowFile)
    {
        QString fi = QFileInfo(getFileName()).baseName().left(16);
        QString li = (options & LogOptions::ShowLine)
                     ? ("[" + QString::number(getFileLine()) + "]")
                         : QString();
        result += QString("%1%2 ").arg(fi, 16)
                  .arg(li, (options & LogOptions::ShowLine) ? -6 : 0);
    }

    if (options & LogOptions::ShowFunction)
    {
        LogFunction lf(getFunction());
        QString fn = lf.functionName().left(16) + "()";
        if (options & LogOptions::ShowClass)
        {
            QString cn = lf.className().left(16);
            result += QString("%1::%2 ").arg(cn, 16).arg(fn, -18);
        }
        else
            result += QString("%1 ").arg(fn, 18);
    }

    if ((options & LogOptions::ShowThread) && ! getThread().isEmpty())
        result += QString("[@%1] ").arg(getThread());

    if (Severity::Enter == getSeverity()
            || Severity::Leave == getSeverity()
            || Severity::Signal == getSeverity())
        result += QString(getDepth(), QChar('='));
    else if (Severity::EnterArg == getSeverity())
        result += QString(getDepth(), QChar('-'));

    result += eirBase::formatMessage(QString(value()), values());

    if (Severity::DumpHex == getSeverity())
    {
        QByteArray ba = value(3).toByteArray();
        while (ba.size() % 16) ba.append((char)0xFF);
        int o = 0;
        while (o < ba.size())
        {
            result += QString("\r\n   +%1: ").arg(o, 8, 16, QChar('0'));
            for (int i = 0; i < 16; ++i)
            {
                if (0 == (i % 4)) result += " ";
                QChar c(ba.at(o+i));
                result += (QString("%1").arg(c.cell(), 2, 16, QChar('0'))).toUpper();
            }
            result += " - ";
            for (int i = 0; i < 16; ++i)
            {
                QChar c(ba.at(o+i));
                result += c.isPrint() ? QString(c) : ".";
            }
            o += 16;
        }
    }

    return result;
}
