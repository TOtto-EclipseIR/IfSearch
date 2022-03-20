/** @file QQRect.h
*	Declaration of QQRect class for DDT core library
*
*	@author	Dynamic DeZigns Technology
*/

#pragma once
#include <qglobal.h>

#include <QRect>

class QQRect : public QRect
{
public:
    QQRect(const QRect & r=QRect())
        : QRect(r) {};
    QQRect(QSize sz)
        : QRect(QPoint(0,0), sz) {}
    QQRect(QSize sz, QPoint ctr)
        : QRect(QPoint(ctr.x() - sz.width()/2, ctr.y() - sz.height()/2), sz) {};
    qreal aspect(void) const { return (qreal)width() / (qreal)height(); }
    int area(void) const { return width() * height(); }
    int overlapArea(QRect other);
    bool isNeighbor(QRect other, qreal factor);
    void matchAspect(const QSize sz);
    QQRect scaled(const qreal scale) const;
    QQRect operator*(const qreal scale) const;
}; // class QQRect
