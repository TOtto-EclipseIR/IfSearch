/*! \file AnyColor.h Declarations for Color class
  *
  */

#ifndef ANYCOLOR_H
#define ANYCOLOR_H

#include "ColorModel.h"

#include <QtCore/QString>
#include <QtGui/QColor>
#include <QtGlobal>

/*! \class  AnyColor
  * \brief  Representation of color in several models
  *
  */
class AnyColor
{
public:
    AnyColor(void);
    AnyColor(const AnyColor & other);
    AnyColor(const ColorModel model);
    AnyColor(const ColorModel model, qreal f1, qreal f2, qreal f3, qreal f4=0.0);
    AnyColor(const ColorModel model, quint8 u1, quint8 u2, quint8 u3, quint8 u4=0);
    AnyColor(const ColorModel model, const AnyColor & other);
    AnyColor(const QColor & qcolor);
    AnyColor(const QRgb qrgb);

//	Color & operator = (const Color & other);

    qreal red(void) const;
    qreal green(void) const;
    qreal blue(void) const;
    qreal hue(void) const;
    qreal lightness(void) const;
    qreal saturation(void) const;
    qreal value(void) const;
    void setHue(qreal h);

    bool isNull(void) const;
    bool is(const ColorModel model)
        { return model == model_; }
    bool inRange(const AnyColor & min, const AnyColor & max) const;
    qreal distance(const AnyColor & other) const;
    AnyColor complement(void) const;
    void clear(void);
    void add(const AnyColor & color);
    AnyColor average(void) const;
    AnyColor convertTo(const ColorModel model) const;
    qreal grey(void) const;
    quint8 grey8(void) const;
    QRgb toQRgb(void) const;
    QString string(void) const;
    qreal channel(const int index) const
    { return (index >= 0 && index < 4) ? channel_[index] : -1.0; }
    AnyColor operator += (const AnyColor & other)
    { add(other); return *this; }

private:
    ColorModel model_;
    qreal channel_[4];
    unsigned count_;
}; // Color
Q_DECLARE_TYPEINFO(AnyColor, Q_MOVABLE_TYPE);

#endif // ANYCOLOR_H
