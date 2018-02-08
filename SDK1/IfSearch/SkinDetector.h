#ifndef SKINDETECTOR_H
#define SKINDETECTOR_H

#include "AnyColor.h"

#include <QBitArray>
#include <QImage>

class SkinDetector
{
public:
	enum Method { Null, Simple, Keyhole, Table };

public:
	SkinDetector(void);
	SkinDetector(const Method method);
	void initialize(const Method method);
	bool isNull(void) const
	{ return Null == method_ || bits_.isNull(); }
	void clear(void);
	bool isSkin(AnyColor color) const;
	bool isSkin(QColor qcolor) const;
	bool isSkin(QRgb qrgb) const;
	bool isSkinRGB666(unsigned rgb666) const;
	QImage masked(const QImage & image, const QImage & background=QImage()) const;

private:
	Method method_;
	QBitArray bits_;
};

#endif // SKINDETECTOR_H
