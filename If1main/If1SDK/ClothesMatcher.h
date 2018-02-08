#ifndef CLOTHESMATCHER_H
#define CLOTHESMATCHER_H

#include <QObject>
#include <QtGui/QImage>

#include <Eyes.h>
#include "CharacteristicColor.h"
#include "ClothesMatchProperties.h"
#include "AnyColor.h"

class  ClothesMatcher : public QObject
{
    Q_OBJECT

public:
    explicit ClothesMatcher(ClothesMatchProperties * parent);
    void setSkinColor(const AnyColor color) { skinColor_ = color; }
    void clear(void);
    QRect upperRect(void) const { return upperRect_; }
    QRect lowerRect(void) const { return lowerRect_; }
    QImage upperImage(void) const { return upperImage_; }
    QImage lowerImage(void) const { return lowerImage_; }
    CharacteristicColor upperPrimary(void) const;
    CharacteristicColor lowerPrimary(void) const;
    CharacteristicColor upperSecondary(void) const;
    CharacteristicColor lowerSecondary(void) const;
    QString upperColorNames(void) const;
    QString lowerColorNames(void) const;
//    bool isActive(void) const { return props_ && props_->isActive(); }
    int upperConfidence(void) const { return upperConfidence_; }
    int lowerConfidence(void) const { return lowerConfidence_; }
    bool isMatch(void) const;
    QString upperColorString(void) const;
    QString lowerColorString(void) const;


public slots:
    void setFace(const QImage & orig, const Eyes eyes);
    void calculate(void);

signals:
    void calculated(void);
    void error(QString message);

private:
    ClothesMatchProperties * props_;
    QImage	origImage_;
    Eyes	eyes_;
    AnyColor	skinColor_;
    QRect	upperRect_;
    QRect	lowerRect_;
    QImage	upperImage_;
    QImage	lowerImage_;
    unsigned upperCount_;
    unsigned lowerCount_;
    unsigned upperConfidence_;
    unsigned lowerConfidence_;
    Histogram<quint8> upperHistogram_;
    Histogram<quint8> lowerHistogram_;
    CharacteristicColor upperPrimary_;
    CharacteristicColor lowerPrimary_;
    CharacteristicColor upperSecondary_;
    CharacteristicColor lowerSecondary_;
};

#endif // CLOTHESMATCHER_H
