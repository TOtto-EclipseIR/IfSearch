/*! \file Eyes.hh Declarations for TEyes template
  *
  */

#ifndef TEYES_H
#define TEYES_H

#include <QtCore/QSize>
#include <QTextStream>

#include <math.h>

/*! \class  TEyes
  * \brief  Template class for Eye Distance Calculations
  * \tparam LINE    Line class (QLine or QLineF)
  * \tparam POINT   Point class (QPoint or QPointF)
  * \tparam VALUE   value class (int or qreal)
  */
template <class LINE, class POINT, class VALUE> class TEyes : public LINE
{
public:
    TEyes() : LINE() {}
    TEyes(POINT lt, POINT rt) : LINE(lt, rt) {}
    TEyes(LINE other) : LINE(other) {}
    TEyes(QString & string)
    {
        VALUE lx=0, ly=0, rx=0, ry=0;
        QTextStream s(&string);
        s >> lx >> ly >> rx >> ry;
        if (s.status() != QTextStream::Ok) return;
        POINT l(lx, ly), r(rx, ry);
        if (l.isNull() || r.isNull()) return;
        LINE::setPoints(l , r);
    }

    POINT left(void) const { return LINE::p1(); }
    POINT right(void) const { return LINE::p2(); }
    VALUE width(void) const { return LINE::dx(); }
    QString toString(void) const
    {
        return QString("%1,%2~%3,%4")
                .arg(LINE::x1()).arg(LINE::y1())
                .arg(LINE::x2()).arg(LINE::y2());
    }
    void set(POINT lt, POINT rt) { setP1(lt), setP2(rt); }
    POINT center(void) const
    { return POINT((left().x() + right().x()) / (VALUE)2,
                   (left().y() + right().y()) / (VALUE)2); }
    VALUE distance(void) const
    { return (VALUE)sqrt((qreal)LINE::dx() * (qreal)LINE::dx()
                             + (qreal)LINE::dy() * (qreal)LINE::dy()); }
    POINT noseTip(const qreal scale=1.0) const
    { return POINT(center().x(), center().y()
                   + scale * distance() / (VALUE)2); }
    POINT topOfHead(const qreal scale=1.0) const
    { return POINT(center().x(), center().y()
                   - scale * distance() * (VALUE)2); }
    LINE operator*(const qreal scale) const
    { return LINE(left() * scale, right() * scale); }
    LINE operator+(const POINT offset) const
    { return LINE(left() + offset, right() + offset); }

    static LINE expected(QSizeF size)
    {
        qreal half = size.width() / 4.0;
        POINT nose(size.width() / 2.0, size.height() / 2.0);
        POINT left = nose + POINT(-half, -half);
        POINT right = nose + POINT(half, -half);
        return LINE(left, right);
    }
    static LINE fromSizeDistance(QSizeF size, VALUE distance)
    {
        POINT ctr(size.width() / 2, size.height() / 2);
        POINT lEye = ctr + POINT( - distance / 2, - distance / 2);
        POINT rEye = ctr + POINT( + distance / 2, - distance / 2);
        return LINE(lEye, rEye);
    }
};

#endif // TEYES_H
