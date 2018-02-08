#include "QQRectF.h"

#include <QtCore/QSizeF>
#include <QtCore/QString>

QQRectF::QQRectF(const QRectF rf)
    : QRectF(rf)
{
}

QQRectF::QQRectF(const QSizeF size,
                 const QPointF center)
    : QRectF(QPointF(center.x() - (size.width()  / 2.0),
                     center.y() - (size.height() / 2.0)),
             size)
{
}

bool QQRectF::isUnit(void) const
{
    return (x() >= 0. && x() <= 1.
            && y() >= 0. && y() <= 1.
            && width() >= 0. && width() <= 1.
            && height() >= 0. && height() <= 1.);
}

QRect QQRectF::rect(const int iWidth)
{
    if ( ! iWidth || isNull() || ! isUnit()) return toRect();
    qreal fWidth = iWidth;
    QRectF rf(QPointF(x() * fWidth, y() * fWidth),
              QSizeF(width() * fWidth, height() * fWidth));
    return rf.toRect();
}

QSize QQRectF::size(const int iWidth)
{
    if ( ! iWidth || isNull() || ! isUnit()) return toRect().size();
    qreal fWidth = iWidth;
    QSizeF szf(width() * fWidth, height() * fWidth);
    return szf.toSize();
}

QString QQRectF::toString(void) const
{
    return QString("%1,%2~%3,%4").arg(left()).arg(top())
            .arg(right()).arg(bottom());
}
