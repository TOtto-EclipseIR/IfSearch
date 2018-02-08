#ifndef TRANSFORMPROPERTIES_H
#define TRANSFORMPROPERTIES_H

#include <QObject>
#include <QtCore/QRectF>
class QTransform;

#include <QProperty.h>

#define TRANSFORM_QPROPERTIES(TND) \
        TND(qreal, Scale,   0.0) \
        TND(qreal, Aspect,  0.0) \
        TND(qreal, Rotate,  0.0) \
        TND(QRectF, Crop,   QRectF()) \

class TransformProperties : public QObject
{
    Q_OBJECT
    DECLARE_QPROPERTIES(TRANSFORM_QPROPERTIES);

public:
    explicit TransformProperties(QObject *parent = 0);
    bool isNull(void) const;
    QTransform transform(void) const;
    int dimension(const QSizeF sz) const;

signals:

public slots:

};

#endif // TRANSFORMPROPERTIES_H
