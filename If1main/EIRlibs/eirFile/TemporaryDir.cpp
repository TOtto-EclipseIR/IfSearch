#include "TemporaryDir.h"

#include <QtDebug>
#include <QtCore/QDateTime>

TemporaryDir::TemporaryDir(QDir baseDir,
                           const QString & baseName,
                           const bool deleteAfter)
    : QQDir(baseDir)
    , deleteAfter_bool(deleteAfter)
{
    QString dirName;
    quint32 u = (quint32)QDateTime::currentMSecsSinceEpoch();
    do
        dirName = QString("%1.%2").arg(baseName)
                  .arg(u++, 8, 16, QChar('0'));
    while (QDir::exists(dirName));
    if ( ! QDir::mkdir(dirName))
        warning_string = "Unable to make "+dirName;
    if ( ! QDir::cd(dirName))
        warning_string = "Unable to cd to "+dirName;
    qDebug() << "TemporaryDir c'tor:"
             << QDir::absolutePath() << warning_string;
}

TemporaryDir::~TemporaryDir()
{
    if (deleteAfter_bool)
        QQDir::removeAll(*this);
}

bool TemporaryDir::isWarning(void) const
{
    return warning_string.isEmpty();
}

QString TemporaryDir::warningString(void) const
{
    return warning_string;
}
