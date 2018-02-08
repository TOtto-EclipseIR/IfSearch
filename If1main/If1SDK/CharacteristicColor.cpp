#include "CharacteristicColor.h"

#include <math.h>

#include <QtGui/QImage>

#include "AnyColor.h"
#include "InfoMacros.h"
#include "SkinDetector.h"


QVector<CharacteristicColor> CharacteristicColor::rgb444table_;
QVector<QRgb> CharacteristicColor::rgbLookup_;
QVector<QString> CharacteristicColor::names_;
QVector< QVector<quint8> > CharacteristicColor::adjacent_;
const CharacteristicColor CharacteristicColor::initialized_(CharacteristicColor::Initialize);

CharacteristicColor::CharacteristicColor(void) : index_(NullIndex)
{
}

CharacteristicColor::CharacteristicColor(const QColor qcolor) : index_(NullIndex)
{
    if (qcolor.isValid())
        *this = CharacteristicColor(qcolor.rgb());
}

CharacteristicColor::CharacteristicColor(const QRgb qrgb)
{
    unsigned x = (qRed(qrgb)   >> 4) << 8
               | (qGreen(qrgb) >> 4) << 4
               |  qBlue(qrgb)  >> 4;
    *this = rgb444table_.at(x);
}

CharacteristicColor::CharacteristicColor(const AnyColor color) : index_(NullIndex)
{
    AnyColor hls = color.convertTo(HLS);
    if (hls.isNull())
        return;

    qreal h = hls.hue(), l = hls.lightness(), s = hls.saturation();

    if (s < 0.20)
    {
        if      (l < 0.125)		index_ = 1;		// black
        else if (l < 0.375)		index_ = 2;		// charcoal
        else if (l < 0.625)		index_ = 15;	// grey
        else if (l < 0.875)		index_ = 28;	// silver
        else					index_ = 41;	// white
    }
    else
    {
        int ih = qRound(h / 30.0);
        int il = (l < 0.333) ? 3 : ((l < 0.667) ? 16 : 29);
        index_ = il + ih % 12;
    }
}

