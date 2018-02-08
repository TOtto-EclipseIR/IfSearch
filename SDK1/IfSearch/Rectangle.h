#ifndef RECTANGLE_H
#define RECTANGLE_H

/*! \class  TRectangle
  * \brief  Template class for Rectangle and RectangleF
  *
  * A center-size based rectangle class.
  *
  * \tparam SIZE    Line class (QSize or QSizeF)
  * \tparam POINT   Point class (QPoint or QPointF)
  * \tparam RECT    Rect class (QRect or QRectF)
  * \tparam VALUE   value class (int or qreal)
  */
#include "Rectangle.hh"

#include <QtCore/QPoint>
#include <QtCore/QRect>
#include <QtCore/QSize>
#include <QtCore/QPointF>
#include <QtCore/QRectF>
#include <QtCore/QSizeF>

/*! \class  Rectangle
  * \brief  A integer center-size rectangle
  * \see    TRectangle - template class declaration for details
  */
typedef TRectangle<QSize, QPoint, QRect, int> Rectangle;
Q_DECLARE_TYPEINFO(Rectangle, Q_MOVABLE_TYPE);

/*! \class  RectangleF
  * \brief  A float center-size rectangle
  * \see    TRectangle - template class declaration for details
  */
typedef TRectangle<QSizeF, QPointF, QRectF, qreal> RectangleF;
Q_DECLARE_TYPEINFO(RectangleF, Q_MOVABLE_TYPE);

#endif // RECTANGLE_H
