/*! \file AnyColor.cpp Definitions for AnyColor class
  *
  */

#include "AnyColor.h"

#include <math.h>

AnyColor::AnyColor(void) :
    model_(Not),
    count_(0)
{
    channel_[0] = channel_[1] = channel_[2] = channel_[3] = 0.0;
}

AnyColor::AnyColor(const ColorModel model) :
    model_(model),
    count_(0)
{
    channel_[0] = channel_[1] = channel_[2] = channel_[3] = 0.0;
}

AnyColor::AnyColor(const AnyColor & other) :
        model_(other.model_),
        count_(other.count_)
{
    channel_[0] = other.channel_[0],
    channel_[1] = other.channel_[1],
    channel_[2] = other.channel_[2],
    channel_[3] = other.channel_[3];
}

AnyColor::AnyColor(const ColorModel model, qreal f1, qreal f2, qreal f3, qreal f4) :
    model_(model),
    count_(0)
{
    channel_[0] = f1,
    channel_[1] = f2,
    channel_[2] = f3,
    channel_[3] = f4;
}

AnyColor::AnyColor(const ColorModel model, quint8 u1, quint8 u2, quint8 u3, quint8 u4) :
    model_(model),
    count_(0)
{
    channel_[0] = (qreal)u1 / 255.0,
    channel_[1] = (qreal)u2 / 255.0,
    channel_[2] = (qreal)u3 / 255.0,
    channel_[3] = (qreal)u4 / 255.0;
}

AnyColor::AnyColor(const ColorModel model, const AnyColor & other)
{
    AnyColor temp = other.convertTo(model);
    model_      = temp.model_,
    count_      = temp.count_,
    channel_[0] = temp.channel_[0],
    channel_[1] = temp.channel_[1],
    channel_[2] = temp.channel_[2],
    channel_[3] = temp.channel_[3];
}

AnyColor::AnyColor(const QRgb qrgb)
    : model_(RGB), count_(0)
{
    quint8 r = qRed(qrgb), g = qGreen(qrgb), b = qBlue(qrgb);
    channel_[0] = (qreal)r / 255.0,
    channel_[1] = (qreal)g / 255.0,
    channel_[2] = (qreal)b / 255.0,
    channel_[3] = 0.0;
}

AnyColor::AnyColor(const QColor & qcolor)
    : count_(0)
{
    switch (qcolor.spec())
    {
    case QColor::Invalid:
        channel_[0] = channel_[1] = channel_[2] = channel_[3] = 0.0,
        model_ = Not;
        break;

    case QColor::Rgb:
        channel_[0] = qcolor.redF(),
        channel_[1] = qcolor.greenF(),
        channel_[2] = qcolor.blueF(),
        channel_[3] = 0.0,
        model_ = RGB;
        break;

    case QColor::Hsl:
        channel_[0] = qcolor.hueF(),
        channel_[1] = qcolor.lightnessF(),
        channel_[2] = qcolor.saturationF(),
        channel_[3] = 0.0,
        model_ = HLS;
        break;

    case QColor::Hsv:
        channel_[0] = qcolor.hueF(),
        channel_[1] = qcolor.saturationF(),
        channel_[2] = qcolor.valueF(),
        channel_[3] = 0.0,
        model_ = HSV;
        break;

    default:
        channel_[0] = qcolor.convertTo(QColor::Rgb).redF(),
        channel_[1] = qcolor.convertTo(QColor::Rgb).greenF(),
        channel_[2] = qcolor.convertTo(QColor::Rgb).blueF(),
        channel_[3] = 0.0,
        model_ = RGB;
        break;
    }
}

bool AnyColor::isNull(void) const
{
    return Not == model_;
}

