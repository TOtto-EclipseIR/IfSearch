#ifndef HEIGHTGRID_H
#define HEIGHTGRID_H

#include <QObject>
#include <QImage>
#include <QString>

#include <Eyes.h>
#include <QProperty.h>
#include <Matrix.h>

class HeightGrid : public QObject
{
    Q_OBJECT

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
    { return qRound((qreal)HeightEstimate_mm / (qreal)HeightUnits); }
    int rawHeight(void) const
    { return HeightEstimate_mm; }

private:
    bool useEyes(const Eyes eyes) const;

signals:
    void error(QString message);

public slots:

private:
    qreal scale(void) const
    { return HeightScale ? ((qreal)HeightScale / 100.0) : 1.0; }

private:
    Matrix<int> Grid;
    Eyes EyeLocation;
    QSize ImageSize;
    int Consistency;
    int HeightEstimate_mm;

QString GridFile;
QString GridImage;
QString GridEyes;
int GridRows = 0;
int GridCols = 0;
int MinConsistency = 0;
int HeightUnits = 25;
int HeightScale = 0;
int TargetEyePixels = 0;
int TargetHeight = 0;
int MinConfidence = 0;

};

#endif // HEIGHTGRID_H
