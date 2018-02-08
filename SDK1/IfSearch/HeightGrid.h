#ifndef HEIGHTGRID_H
#define HEIGHTGRID_H

#include <QObject>
#include <QImage>
#include <QString>

#include <Eyes.h>
#include <QProperty.h>
#include <Matrix.h>

#define HEIGHTGRID_QPROPERTIES(TND) \
        TND(QString, GridFile, QString())   \
        TND(QString, GridImage, QString())   \
        TND(QString, GridEyes, QString())   \
        TND(int, GridRows, 0) \
        TND(int, GridCols, 0) \
        TND(int, MinConsistency, 0) \
        TND(int, HeightUnits, 25) \
        TND(int, HeightScale, 0) \
        TND(int, TargetEyePixels, 0) \
        TND(int, TargetHeight, 0) \
        TND(int, MinConfidence, 0) \

class HeightGrid : public QObject
{
    Q_OBJECT
    DECLARE_QPROPERTIES(HEIGHTGRID_QPROPERTIES)

public:
    explicit HeightGrid(QObject * parent=0);
    QImage initializeGrid(void);
    void clear(void);
    quint8 gridAt(int x, int y) const;
    void set(const Eyes eyeLocation, const QSize imageSize, const int consistency);
    qreal distance(void) const;
    int confidence(void) const;
    bool isMatch(void) const;
    QImage gridImage(QImage image) const;
    QPoint headTop(void) const
    { return EyeLocation.topOfHead(scale()); }
    int heightEstimate(void) const
    { return qRound((qreal)HeightEstimate_mm / (qreal)getHeightUnits()); }
    int rawHeight(void) const
    { return HeightEstimate_mm; }

private:
    bool useEyes(const Eyes eyes) const;

signals:
    void error(QString message);

public slots:

private:
    qreal scale(void) const
    { return getHeightScale() ? ((qreal)getHeightScale() / 100.0) : 1.0; }

private:
    Matrix<int> Grid;
    Eyes EyeLocation;
    QSize ImageSize;
    int Consistency;
    int HeightEstimate_mm;
};

#endif // HEIGHTGRID_H
