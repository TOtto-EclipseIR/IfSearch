#include "QQIcon.h"

#include <QtDebug>
#include <QtSvg>
#include <QtGui/QImage>
#include <QtGui/QPainter>

QList<QSize> QQIcon::size_list = QList<QSize>() << QSize(12, 12)
                                                << QSize(16, 16)
                                                << QSize(24, 24)
                                                << QSize(32, 32)
                                                << QSize(48, 48)
                                                << QSize(64, 64);

QQIcon::QQIcon(void)
{
}

QQIcon::QQIcon(const QString & name)
{
    *(QIcon *)this = create(name);
}

QIcon QQIcon::create(const QString & name)
{
    QFileInfo fi(name);
    if (fi.exists())
    {
        if (fi.suffix().startsWith("svg", Qt::CaseInsensitive))
            return fromSvg(fi.absoluteFilePath());
        else
            return fromImageFile(fi.absoluteFilePath());
    }

    if (QIcon::hasThemeIcon(name))
        return QIcon::fromTheme(name);

    QDir dir(":/icons/" + (QIcon::themeName().isEmpty()
                                ? "standard"
                                : QIcon::themeName()));
    //qDebug() << "QQIcon::create() fallback dir:" << dir.absolutePath();
    QFileInfoList fis(dir.entryInfoList(QStringList() << name + ".*", QDir::Files));
    if ( ! fis.isEmpty())
    {
        fi = fis.first();
        if (fi.suffix().startsWith("svg", Qt::CaseInsensitive))
            return fromSvg(fi.absoluteFilePath());
        else
            return fromImageFile(fi.absoluteFilePath());
    }

    return fromSvg(":/icons/eIRonly.svg");
}

QIcon QQIcon::fromSvg(const QString & fileName)
{
    QIcon icon;
    QSvgRenderer sr(fileName);
    if (sr.isValid())
    {
        foreach (QSize sz, size_list)
        {
            QImage i(sz, QImage::Format_ARGB32_Premultiplied);
            i.fill(Qt::transparent);
            QPainter p(&i);
            sr.render(&p);
            p.end();
            icon.addPixmap(QPixmap::fromImage(i));
        }
    }
    return icon;
}

QIcon QQIcon::fromImageFile(const QString & fileName)
{
    QIcon icon;
    foreach (QSize sz, size_list)
        icon.addFile(fileName, sz);
    return icon;
}
