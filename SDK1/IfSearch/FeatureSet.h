/** @file FeatureSet.h
*	Public declaration of FeatureSet class for DDT image library
*
*	@author	Dynamic DeZigns Technology
*/

#pragma once
#include <qglobal.h>
#ifdef DDTCORE_LIB
# define DDTCORE_EXPORT Q_DECL_EXPORT
#else
# define DDTCORE_EXPORT Q_DECL_IMPORT
#endif

#include <QObject>
#include <QVariant>
#include <QVector>
class QDomDocument;
class QDomElement;

#include <NamedEnum.h>
#include <NamedArray.h>

    class Feature : public NamedEnum<Feature>
	{
		DECLARE_NAMEDENUM_CTORS(Feature)
		static const Feature ImageSource;
		static const Feature TimeStamp;
		static const Feature ImageSize;
		static const Feature Usage;

		static const Feature Method; // TODO: Remove eventually
		static const Feature HeadMethod;
		static const Feature Quality;
		static const Feature HeadBox;
		static const Feature HeadCenter;

		static const Feature EyeMethod;
		static const Feature LeftEye;
		static const Feature RightEye;
		static const Feature EyeLine;
		static const Feature EyeDistance;

		static const Feature TemplateMethod;
		static const Feature Consistency;
	}; // class Feature

	class DDTCORE_EXPORT FeatureSet : public NamedArray<Feature, QVariant>
	{
	public:
		FeatureSet(void);
		FeatureSet(const QDomElement & elementFeatureSet);
		void calculate(void);
		void translate(const QPoint center, const QSize size, qreal scale=0.0);
		void crop(const QRect rect);
		QString toString(void) const;
	}; // class FeatureSet

