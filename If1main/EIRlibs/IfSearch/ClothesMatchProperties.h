#ifndef CLOTHESMATCHPROPERTIES_H
#define CLOTHESMATCHPROPERTIES_H

#include <QObject>
#include <QtGui/QColor>
class QRect;

#include <Eyes.h>
#include <QProperty.h>
class CharacteristicColor;

#define CLOTHESMATCH_QPROPERTIES(TND) \
        TND(QString, UpperColor, QString())   \
        TND(QString, LowerColor, QString())   \
        TND(int, Shoulder, 0) \
        TND(int, Waist, 0) \
        TND(int, Ankle, 0) \
        TND(int, Width, 0) \
        TND(int, UnderCrop, 0) \
        TND(int, UpperConfidence, 0) \
        TND(int, LowerConfidence, 0) \

class ClothesMatchProperties : public QObject
{
    Q_OBJECT
    DECLARE_QPROPERTIES(CLOTHESMATCH_QPROPERTIES);

public:
    explicit ClothesMatchProperties(QObject * parent=0);
    qreal shoulder(void) const;
    qreal waist(void) const;
    qreal width(void) const;
    qreal ankle(void) const;
    qreal underCrop(void) const;

    QRect upperRect(const Eyes eyes) const;
    QRect lowerRect(const Eyes eyes) const;
    CharacteristicColor upperCharacteristicColor(void) const;
    CharacteristicColor lowerCharacteristicColor(void) const;

signals:

public slots:

private:

};

#endif // CLOTHESMATCHPROPERTIES_H
