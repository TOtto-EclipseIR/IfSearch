/** @file GreyImage.cpp
*	Definition of GreyImage class for DDT image library
*
*	@author	Dynamic DeZigns Technology
*
*/


#include <GreyImage.h>

#include <QImage>
#include <QSet>

#include <InfoMacros.h>


QVector<QRgb> GreyImage::staticGreyColorTable;
qreal GreyImage::redComponent = 0.299;
qreal GreyImage::greenComponent = 0.587;
qreal GreyImage::blueComponent = 0.144;
//QSet<GreyImage *> GreyImage::instances;

GreyImage::GreyImage(void)
{
	qRegisterMetaType<GreyImage>("GreyImage");
//	instances.insert(this);
}
		
GreyImage::GreyImage(QSize size, const int fill)
: Size(QSize(size.width() & 0xFFFFFFFC, size.height() & 0xFFFFFFFC))
{
	qRegisterMetaType<GreyImage>("GreyImage");
//	instances.insert(this);
	if (fill < 0)
		Pixels.resize(Size.width() * Size.height());
	else
		Pixels.fill(pixel(fill), Size.width() * Size.height());
}
		
GreyImage::GreyImage(QSize size, const qreal fill)
: Size(QSize(size.width() & 0xFFFFFFFC, size.height() & 0xFFFFFFFC))
{
	qRegisterMetaType<GreyImage>("GreyImage");
//	instances.insert(this);
	if (fill < 0.0)
		Pixels.resize(Size.width() * Size.height());
	else
		Pixels.fill(fill, Size.width() * Size.height());
}

GreyImage::~GreyImage()
{
//	instances.remove(this);
//	DETAIL("GreyImage d'tor %1 instances left", instances.size());
}

QVector<QRgb> GreyImage::greyColorTable(void)
{
	if (staticGreyColorTable.isEmpty())
	{
		staticGreyColorTable.fill(0, 256);
		for (int i = 0; i < 256; ++i)
			staticGreyColorTable[i] = qRgb(i, i, i);
	}
	return staticGreyColorTable;
}
		
bool GreyImage::isBlack(void) const
{
	for (int x = 0; x < Pixels.size(); ++x)
		if ( ! qIsNull(Pixels[x]))
			return false;
	return true;
}
		
void GreyImage::fill(QSize size, const qreal fill)
{
	Size = QSize(size.width() & 0xFFFFFFFC, size.height() & 0xFFFFFFFC);
	Pixels.fill(fill, Size.width() * Size.height());
}
		
qreal GreyImage::pixelF(const int x, const int y) const
{
	if (x >= 0 && x < Size.width() && y >= 0 && y < Size.height())
		return Pixels.at(x + y * Size.width());
	else
		return 0.0;
}
		
void GreyImage::setPixelF(const int x, const int y, const qreal v)
{
	if (x >= 0 && x < Size.width() && y >= 0 && y < Size.height())
		Pixels[x + y * Size.width()] = v;
}
		
QByteArray GreyImage::bytes(void) const
{
	if (Size.isEmpty())
		return QByteArray();

	int n = Size.width() * Size.height();
	QByteArray ba;
	ba.resize(n);
	for (int i = 0; i < n; ++i)
		ba[i] = iPixel(Pixels[i]);
	return ba;
}

		
QImage GreyImage::image(QImage::Format imageFormat) const
{
	QImage outImage(size(), imageFormat);
	if (QImage::Format_Indexed8 == imageFormat)
	{
		outImage.setColorTable(greyColorTable());
		for (int y = 0; y < height(); ++y)
			for (int x = 0; x < width(); ++x)
			{
				int grey = pixel(x, y);
				outImage.setPixel(x, y, grey);
			}
	}
	else
	{
		for (int y = 0; y < height(); ++y)
			for (int x = 0; x < width(); ++x)
			{
				int grey = pixel(x, y);
				outImage.setPixel(x, y, qRgb(grey, grey, grey));
			}
	}
	return outImage;
}
