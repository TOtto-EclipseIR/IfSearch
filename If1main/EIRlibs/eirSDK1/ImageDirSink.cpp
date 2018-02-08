#include "ImageDirSink.h"

#include <QtDebug>
#include <QtCore/QFileInfo>

#include "../eirExe/Log.h"
#include "../eirFile/TemporaryDir.h"

ImageDirSink::ImageDirSink(const QString & dirName,
                           QObject *parent)
    : QObject(parent)
    , after_dir(QQDir::nullDir)
    , _watcher(this)
{
    if (dirName.isEmpty())
    {
        TemporaryDir tempDir(QDir::temp(), "ImageDir", false);
        _dir.setPath(tempDir.absolutePath());
    }
    else
    {
        _dir.setPath(dirName);
        if (_dir.exists())
            QQDir::removeAll(_dir);
        else
        {
            _dir.mkpath(".");
            _dir.cd(".");
        }
    }

    CONNECT(&_watcher, SIGNAL(directoryChanged(QString)),
            this, SLOT(checkDir()));
    _watcher.addPath(_dir.absolutePath());
    qDebug() << "ImageDirSink watching" << _dir.absolutePath();
}

void ImageDirSink::setMoveAfter(QQDir moveAfter)
{
    after_dir = moveAfter;
}

void ImageDirSink::clear(void)
{
    QQDir::removeAll(_dir);
}

void ImageDirSink::checkDir(void)
{
    //qDebug() << "ImageDirSink::checkDir";
    foreach (QFileInfo fi, _dir.entryInfoList(QDir::Files))
    {
        //qDebug() << "ImageDirSink::checkDir" << fi.filePath();
        QImage image(fi.filePath());
        if (after_dir.isNull())
            _dir.remove(fi.fileName());
        else
            after_dir.moveFrom(_dir.absoluteFilePath(fi.fileName()));
        if ( ! image.isNull())
            emit gotImage(image, fi.completeBaseName());
    }
}
