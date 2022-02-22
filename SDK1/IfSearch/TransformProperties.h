#ifndef TRANSFORMPROPERTIES_H
#define TRANSFORMPROPERTIES_H

#include <QObject>
#include <QtCore/QRectF>
class QTransform;

#include <QProperty.h>

class TransformProperties : public QObject
{
    Q_OBJECT

public:
    explicit TransformProperties(QObject *parent = 0);

public:
    bool isNull(void) const;
    QTransform transform(void) const;
    int dimension(const QSizeF sz) const;
    qreal getScale() const;
    qreal getAspect() const;
    qreal getRotate() const;
    const QRectF &getCrop() const;

public:
    void setScale(qreal newScale);
    void setAspect(qreal newAspect);
    void setRotate(qreal newRotate);
    void setCrop(const QRectF &newCrop);

signals:
    void ScaleChanged(const qreal newScale);

private:
    qreal mScale=0.0;
    qreal mAspect=0.0;
    qreal mRotate=0.0;
    QRectF mCrop;

    Q_PROPERTY(qreal getScale READ getScale WRITE setScale NOTIFY ScaleChanged)
};

#endif // TRANSFORMPROPERTIES_H
