/*! \file QProperty.h Declarations for QProperty mechanism
*/

#ifndef QPROPERTY_H
#define QPROPERTY_H

#include <QVariant>

/*! Use the QProperty mechanism to specify run-time properties for a QObject

Specify an external type, a QVariant type, a name,
and a default value in the class' header file.
Three functions will be generated for each item:
void initName() which sets the value to its default,
void setName(Type Value) which sets it to the specified value,
and Type getName(void) const which retrieves the current value.
@code
#define TRANSFORM_QPROPERTIES(TVND) \
        TVND(qreal, double, Scale,   0.0) \
        TVND(qreal, double, Aspect,  0.0) \
        TVND(qreal, double, Rotate,  0.0) \
        TVND(RectangleF, QRectF, Crop,   QRectF()) \

class IJMMATHSHARED_EXPORT TransformProperties : public QObject
{
    Q_OBJECT
    DECLARE_QPROPERTIES(TRANSFORM_QPROPERTIES);

public:
    explicit TransformProperties(QObject *parent = 0);
    bool isNull(void) const;
    QTransform transform(void) const;

signals:

public slots:

};
@endcode

Use the following code in the ctor in the class' source file.
@code
TransformProperties::TransformProperties(QObject *parent) :
    QObject(parent)
{
    DEFINE_QPROPERTIES_CTORS(TRANSFORM_QPROPERTIES);
    setObjectName("TransformProperties");
}
@endcode
*/

#define QPROPERTY_DECLARE(TYPE, VAR, NAME, DFLT) \
    public: \
        void init##NAME(void)      { setProperty(#NAME, QVariant((VAR)DFLT)); } \
        void set##NAME(TYPE v)     { setProperty(#NAME, QVariant((VAR)v)); } \
        TYPE get##NAME(void) const { return (TYPE)property(#NAME).value<VAR>(); } \

#define QPROPERTY_CTOR(TYPE, VAR, NAME, DFLT) \
        setProperty(#NAME, QVariant((VAR)DFLT)); \

#define DECLARE_QPROPERTIES(DEF) \
        DEF(QPROPERTY_DECLARE) \

#define DEFINE_QPROPERTIES_CTORS(DEF) \
        DEF(QPROPERTY_CTOR) \

#endif // QPROPERTY_H
