#ifndef COLORRANGEWEIGHTVECTOR_INC
#define COLORRANGEWEIGHTVECTOR_INC

#include "AnyColor.h"
#include <QtCore/QVector>
#include <math.h>

class ColorRangeWeightVector
{
public:
	void initialize(unsigned n, qreal * v);
	qreal weight(const AnyColor color);

private:
	QVector<AnyColor>	minColors_;
	QVector<AnyColor>	maxColors_;
	QVector<qreal>	weights_;
}; // ColorRangeWeightVector

void ColorRangeWeightVector::initialize(unsigned n, qreal * v)
{
	minColors_.reserve(n);
	maxColors_.reserve(n);
	weights_.reserve(n);
	while (n)
	{
		qreal meanR = *v++;
		qreal meanG = *v++;
		qreal meanB = *v++;
		qreal covR  = *v++;
		qreal covG  = *v++;
		qreal covB  = *v++;
		qreal weight= *v++;
		--n;

		qreal sdevR = sqrt(covR);
		qreal sdevG = sqrt(covG);
		qreal sdevB = sqrt(covB);

		qreal minR = (meanR - 3 * sdevR) / 255.0;
		qreal minG = (meanG - 3 * sdevG) / 255.0;
		qreal minB = (meanB - 3 * sdevB) / 255.0;
		qreal maxR = (meanR + 3 * sdevR) / 255.0;
		qreal maxG = (meanG + 3 * sdevG) / 255.0;
		qreal maxB = (meanB + 3 * sdevB) / 255.0;

		minColors_.append(AnyColor(RGB, minR, minG, minB));
		maxColors_.append(AnyColor(RGB, maxR, maxG, maxB));
		weights_.append(weight);
	}
} // initialize()

qreal ColorRangeWeightVector::weight(const AnyColor color)
{
	qreal result = 0.0;
	int n = weights_.size();
	while (n)
	{
		--n;
		if (color.inRange(minColors_.at(n), maxColors_.at(n)))
			result += weights_.at(n);
	}
	return result;
} // weight()

#endif // COLORRANGEWEIGHTVECTOR_INC
