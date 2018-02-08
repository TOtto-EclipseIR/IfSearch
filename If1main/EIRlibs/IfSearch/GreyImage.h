/** @file GreyImage.h
*	Public declaration of GreyImage class for DDT image library
*
*	@author	Dynamic DeZigns Technology
*/

#pragma once
#include <qglobal.h>
#ifdef DDTIMG_LIB
# define DDTIMG_EXPORT Q_DECL_EXPORT
#else
# define DDTIMG_EXPORT Q_DECL_IMPORT
#endif

#include <QImage>
#include <QPoint>
#include <QRect>
#include <QRgb>
#include <QSize>
#include <QVector>

namespace DDT
{
	class DDTIMG_EXPORT GreyImage
	{
	public:
		GreyImage(void);
		GreyImage(const QSize size, const int fill);
		GreyImage(const QSize size, const qreal fill = -1.0);
		~GreyImage();
		void clear(void)
			{ Size = QSize(); Pixels.clear(); }
		void fill(const QSize size, const qreal fill);
		bool isNull(void) const
			{ return Size.isEmpty(); }
		bool isBlack(void) const;
		int height(void) const
			{ return Size.height(); }
		int width(void) const
			{ return Size.width(); }
		QRect rect(void) const
			{ return QRect(QPoint(0, 0), Size); }
		QPoint center(void) const
			{ return rect().center(); }
		QSize size(void) const
			{ return Size; }
		int numPixels(void) const
			{ return Size.height() * Size.width(); }
		QVector<qreal> vector(void) const
			{ return Pixels; }
		const qreal * constData(void) const
			{ return Pixels.constData(); }
		qreal * data(void)
			{ return Pixels.data(); }
		const int numBytes(void) const
			{ return Size.height() * Size.width() * sizeof(qreal); }
		QByteArray bytes(void) const;
		QImage image(QImage::Format imageFormat=QImage::Format_Indexed8) const;
		qreal pixelF(const int x, const int y) const;
		void setPixelF(const int x, const int y, const qreal v);
		qreal pixelF(const QPoint point) const
			{ return pixelF(point.x(), point.y()); }
		int pixel(const int x, const int y) const
			{ return iPixel(pixelF(x, y)); }
		int pixel(const QPoint point) const
			{ return iPixel(pixelF(point.x(), point.y())); }
		void setPixel(const int x, const int y, const int v)
			{ setPixelF(x, y, pixel(v)); }
		void setPixelF(const QPoint point, const qreal v)
			{ setPixelF(point.x(), point.y(), v); }
		void setPixel(const QPoint point, const int v)
			{ setPixelF(point.x(), point.y(), pixel(v)); }

	public:
		static QVector<QRgb> greyColorTable(void);
		static qreal pixel(const int value)
			{ return (qreal)value / 255.0; }
		static qreal pixel(const QRgb rgb)
			{ return (qreal)qRed(rgb)   / 255.0 * redComponent
				   + (qreal)qGreen(rgb) / 255.0 * greenComponent
				   + (qreal)qBlue(rgb)  / 255.0 * blueComponent; }
		static int iPixel(const qreal value)
			{ return qBound(0, qRound(value * 255.0), 255); }

	private:
		QSize Size;
		QVector<qreal> Pixels;
		static QVector<QRgb> staticGreyColorTable;
		static qreal redComponent;
		static qreal greenComponent;
		static qreal blueComponent;
//		static QSet<GreyImage *> instances;
	}; // class GreyImage

} // namespace DDT

#include <QVariant>
Q_DECLARE_METATYPE(DDT::GreyImage);
