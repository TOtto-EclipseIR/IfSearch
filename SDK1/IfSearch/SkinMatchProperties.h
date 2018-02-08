#ifndef SKINMATCHPROPERTIES_H
#define SKINMATCHPROPERTIES_H




#include <QObject>

#include "AnyColor.h"

#include <QtCore/QPair>
#include <QtCore/QRectF>
#include <QtCore/QString>

#include <Eyes.h>

class SkinMatchProperties : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString TargetColor READ targetColor WRITE setTargetColor)
    Q_PROPERTY(unsigned MinConfidence READ minConfidence WRITE setMinConfidence)

private:
    QString TargetColor;
    unsigned MinConfidence;

public:
    explicit SkinMatchProperties(const QString & name, QObject * parent=0);
    QString targetColor(void) const { return TargetColor; }
    void setTargetColor(QString v);
    unsigned minConfidence(void) const { return MinConfidence; }
    void setMinConfidence(unsigned v) { MinConfidence = v; }
    QString name(void) const { return Name; }
    unsigned confidence(const AnyColor & other);
    AnyColor color(void) const { return matchColor; }

private:
    AnyColor matchColor;
    QString Name;

signals:

public slots:

};

#endif // SKINMATCHPROPERTIES_H
