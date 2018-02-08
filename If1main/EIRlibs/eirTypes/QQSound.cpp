#include "QQSound.h"

#include <QtCore/QDir>
#include <QtCore/QFileInfo>


QQSound::QQSound(const QString & filename,
                 QObject *parent)
    : QSound(filename, parent)
{
}

bool QQSound::play(const QString & filename)
{
    QFileInfo fi(filename);
    fi.makeAbsolute();
    if ( ! fi.exists()) return false;
    QString fn(fi.absoluteFilePath());
    fn = QDir::toNativeSeparators(fn);
    QSound::play(fn);
    return true;
}