bool AnyColor::inRange(const AnyColor & min, const AnyColor & max) const
{
    AnyColor tempMin(model_, min), tempMax(model_, max);
    return channel_[0] >= tempMin.channel_[0] && channel_[0] <= tempMax.channel_[0]
        && channel_[1] >= tempMin.channel_[1] && channel_[1] <= tempMax.channel_[1]
        && channel_[2] >= tempMin.channel_[2] && channel_[2] <= tempMax.channel_[2]
        && channel_[3] >= tempMin.channel_[3] && channel_[3] <= tempMax.channel_[3];
} // inRange()

qreal AnyColor::distance(const AnyColor & other) const
{
    if (NULL == model_ || NULL == other.model_)
        return 4.0;
    AnyColor temp(model_, other);
    qreal result = 0.0;
    result += (channel_[0] - temp.channel_[0]) * (channel_[0] - temp.channel_[0]);
    if (HLS == model_ || HSV == model_)
        result /= 360.0 * 360.0;
    result += (channel_[1] - temp.channel_[1]) * (channel_[1] - temp.channel_[1]);
    result += (channel_[2] - temp.channel_[2]) * (channel_[2] - temp.channel_[2]);
    result += (channel_[3] - temp.channel_[3]) * (channel_[3] - temp.channel_[3]);
    return sqrt(result);
}

AnyColor AnyColor::complement(void) const
{
    AnyColor result;

    switch (model_)
    {
    case NULL:
        break; // return null result

    case HLS:
    case HSV:
        result = *this;
        result.setHue(180.0 - hue());
        result.model_ = model_;
        break;

    default:
        result.channel_[0] = 1.0 - channel_[0],
        result.channel_[1] = 1.0 - channel_[1],
        result.channel_[2] = 1.0 - channel_[2],
        result.channel_[3] = 1.0 - channel_[3];
        result.model_ = model_;
        break;
    }

    return result;
}

qreal AnyColor::red(void) const
{
    if (RGB != model_)
        return -1.0;
    else if (count_)
        return channel_[0] / (qreal)count_;
    else
        return channel_[0];
}

qreal AnyColor::green(void) const
{
    if (RGB != model_)
        return -1.0;
    else if (count_)
        return channel_[1] / (qreal)count_;
    else
        return channel_[1];
}

qreal AnyColor::blue(void) const
{
    if (RGB != model_)
        return -1.0;
    else if (count_)
        return channel_[2] / (qreal)count_;
    else
        return channel_[2];
}

qreal AnyColor::hue(void) const
{
    if (HSV != model_ && HLS != model_)
        return -1.0;
    else if (count_)
        return channel_[0] / (qreal)count_;
    else
        return channel_[0];
}

void AnyColor::setHue(qreal h)
{
    if (HSV != model_ && HLS != model_)
        ;
    else if (count_)
        channel_[0] = (qreal)count_ * h;
    else
        channel_[0] = h;
}

qreal AnyColor::lightness(void) const
{
    if (HLS != model_)
        return -1.0;
    else if (count_)
        return channel_[1] / (qreal)count_;
    else
        return channel_[1];
}


qreal AnyColor::saturation(void) const
{
    qreal f = -1.0;
    if (HSV == model_)		f = channel_[1];
    else if (HLS == model_)	f = channel_[2];
    else	return f;
    if (count_)	f /= count_;
    return f;
}

qreal AnyColor::value(void) const
{
    if (HSV != model_)
        return -1.0;
    else if (count_)
        return channel_[2] / (qreal)count_;
    else
        return channel_[2];
}

QString AnyColor::string(void) const
{
    QString result;
    switch (model_)
    {
    case Not:
        break;

    case RGB:
        result = QString("R%1:G%2:B%3")
                    .arg(qRound(channel_[0] * 1000), 3 ,10, QChar('0'))
                    .arg(qRound(channel_[1] * 1000), 3 ,10, QChar('0'))
                    .arg(qRound(channel_[2] * 1000), 3 ,10, QChar('0'));
        break;

    case HLS:
        result = QString("H%1:L%2:S%3").arg(qRound(channel_[0]), 3, 10, QChar('0'))
                                        .arg(channel_[1], 3, 'f', 3)
                                        .arg(channel_[2], 3, 'f', 3);
        break;

    case HSV:
    case CMYK:
        // TODO
        break;
    }
    return result;
} // string()

