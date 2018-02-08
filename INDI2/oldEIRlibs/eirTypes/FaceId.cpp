#include "FaceId.h"

#include <QtCore/QDateTime>

QChar FaceId::trigger('%');

FaceId::FaceId()
    : VariableGroup(VariableId())
{
    DEFINE_GROUPVARIABLES_CTORS(FACEID_GROUPVARIABLES);
}

QString FaceId::generate(const QString & outputType)
{
    QString result;
    QString format = getFormat();
    QString::const_iterator it = format.begin();

    while (it != format.end())
    {
        QChar cValue(*it++);
        if (trigger != cValue)
        {
            result.append(cValue);
            continue;
        }

        cValue = *it++;
        qint64 iValue = numericValue(cValue);
        if (iValue < 0)
        {
            result.append(stringValue(cValue, outputType));
            continue;
        }

        if (':' == *it)
        {
            ++it;
            bool forceZero = false;
            if ('0' == *it)
            {
                ++it;
                forceZero = true;
            }
            int w = (*it++).digitValue();
            if (iValue || forceZero)
            {
                result.append(cValue);
                result.append(QString("%1").arg(iValue, w, 10, QChar('0')));
            }
        }
        else
        {
            if (iValue)
            {
                result.append(cValue);
                result.append(QString::number(iValue));
            }
        }
    }

    return result;
}

qint64 FaceId::numericValue(const QChar c)
{
    switch (c.cell())
    {
    case 'x':   return getDetectRect().center().x();
    case 'y':   return getDetectRect().center().y();
    case 'c':   return getConsistency();
    case 'e':   return getEyeLine().length();
    case 'w':   return getDetectRect().width();
    case 'm':   return getConfidence();
    case 'r':   return getRank();
    case '#':   return getFrameNumber();
    case 'n':   return getFaceNumber();
    case 'l':   return getModifiedEms();
    case 'q':   return getQuality();
    default:    return -1;
    }
}

QString FaceId::stringValue(const QChar c,
                            const QString & outputType)
{
    switch (c.cell())
    {
    case '%':   return "%";
    case 'i':   return getFrameId();
    case 'd':   return QDateTime::fromMSecsSinceEpoch(getModifiedEms()).toString("yyyyMMdd");
    case 't':   return QDateTime::fromMSecsSinceEpoch(getModifiedEms()).toString("hhmmxxzzz");
    case 'o':   return outputType;
    default:    return QString();
    }
}
