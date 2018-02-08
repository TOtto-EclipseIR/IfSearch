/** @file FeatureSet.cpp
*	Definition of FeatureSet class for DDT image library
*
*	@author	Dynamic DeZigns Technology
*
*/

#include <FeatureSet.h>

#include <QQRect.h>
#include <NamedArray.h>

#include <QDateTime>
#include <QLine>
#include <QPoint>
#include <QRect>
#include <QtDebug>

#include <math.h>

using namespace DDT;

DECLARE_NAMEDARRAY_INSTANCE(Feature, QVariant)
DECLARE_NAMEDENUM_INSTANCE(Feature)

DECLARE_NAMEDENUM_ID(Feature, ImageSource)
DECLARE_NAMEDENUM_ID(Feature, ImageSize)
DECLARE_NAMEDENUM_ID(Feature, Usage)

DECLARE_NAMEDENUM_ID(Feature, Method)
DECLARE_NAMEDENUM_ID(Feature, HeadMethod)
DECLARE_NAMEDENUM_ID(Feature, TimeStamp)
DECLARE_NAMEDENUM_ID(Feature, Quality)
DECLARE_NAMEDENUM_ID(Feature, HeadBox)

DECLARE_NAMEDENUM_ID(Feature, EyeMethod)
DECLARE_NAMEDENUM_ID(Feature, LeftEye)
DECLARE_NAMEDENUM_ID(Feature, RightEye)
DECLARE_NAMEDENUM_ID(Feature, HeadCenter)
DECLARE_NAMEDENUM_ID(Feature, EyeLine)
DECLARE_NAMEDENUM_ID(Feature, EyeDistance)

DECLARE_NAMEDENUM_ID(Feature, TemplateMethod)
DECLARE_NAMEDENUM_ID(Feature, Consistency)

FeatureSet::FeatureSet(void) 
{ 
	set(Feature::TimeStamp, QDateTime::currentDateTime());
}

FeatureSet::FeatureSet(const QDomElement & elementFeatureSet) 
{ 
	fromDomElement(elementFeatureSet, "Variant");
	if (get(Feature::TimeStamp).isNull())
		set(Feature::TimeStamp, QDateTime::currentDateTime());
	if (get(Feature::HeadMethod).isNull()
			&& ! get(Feature::Method).isNull())
	{
		set(Feature::HeadMethod, get(Feature::Method));
		set(Feature::Method, QVariant());
	}
}

void FeatureSet::calculate(void)
{
	QPoint leftEye = get(Feature::LeftEye).toPoint();
	QPoint rightEye = get(Feature::RightEye).toPoint();
	QRect headBox = get(Feature::HeadBox).toRect();

	if ( ! leftEye.isNull() && ! rightEye.isNull())
	{
		QLine eyeLine(leftEye, rightEye);
		qreal f = eyeLine.dx() * eyeLine.dx() + eyeLine.dy() * eyeLine.dy();
		set(Feature::EyeLine, eyeLine);
		set(Feature::EyeDistance, (int)sqrt(f));
		QPoint c = (leftEye + rightEye) / 2;
		set(Feature::HeadCenter, c);
	}
	else if ( ! headBox.isNull())
	{
		set(Feature::HeadCenter, headBox.center());
	}

} // calculate()
		
void FeatureSet::translate(const QPoint center, const QSize size, qreal scale)
{
	QPoint leftEye = get(Feature::LeftEye).toPoint();
	QPoint rightEye = get(Feature::RightEye).toPoint();
	QRect headBox = get(Feature::HeadBox).toRect();
	QQRect rect(size, center);

	if ( ! leftEye.isNull() && ! rightEye.isNull())
	{
		leftEye -= rect.topLeft();
		rightEye -= rect.topLeft();
		if ( ! qIsNull(scale))
		{
			leftEye *= scale;
			rightEye *= scale;
		}
		set(Feature::LeftEye, leftEye);
		set(Feature::RightEye, rightEye);
	}

	if ( ! headBox.isNull())
	{
		QRect newHeadBox(headBox.topLeft() - rect.topLeft(), 
						 headBox.size() * (qIsNull(scale) ? 1.0 : scale));
		set(Feature::HeadBox, newHeadBox);
	}

	set(Feature::ImageSize, size);
	calculate();
} // translate()
		
void FeatureSet::crop(const QRect rect)
{
	QPoint leftEye = get(Feature::LeftEye).toPoint();
	QPoint rightEye = get(Feature::RightEye).toPoint();
	QRect headBox = get(Feature::HeadBox).toRect();

	if ( ! leftEye.isNull() && ! rightEye.isNull())
	{
		leftEye -= rect.topLeft();
		rightEye -= rect.topLeft();
		set(Feature::LeftEye, leftEye);
		set(Feature::RightEye, rightEye);
	}

	if ( ! headBox.isNull())
	{
		headBox.moveLeft(headBox.left() - rect.left());
		headBox.moveTop(headBox.top() - rect.top());
		set(Feature::HeadBox, headBox);
	}

	set(Feature::ImageSize, rect.size());
	calculate();
} // crop()

QString FeatureSet::toString(void) const
{
	QString rtn;
	for (int f = Feature::min(); f <= Feature::max(); f++)
	{
		QVariant v = get(f);
		if ( ! v.isNull())
			rtn += QObject::tr("%1 = %2\n").arg(Feature::name(f)).arg(v.toString());
	}
	return rtn;
} // toString()