CharacteristicColor::CharacteristicColor(enum enumCC e)
{
    (void)e;
    // static c'tor
    QVector<quint8> vec;
    unsigned x = 0;
    QString baseName[] =
    {
        "Red", "Orange", "Yellow", "Lime",
        "Green", "Green-Cyan", "Cyan", "Turquoise",
        "Blue", "Blue-Magenta", "Magenta", "Purple"
    };

    rgb444table_.resize(16*16*16);
    for (unsigned r4 = 0; r4 < 256; r4+=16)
        for (unsigned g4 = 0; g4 < 256; g4+=16)
            for (unsigned b4 = 0; b4 < 256; b4+=16)
            {
                QRgb qrgb = qRgb(r4, g4, b4);
                AnyColor color(qrgb);
                CharacteristicColor charcol(color);
                rgb444table_.replace(x++, charcol);
//				qDebug("r=%i g=%i b=%i x=%i v=%i", r4, g4, b4, x, charcol.value());
            }

    rgbLookup_.fill(0, 42);
    names_.resize(42);
    rgbLookup_.replace(0, qRgba(0,0,0,0));
    rgbLookup_.replace(1, qRgb(0,0,0));
    rgbLookup_.replace(2, qRgb(64,64,64));
    rgbLookup_.replace(15, qRgb(128,128,128));
    rgbLookup_.replace(28, qRgb(191,191,191));
    rgbLookup_.replace(41, qRgb(255,255,255));
    for (x = 0; x < 12; ++x)
    {
        AnyColor dark(HLS, 30.0 * (qreal)x, 0.25, 1.0);
        AnyColor medium(HLS, 30.0 * (qreal)x, 0.5, 1.0);
        AnyColor light(HLS, 30.0 * (qreal)x, 0.75, 1.0);
        rgbLookup_.replace(3+x, dark.toQRgb());
        rgbLookup_.replace(16+x, medium.toQRgb());
        rgbLookup_.replace(29+x, light.toQRgb());
        names_.replace(3+x, "Dark "+baseName[x]);
        names_.replace(16+x, baseName[x]);
        names_.replace(29+x, "Light "+baseName[x]);
    }
    names_.replace(0, "Null");
    names_.replace(1, "Black");
    names_.replace(2, "Charcoal");
    names_.replace(15, "Grey");
    names_.replace(28, "Silver");
    names_.replace(41, "White");
    names_.replace(3, "Brick");
    names_.replace(29, "Pink");
    names_.replace(4, "Jail");
    names_.replace(30, "Sun");
    names_.replace(5, "Brown");
    names_.replace(31, "Khaki");
    names_.replace(7, "Forest");
    names_.replace(33, "Wasabi");
    names_.replace(7, "Avocado");
    names_.replace(11, "Navy");
    names_.replace(37, "Sky");
    names_.replace(12, "Violet");
    names_.replace(38, "Lavender");
    names_.replace(13, "Blood");
    names_.replace(39, "Hot");
    names_.replace(14, "Maroon");

    vec.clear();
    adjacent_.fill(vec, count());
    // black is adjacent to charcoal and all dark colors
    vec << 2 << 3 << 4 << 5 << 6 << 7 << 8 << 9 << 10 << 11 << 12 << 13 << 14;
    adjacent_[1] = vec;
    vec.clear();
    // charcol is adjacent to black, grey, and all dark colors
    vec << 1 << 3 << 4 << 5 << 6 << 7 << 8 << 9 << 10 << 11 << 12 << 13 << 14 << 15;
    adjacent_[2] = vec;
    vec.clear();
    // brick (dark red) is adjacent to black, charcoal, jail (dark orange), maroon (dark purple), and red
    vec << 1 << 2 << 4 << 14 << 16;
    adjacent_[3] = vec;
    vec.clear();
    // red is adjacent to brick (dark red), pink (bright red), orange, purple, and grey
    vec << 3 << 29 << 17 << 27 << 15;
    adjacent_[16] = vec;
    vec.clear();
    // pink (bright red) is adjacent to red, sun(bright orange), bright purple, silver, and white
    vec << 16 << 30 << 40 << 28 << 41;
    adjacent_[29] = vec;
    for (x = 4; x <= 13; ++x)
    {
        vec.clear();
        vec << 1 << 2 << x-1 << x+1 << x+13;
        adjacent_[x] = vec;
        vec.clear();
        vec << 15 << x << x+12 << x+14 << x+26;
        adjacent_[x+13] = vec;
        vec.clear();
        vec << 28 << x+13 << x+25 << x+27 << 41;
        adjacent_[x+26] = vec;
    }
    vec.clear();
    // maroon (dark purple) is adjacent to black, charcoal, brick (dark red), blood (dark magenta), and purple
    vec << 1 << 2 << 3 << 13 << 27;
    adjacent_[14] = vec;
    vec.clear();
    // purple is adjacent to grey, magenta, red, maroon (dark purple), and bright purple
    vec << 15 << 26 << 16 << 14 << 40;
    adjacent_[27] = vec;
    vec.clear();
    // bright purple is adjacent to purple, pink (bright red), hot (bright magenta), silver, and white
    vec << 27 << 29 << 39 << 28 << 41;
    adjacent_[40] = vec;
}

bool CharacteristicColor::isNull(void) const
{
    return NullIndex == index_;
}

bool CharacteristicColor::isValid(void) const
{
    return index_ < rgbLookup_.size() && index_ > 0;
}

bool CharacteristicColor::isValid(unsigned char v)
{
    return v < rgbLookup_.size() && v > 0;
}

unsigned char CharacteristicColor::value(void) const
{
    return index_;
}

