#include "SkinDetector.h"

#include "ColorRangeWeightVector.inc"
#include "SkinColorTables.inc"

#include <QtGui/QBitmap>
#include <QtGui/QImage>
#include <QtGui/QPainter>

SkinDetector::SkinDetector(void) :
		method_(Null)
{
} // default c'tor

SkinDetector::SkinDetector(const Method method)
{
	initialize(method);
} // c'tor(method)

void SkinDetector::clear(void)
{
	method_ = Null;
	bits_.clear();
}

void SkinDetector::initialize(const Method method)
{
	clear();
	if (Null == method)
		return;

	ColorRangeWeightVector crwvSkin, crwvNon;
	if (Table == method)
	{
		crwvSkin.initialize(16, SkinColorTable);
		crwvNon.initialize(16,  NonSkinTable);
	}

	method_ = method;
	bits_.fill(false, 256 * 1024);
	unsigned x = 0;

	for (int r6 = 0; r6 < 256; r6+=4)
		for (int g6 = 0; g6 < 256; g6+=4)
			for (int b6 = 0; b6 < 256; b6+=4)
			{
				switch (method_)
				{
				case Simple:
					{
						int dRG = r6 - g6;
						int dGB = g6 - b6;
						int dBR = b6 - r6;
						if (qAbs(dRG) + qAbs(dGB) + qAbs(dBR) > 4)
							if (dRG > -8 && dGB > -8 && dRG < 96 && dGB < 96)
//								if (b6 > r6 + 96 && b6 > g6 + 96)
									bits_.setBit(x, true);
					}
					break;

				case Keyhole:
					// todo
					break;

				case Table:
					{
						AnyColor c(RGB, (quint8)r6, g6, b6);
						qreal skinWeight = crwvSkin.weight(c);
						qreal nonWeight  = crwvNon.weight(c);
						if (skinWeight >= nonWeight)
							bits_.setBit(x, true);
					}
					break;

				case Null:
				default:
					; // ignore
				}
				++x;
			} // for(b,g,r)
} // initialize(method)

bool SkinDetector::isSkin(AnyColor color) const
{
	AnyColor temp(RGB, color);
	unsigned rgb666 = qRound(temp.red()   * 63.0) << 12
					| qRound(temp.green() * 63.0) << 6
					| qRound(temp.blue()  * 63.0);
	return isSkinRGB666(rgb666);
} // isSkin(Color)

bool SkinDetector::isSkin(QColor qcolor) const
{
	QColor temp = qcolor.toRgb();
	unsigned rgb666 = qRound(temp.redF()   * 63.0) << 12
					| qRound(temp.greenF() * 63.0) << 6
					| qRound(temp.blueF()  * 63.0);
	return isSkinRGB666(rgb666);
} // isSkin(QColor)

bool SkinDetector::isSkin(QRgb qrgb) const
{
	unsigned rgb666 = qRed(qrgb)	>> 2 << 12
					| qGreen(qrgb)	>> 2 << 6
					| qBlue(qrgb)	>> 2;
	return isSkinRGB666(rgb666);
} // isSkin(QRgb)

bool SkinDetector::isSkinRGB666(unsigned rgb666) const
{
	return bits_.testBit(rgb666);
} // isSkin(rgb666)

QImage SkinDetector::masked(const QImage & image, const QImage & background) const
{
	QImage input;
	QImage result;

	if (QImage::Format_ARGB32 != image.format()
			&& QImage::Format_ARGB32_Premultiplied != image.format()
			&& QImage::Format_RGB888 != image.format()
			&& QImage::Format_RGB32 != image.format())
		input = image.convertToFormat(QImage::Format_ARGB32_Premultiplied);
	else
		input = image;

	if (background.isNull())
	{
		result = QImage(image.size(), QImage::Format_RGB32);
		result.fill(qRgb(64,64,64));
	}
	else
		result = background.scaled(image.size());

	unsigned w = input.width(), h = input.height();
	for (unsigned y = 0; y < h; ++y)
	{
		QRgb * pixel = (QRgb *)input.scanLine(y);
		for (unsigned x = 0; x < w; ++x)
		{
			if (isSkin(*pixel))
				result.setPixel(x, y, *pixel);
			++pixel;
		}
	}

	return result;
} // createMask()

