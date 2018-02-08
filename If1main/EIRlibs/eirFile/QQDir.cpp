#include "QQDir.h"

#include <QtGui/QImageReader>

#include "../eirTypes/MillisecondTime.h"
#include "../eirExe/Log.h"

QQDir QQDir::nullDir("/dev/null");

QQDir::QQDir(void)
    : QDir(nullDir)
{
}

QQDir::QQDir(const QString & path,
             const MillisecondTime base)
    : QDir(parse(path, base))
    , base_mst(base)
{
}

QQDir::QQDir(const QDir & other)
    : QDir(other)
{
}

bool QQDir::isNull(void) const
{
    return QDir::absolutePath() == nullDir.absolutePath();
}

bool QQDir::cd(const QString & dirName)
{
    return QDir::cd(parse(dirName));
}

bool QQDir::mkdir(const QString & dirName)
{
    return QDir::mkdir(parse(dirName));
}

bool QQDir::mkpath(const QString & dirName)
{
    return QDir::mkpath(parse(dirName));
}

bool QQDir::rmdir(const QString & dirName)
{
    return QDir::rmdir(parse(dirName));
}

void QQDir::setPath(const QString & dirName)
{
    QDir::setPath(parse(dirName));
}

bool QQDir::moveFrom(const QString & filePath)
{
    QFileInfo fi(filePath);
    return QFile::rename(filePath, QDir::absoluteFilePath(fi.fileName()));
}

bool QQDir::removeAll(const Filters filters)
{
    return QQDir::removeAll(*this);
}

bool QQDir::removeAll(QDir dir, const Filters filters) // [static]
{
    bool success = true;    
    foreach (QString dirName, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
    {
        success &= dir.cd(dirName);
        success &= removeAll(dir); // recursive
        success &= dir.cdUp();
        if (filters & Dirs)
            success &= dir.rmdir(dirName);
    }
    if (filters & Files)
        foreach (QString fileName, dir.entryList(QDir::Files))
            success &= dir.remove(fileName);
    return success;
}

QString QQDir::parse(const QString & path,
                     const MillisecondTime mst) const
{
    //FUNCTION(path);
    QString result(path);
    result.replace('@', (mst.isNull()
                            ? base_mst
                            : mst).toString("DyyyyMMdd-Thhmm"));
    //FNRETURN(result);
    return result;
}