qreal CharacteristicColor::distance(const QColor & qc) const
{
    qreal result = 0.0;
    QColor us(toRgb());
    result += (us.redF()   - qc.redF())   * (us.redF()   - qc.redF());
    result += (us.greenF() - qc.greenF()) * (us.greenF() - qc.greenF());
    result += (us.blueF()  - qc.blueF())  * (us.blueF()  - qc.blueF());
    return sqrt(result / 3.0);
}
qreal CharacteristicColor::hslDistance(const QColor & qc,
                  qreal hWeight,
                  qreal sWeight,
                  qreal lWeight,
                  qreal sThreshold) const
{
    qreal h1, l1, s1, h2, l2, s2, result=0.0;
    if (qIsNull(hWeight + sWeight + lWeight))
        return -1.0;
    QColor(toRgb()).toHsl().getHslF(&h1, &s1, &l1);
    qc.toHsl().getHslF(&h2, &s2, &l2);
    TRACE("Source: H%2 S%3 L%4 %1", toString(), h1, s1, l1);
    TRACE("Target: H%2 S%3 L%4 %1", AnyColor(qc).string(), h2, s2, l2);
    if (s2 < sThreshold)
        result = qAbs(l1-l2);
    else if (s1 < sThreshold)
    {
        result += sWeight * (s1-s2) * (s1-s2);
        result += lWeight * (l1-l2) * (l1-l2);
        result = sqrt(result / (sWeight + lWeight));
    }
    else
    {
        qreal dh = qAbs(h1-h2);
        if (dh > 0.5) dh = 1.0 - dh;
        result += hWeight * dh * dh;
        result += sWeight * (s1-s2) * (s1-s2);
        result += lWeight * (l1-l2) * (l1-l2);
        result = sqrt(result / (hWeight + sWeight + lWeight));
    }
    TRACE("Distance=%1", result);
    return result;
}

void CharacteristicColor::setValue(unsigned char v)
{
        index_ = isValid(v) ? v : (unsigned char)NullIndex;
}

QRgb CharacteristicColor::toRgb(void) const
{
    return isValid() ? rgbLookup_.at(index_) : qRgba(0,0,0,0);
}

QString CharacteristicColor::toString(void) const
{
    QRgb qrgb = toRgb();
    return QString("#%1%2%3").arg(qRed(qrgb),   2, 16, QChar('0'))
                             .arg(qGreen(qrgb), 2, 16, QChar('0'))
                             .arg(qBlue(qrgb),  2, 16, QChar('0')).toUpper();
}

QString CharacteristicColor::name(void) const
{
    return isValid() ? names_.at(index_) : QString();
}

unsigned CharacteristicColor::count(void)
{
    return qMin(rgbLookup_.size(), names_.size());
}

QVector<quint8> CharacteristicColor::adjacent(void) const
{
    return isValid() ? adjacent_.at(index_) : QVector<quint8>();
}

QRgb CharacteristicColor::process(const QRgb pixel)
{
    CharacteristicColor cc(pixel);
    return cc.toRgb();
}

QImage CharacteristicColor::process(QImage input, const AnyColor skin)
{
    if (QImage::Format_ARGB32 != input.format()
            && QImage::Format_ARGB32_Premultiplied != input.format()
            && QImage::Format_RGB888 != input.format()
            && QImage::Format_RGB32 != input.format())
        input = input.convertToFormat(QImage::Format_ARGB32_Premultiplied);

    unsigned h = input.height(), w = input.width();
    QImage output(w, h, QImage::Format_Indexed8);
    output.setColorTable(rgbLookup_);
    if ( ! skin.isNull())
        output.setColor(0, skin.toQRgb());

    for (unsigned y = 0; y < h; ++y)
        for (unsigned x = 0; x < w; ++x)
        {
            QRgb pixel = input.pixel(x, y);
            if ( ! skin.isNull() && skin.distance(pixel) < 0.3)
                output.setPixel(x, y, 0);
            else
            {
                CharacteristicColor cc(pixel);
                output.setPixel(x, y, cc.value());
            }
        }

    return output;
}

Histogram<quint8> CharacteristicColor::histogram(QImage input)
{
    Histogram<quint8> results(count(), false);

    if (QImage::Format_Indexed8 != input.format()
            && rgbLookup_ != input.colorTable())
        input = process(input);

    unsigned h = input.height(), w = input.width();
    for (unsigned y = 0; y < h; ++y)
        for (unsigned x = 0; x < w; ++x)
        {
            quint8 index = input.pixelIndex(x, y);
            if (index)
                results.increment(index);
        }
    results.enableUpdates();

    return results;
} // histogram()
