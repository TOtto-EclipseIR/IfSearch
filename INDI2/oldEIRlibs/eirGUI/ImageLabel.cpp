#include "ImageLabel.h"

#include <QtDebug>
#include <QtSvg>
#include <QtGui/QImage>
#include <QtGui/QPixmap>
#include <QtGui/QMouseEvent>

#include "../eirExe/Log.h"

ImageLabel::ImageLabel(QWidget * parent)
    : QLabel(parent)
{
    FUNCTION(OBJNAME(parent));
    setObjectName("ImageLabel");
}

ImageLabel::ImageLabel(const QImage defaultImage,
                       QWidget * parent)
    : QLabel(parent)
    , default_pixmap(QPixmap::fromImage(defaultImage))
{
    FUNCTION(defaultImage.size(), OBJNAME(parent));
    TRACE("eirGUI resources: %1", QDir(":/").entryList());
    setObjectName("ImageLabel");
    clear();
}

void ImageLabel::clear(void)
{
    FUNCTION();
    if (default_pixmap.isNull())
        QLabel::clear();
    else
        QLabel::setPixmap(default_pixmap.scaled(QLabel::size(), Qt::KeepAspectRatio));
    QLabel::update();
}

void ImageLabel::setDefault(const QImage & image)
{
    default_pixmap = QPixmap::fromImage(image);
}

void ImageLabel::set(const QImage & image)
{
    FUNCTION(image.byteCount());
    if ( ! image.isNull())
        QLabel::setPixmap(QPixmap::fromImage(image.scaled(QLabel::size(),
                                              Qt::KeepAspectRatio)));
    else
        QLabel::setPixmap(QPixmap());
    QLabel::update();
}

void ImageLabel::mousePressEvent(QMouseEvent * event)
{
    FUNCTION(event->button());
    if (Qt::LeftButton == event->button())
        emit clicked();
}
