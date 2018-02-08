#include "IdGenerator.h"

#include <QtCore/QDateTime>

QChar IdGenerator::trigger('%');
QChar IdGenerator::pathSub('/');

IdGenerator::IdGenerator()
    : VariableSet("IdGenerator")
{
    DEFINE_VARIABLESET_CTORS(IDGENERATOR_VARIABLESET);
}

void IdGenerator::setFaceFormat(const QString formatString)
{
    set(Face, formatString);
}

void IdGenerator::setFrameFormat(const QString formatString)
{
    set(Frame, formatString);
}

BasicId IdGenerator::face(const QString & outputType)
{
    return generate(Face, outputType);
}

BasicId IdGenerator::frame(const QString & outputType)
{
    return generate(Frame, outputType);
}

BasicId IdGenerator::generate(const int formatIndex,
                              const QString & outputType)
{
    pathSub = '/';
    QString result;
    QString format = value(formatIndex).toString();
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
        if ('/' == cValue)
        {
            pathSub = *it++;
            continue;
        }

        qint64 iValue = numericValue(cValue);
        if (iValue < 0)
        {
            result.append(stringValue(cValue, outputType));
            continue;
        }

        if ('!' == *it)
        {
            ++it;
            result.append(cValue);
            result.append(QString("%1").arg(iValue / 100 * 100, 3, 10, QChar('0')));
        }
        else if (':' == *it)
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
        else if (iValue)
        {
            result.append(cValue);
            result.append(QString::number(iValue));
        }
    }

    return BasicId(result);
}

qint64 IdGenerator::numericValue(const QChar c)
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
    case 'l':   return getModifiedMst();
    case 'z':   return getBaseMsd();
    case 'q':   return getQuality();
    case 'Q':   return getBestQuality();
    default:    return -1;
    }
}

QString IdGenerator::stringValue(const QChar c,
                            const QString & outputType)
{
    switch (c.cell())
    {
    case '%':   return "%";
    case 'i':   return getFrameId();
    case 'd':   return getModifiedMst().toString("yyyyMMdd");
    case 't':   return getModifiedMst().toString("hhmmsszzz");
    case 'o':   return outputType;
    case 'a':   return QFileInfo(getFileName()).absoluteDir().path().replace('/', pathSub);
    case 'p':   return QFileInfo(getFileName()).dir().path().replace('/', pathSub);
    case 'n':   return QFileInfo(getFileName()).completeBaseName();
    default:    return QString();
    }
}
