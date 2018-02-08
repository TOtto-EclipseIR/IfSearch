/*! \file CharacteristicColor.h Declarations for CharacteristicColor class
  *
  */
#ifndef CHARACTERISTICCOLOR_H
#define CHARACTERISTICCOLOR_H

#include <QtCore/QVector>
#include <QtGui/QColor>
class QImage;

#include <Histogram.h>
#include <AnyColor.h>

/*! \class  CharacteristicColor
  * \brief  Representation of a very limited set of characteristic colors
  *
  */
class CharacteristicColor
{
private:
    enum enumCC { NullIndex = 0, Initialize,  };

public:
    CharacteristicColor(void);
    CharacteristicColor(const AnyColor color);
    CharacteristicColor(const QColor qcolor);
    CharacteristicColor(const QRgb qrgb);

    bool isNull(void) const;
    bool isValid(void) const;
    static bool isValid(unsigned char v);
    unsigned char value(void) const;
    void setValue(unsigned char v);
    QRgb toRgb(void) const;
    QString name(void) const;
    QString toString(void) const;
    QVector<quint8> adjacent(void) const;
    qreal distance(const QColor & qc) const;
    qreal hslDistance(const QColor & qc,
                      qreal hWeight=0.6,
                      qreal sWeight=0.2,
                      qreal lWeight=0.2,
                      qreal sThreshold=0.1) const;

    static QRgb process(const QRgb pixel);
    static QImage process(QImage input, const AnyColor skin=AnyColor());
    static Histogram<quint8> histogram(QImage input);
    static unsigned count(void);

private:
    CharacteristicColor(enum enumCC e);

private:
    unsigned char index_;
    const static CharacteristicColor initialized_;
    static QVector<CharacteristicColor> rgb444table_;
    static QVector<QRgb> rgbLookup_;
    static QVector<QString> names_;
    static QVector< QVector<quint8> > adjacent_;
};

#endif // CHARACTERISTICCOLOR_H
