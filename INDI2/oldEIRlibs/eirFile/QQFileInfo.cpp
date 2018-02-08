#include "QQFileInfo.h"

#include <QImageReader>

QStringList QQFileInfo::readFormats_sl;


QQFileInfo::QQFileInfo(void)
{
}

bool QQFileInfo::isWildCompleteName(void) const
{
    QString completeName = QFileInfo::completeBaseName();
    return completeName.contains('*')
            || completeName.contains('?');
}

bool QQFileInfo::isWildSuffix(void) const
{
    QString suffix = QFileInfo::suffix();
    return suffix.contains('*')
            || suffix.contains('?');
}

bool QQFileInfo::isWild(void) const
{
    return isWildCompleteName() || isWildSuffix();
}

QStringList QQFileInfo::imageReadNameFilters(void) const
{
    QStringList result;
    if (isWildCompleteName() && isWildSuffix())
    {
        result << "*.*";
    }
    else if (isWildCompleteName())
    {
        result << fileName();
    }
    else if (isWildSuffix())
    {
        foreach (QString format, supportedImageReadFormats())
            result << completeBaseName() + "." + format;
    }
    else
    {
        result << fileName();
    }
    return result;
}

QStringList QQFileInfo::supportedImageReadFormats(void)
{
    if (readFormats_sl.isEmpty())
    {
        QList<QByteArray> formats = QImageReader::supportedImageFormats();
        foreach (QByteArray format, formats)
        {
            readFormats_sl << QString(format);
        }
    }
    return readFormats_sl;
}
