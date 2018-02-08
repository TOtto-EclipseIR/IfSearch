/** @file QQRect.cpp
*	Definition of Rectangle class for DDT math library
*
*	@author	Dynamic DeZigns Technology
*
*/

#include <QQRect.h>


void QQRect::matchAspect(const QSize sz)
{
	qreal widthRatio = (qreal)width() / (qreal)sz.width();
	qreal heightRatio = (qreal)height() / (qreal)sz.height();
	qreal avgRatio = (widthRatio + heightRatio) / 2.0;
	setSize(QSize(sz.width() * avgRatio, sz.height() * avgRatio));
} // matchAspect()

QQRect QQRect::scaled(const qreal scale) const
{
	if (isNull() || qIsNull(scale))
		return QQRect();
	int newWidth = width() * scale;
	int newHeight = height() * scale;
	return QQRect(QSize(newWidth, newHeight), center());
}

QQRect QQRect::operator * (const qreal scale) const
{
	if (isNull() || qIsNull(scale))
		return QQRect();
	int newWidth = width() * scale;
	int newHeight = height() * scale;
	int newCenterX = center().x() * scale;
	int newCenterY = center().y() * scale;
	return QQRect(QSize(newWidth, newHeight), 
				  QPoint(newCenterX, newCenterY));
}

int QQRect::overlapArea(QRect other)
{
	QQRect overlap = intersected(other);
	return overlap.area();
}

bool QQRect::isNeighbor(QRect other, qreal factor)
{
	bool rtn = true;

	if (*this == other)
		return true;

	if (qAbs(left() - other.left()) > width() * factor)
		rtn = false;
	else if (qAbs(right() - other.right()) > width() * factor)
		rtn = false;
	else if (qAbs(top() - other.top()) > height() * factor)
		rtn = false;
	else if (qAbs(bottom() - other.bottom()) > height() * factor)
		rtn = false;

	return rtn;
}
