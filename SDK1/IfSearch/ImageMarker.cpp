/** @file ImageMarker.cpp
*	Definition of ImageMarker class for DDT image library
*
*	@author	Dynamic DeZigns Technology
*
*/

#include <ImageMarker.h>

#include <QColor>
#include <QCoreApplication>
#include <QPaintDevice>
#include <QRect>



ImageMarker::ImageMarker(QImage * image)
 : QPainter(image)
{
    if (image && ! isActive() && ! image->isNull())
    {
        *image = image->convertToFormat(QImage::Format_RGB32);
        begin(image);
    }
}

ImageMarker::~ImageMarker()
{
}


void ImageMarker::rects(QList<QRect> rl, QColor color, int penWidth, int darker, const qreal penOpacity)
{
    qreal oldOpacity = opacity();
    setOpacity(penOpacity);
    foreach(QRect r, rl)
    {
        rect(r, color, penWidth);
        if (darker)
            color = color.darker(darker);
    }
    setOpacity(oldOpacity);
}

void ImageMarker::detectorScale(QSize detectorSize, QList<QSize> sizes,
                                QColor color, int penWidth,
                                Qt::Corner corner)
{
    QPoint offset, origin;

    switch (corner)
    {
    default:
        // todo, for now fall thru
        ;
    case Qt::TopLeftCorner:
        offset = QPoint(16, 16);
        origin = QPoint(offset.x() + detectorSize.width(), offset.y() + detectorSize.height());
    }

    setPen(QPen(QBrush(color), penWidth));
    // draw origin
    drawLine(QPoint(origin.x(), origin.y()-4), QPoint(origin.x(), origin.y()+4));
    drawLine(QPoint(origin.x()-4, origin.y()), QPoint(origin.x()+4, origin.y()));
    // and detector size at offset
    drawLine(QPoint(offset.x(), origin.y()-4), QPoint(offset.x(), origin.y()+4));
    drawLine(QPoint(origin.x()-4, offset.y()), QPoint(origin.x()+4, offset.y()));

    foreach (QSize sz, sizes)
    {
        drawLine(QPoint(origin.x() + sz.width(), origin.y()-4),
                 QPoint(origin.x() + sz.width(), origin.y()+4));
        drawLine(QPoint(origin.x()-4, origin.y() + sz.height()),
                 QPoint(origin.x()+4, origin.y() + sz.height()));
    }
} // detectorScale()


void ImageMarker::rect(QRect r, QColor color, int penWidth)
{
    if (r.isValid() && color.isValid())
    {
        setPen(QPen(QBrush(color), penWidth));
        drawRect(r);
    }
} // mark(QRect)

void ImageMarker::centerLines(QRect r, QColor color, int penWidth)
{
    if (r.isValid() && color.isValid())
    {
        setPen(QPen(QBrush(color), penWidth));
        drawLine(QPoint(0, r.center().y()), QPoint(r.right(), r.center().y()));
        drawLine(QPoint(r.center().x(), 0), QPoint(r.center().x(), r.bottom()));
    }
}

void ImageMarker::horizontalLine(const QPoint pt, const int length,
                                 const QColor color, const int penWidth)
{
    if (color.isValid())
    {
        setPen(QPen(QBrush(color), penWidth));
        drawLine(QPoint(pt.x()-length/2, pt.y()),
                 QPoint(pt.x()+length/2, pt.y()));
    }
}

void ImageMarker::eyes(Eyes eyes, QColor color, int penWidth, int length)
{
    if ( ! eyes.isNull() && color.isValid())
    {
        setPen(QPen(QBrush(color), penWidth));
        drawLine(QPoint(eyes.left().x()-length, eyes.left().y()),
                 QPoint(eyes.left().x()+length, eyes.left().y()));
        drawLine(QPoint(eyes.left().x(),   eyes.left().y()-length),
                 QPoint(eyes.left().x(),   eyes.left().y()+length));
        drawLine(QPoint(eyes.right().x()-length, eyes.right().y()),
                 QPoint(eyes.right().x()+length, eyes.right().y()));
        drawLine(QPoint(eyes.right().x(),   eyes.right().y()-length),
                 QPoint(eyes.right().x(),   eyes.right().y()+length));
    }
}

void ImageMarker::title(const QString text, QColor color, QColor bgColor, QFont font)
{
    QPaintDevice * dev=device();
    title(QRect(QPoint(0,0), QSize(dev->width(), dev->height())), text, color, bgColor, font);
}

void ImageMarker::title(const QRect rect, const QString text, QColor color, QColor bgColor,
                        QFont font, Qt::AlignmentFlag align)
{
    if (rect.isEmpty() || text.isEmpty())
        return;

    font.setFamily("Helvetica");
    font.setPointSize(10);
    setFont(font);
    setPen(color);

    int w = rect.width(), h = rect.height();
    QRect boxRect(rect.topLeft() + QPoint(2, (Qt::AlignBottom == align) ? (h - 16) : 2),
                  QSize(w - 4, 16));

    fillRect(boxRect, QBrush(bgColor));
    drawText(boxRect, Qt::AlignLeft | Qt::AlignVCenter | Qt::TextSingleLine, text);
} // title()


void ImageMarker::score(qreal value,
                        int width,
                        QColor barColor,
                        QColor bgColor,
                        QColor fgColor,
                        int length)
{
    QPaintDevice * dev=device();
    int w = dev->width(), h = dev->height();
    length = length ? length : (w - w % 50);
    length = length ? length : w;
    QRect boxRect(QPoint(0, h - width - 4), QSize(w, length));
    QRect barRect(QPoint((w - length) / 2, h - width - 2), QSize(length * value, width));
    QRect fgRect (QPoint(barRect.right() + 1, h - width - 2), QSize(length - barRect.width(), width));
    fillRect(boxRect, QBrush(bgColor));
    fillRect(barRect, QBrush(barColor));
    fillRect(fgRect,  QBrush(fgColor));
} // score()