void AnyColor::add(const AnyColor & color)
{
    AnyColor temp = color.convertTo(model_);
    if (isNull() || color.isNull() || temp.isNull())
        return;
    channel_[0] += temp.channel_[0];
    channel_[1] += temp.channel_[1];
    channel_[2] += temp.channel_[2];
    channel_[3] += temp.channel_[3];
    ++count_;
} // add()

AnyColor AnyColor::average(void) const
{
    AnyColor result(*this);
    if (count_)
    {
        result.channel_[0] /= count_,
        result.channel_[1] /= count_,
        result.channel_[2] /= count_,
        result.channel_[3] /= count_;
        result.count_ = 0;
    }
    return result;
}

QRgb AnyColor::toQRgb(void) const
{
    AnyColor temp = convertTo(RGB);
    return qRgb(temp.red() * 255.0, temp.green() * 255.0, temp.blue() * 255.0);
} // toQRgb()

qreal AnyColor::grey(void) const
{
    switch (model_)
    {
    case HLS:       return lightness();
    case HSV:       return value();
    case RGB:       return 0.299 * red() + 0.587 * green() + 0.114 * blue();
    default:        return 0.0;
    }
}

quint8 AnyColor::grey8(void) const
{
    return qBound(0, qRound(255.0 * grey()), 255);
}

AnyColor AnyColor::convertTo(const ColorModel newModel) const
{
    if (model_ == newModel)
        return *this;
    else if (RGB == model_ && HLS == newModel)
    {
            qreal r = red(), g = green(), b = blue();
            qreal maxV = (r > g) ? ((r > b) ? r : b) : ((g > b) ? g : b);
            qreal minV = (r < g) ? ((r < b) ? r : b) : ((g < b) ? g : b);
            qreal diffV = maxV - minV;
            qreal sumV = maxV + minV;
            qreal h = 0.0, l = (maxV + minV) / 2.0, s = 0.0;
            if ( ! qIsNull(diffV))
            {
                    s = (l < 0.5) ? (diffV / sumV) : (diffV / (2 - sumV));
                    qreal diffH = 60.0 / diffV;
                    if (r == maxV)			h =         (g - b) * diffH;
                    else if (g == maxV)		h = 120.0 + (b - r) * diffH;
                    else					h = 240.0 + (r - g) * diffH;
                    if (h < 0.0)
                        h = h + 360.0;
            }
            return AnyColor(HLS, h, l, s);
    }
    else if (HLS == model_ && RGB == newModel)
    {
            qreal h = hue(), l = lightness(), s = saturation();
            qreal r, g, b;
            if (qIsNull(s))
                    r = g = b = l;
            else
            {
                    qreal p2 = (l < 0.5) ? l * (1 + s) : l + s - l * s;
                    qreal p1 = 2.0 * l - p2;
                    int ih = qRound(h) % 360 / 60;
                    qreal ah = (h - (qreal)ih * 60.0) / 60.0;
                    qreal v0 = p2;
                    qreal v1 = p1;
                    qreal v2 = p1 + (p2 - p1) * (1 - ah);
                    qreal v3 = p1 + (p2 - p1) * ah;
                    switch (ih)
                    {
                    case 0:		r = v0, g = v3, b = v1;	break;
                    case 1:		r = v2, g = v0, b = v1;	break;
                    case 2:		r = v1, g = v0, b = v3;	break;
                    case 3:		r = v1, g = v2, b = v0;	break;
                    case 4:		r = v3, g = v1, b = v0;	break;
                    case 5:		r = v0, g = v1, b = v2;	break;
                    default:	r = g = b = 0.0;		break;
                    }
            }
            return AnyColor(RGB, r, g, b);
    }
    // TODO
    else
        return AnyColor();
} // convertTo()
