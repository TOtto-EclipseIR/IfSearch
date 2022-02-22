#include "HeightGrid.h"

#include <QFile>
#include <QPainter>
#include <QRectF>
#include <QTextStream>

#include "ImageMarker.h"
#include "Rectangle.h"

qreal interpolate(const qreal positionOfUnknown,
                  const qreal knownPosition1,
                  const qreal knownPosition2,
                  const qreal knownValue1,
                  const qreal knownValue2)
{
    qreal dV = knownValue1 - knownValue2;
    qreal dP = knownPosition1 - knownPosition2;
    qreal dU = knownPosition1 - positionOfUnknown;
    qreal s  = dV / dP;
    qreal v  = knownValue1 - dU * s;
    return v;
}

HeightGrid::HeightGrid(QObject *parent) :
    QObject(parent)
{
    setObjectName("HeightGrid");
    clear();
}

QImage HeightGrid::initializeGrid(void)
{
    int value;
    QString err;
    int gr = GridRows;
    int gc = GridCols;
    int un = HeightUnits;
    QString fn = GridFile;
    QFile file(fn);

    if (fn.isEmpty())
        err = "GridFile not specified";
    else if (gr < 2 || gc < 1 || gr > 255 || gc > 255)
        err = "Invalid Grid Size";
    else if ( ! un)
        err = "Units can not be zero";
    else if ( ! file.exists(fn))
        err = "GridFile does not exist: " + fn;
    else if ( ! file.open(QIODevice::ReadOnly | QIODevice::Text))
        err = "Error opening GridFile: " + fn;
    else
    {
        QTextStream in(&file);
        Grid.fill(0, QSize(gc, gr));
        for (int y = 0; y < gr; ++y)
            for (int x = 0; x < gc; ++x)
            {
                in >> value;
                Grid.set(x, y, un * value);
            }
    }

    QImage calibrationImage(GridImage);
    if (err.isEmpty() && ! calibrationImage.isNull())
    {
        QString gridEyePos(GridEyes);
        set(Eyes(gridEyePos), calibrationImage.size(), 999);
        calibrationImage = gridImage(calibrationImage);
    }

    if ( ! err.isEmpty())
        emit error(err);

    return calibrationImage;
} // initializeGrid()

void HeightGrid::clear(void)
{
    EyeLocation = Eyes();
    ImageSize = QSize();
    Consistency = 0;
    HeightEstimate_mm = 0;
} // clear()

quint8 HeightGrid::gridAt(int x, int y) const
{
    return Grid.at(x, y);
}

bool HeightGrid::useEyes(const Eyes eyes) const
{
#if 1
    return true;
#else
    int ed = eyes.distance();
    int te = TargetEyePixels();
    int et = 150;
    return ed > te * float(100.0 / et) && ed < te * et / 100;
#endif
}

void HeightGrid::set(const Eyes eyeLocation, const QSize imageSize, const int consistency)
{
    clear();
    EyeLocation = eyeLocation;
    ImageSize = imageSize;
    Consistency = consistency;
    if (consistency < MinConsistency)
        return;
    if ( ! useEyes(eyeLocation))
        return;
    if (Grid.isEmpty())
    {
        emit error("No Height Grid");
        return;
    }

    QSize cellPixel(imageSize.width() / GridCols,
                    imageSize.height() / GridRows);
    QPoint headTop = eyeLocation.topOfHead(scale());
    QPoint cellIndex(headTop.x() / cellPixel.width(),
                     headTop.y() / cellPixel.height());
    QRect cell(QPoint(cellIndex.x() * cellPixel.width(),
                      cellIndex.y() * cellPixel.height()),
               cellPixel);
    int htCell = Grid.at(cellIndex);
    int htAbove = Grid.at(cellIndex - QPoint(0, 1));
    int htBelow = Grid.at(cellIndex + QPoint(0, 1));
    if (0 == htCell || qAbs(headTop.y() - cell.center().y())
                                < cellPixel.height() / 12)
        HeightEstimate_mm = htCell;
    else if (headTop.y() < cell.center().y() && htAbove)
        // Interpolate above
        HeightEstimate_mm = qRound(interpolate(headTop.y(),
                                            cell.center().y(),
                                            cell.center().y()
                                                - cellPixel.height(),
                                            htCell, htAbove));
    else if (headTop.y() > cell.center().y() && htBelow)
        // Interpolate below
        HeightEstimate_mm = qRound(interpolate(headTop.y(),
                                            cell.center().y(),
                                            cell.center().y()
                                                + cellPixel.height(),
                                            htCell, htBelow));
    else if (htBelow)
        // Extrapolate from below
        HeightEstimate_mm = qRound(interpolate(headTop.y(),
                                            cell.center().y(),
                                            cell.center().y()
                                                + cellPixel.height(),
                                            htCell, htBelow));
    else if (htBelow)
        // Extrapolate from above
        HeightEstimate_mm = qRound(interpolate(headTop.y(),
                                            cell.center().y(),
                                            cell.center().y()
                                                - cellPixel.height(),
                                            htCell, htAbove));
    else
        HeightEstimate_mm = htCell;
} // set()

qreal HeightGrid::distance(void) const
{
    if (HeightEstimate_mm)
    {
        qreal dH = HeightEstimate_mm - TargetHeight * HeightUnits;
        qreal dE = EyeLocation.distance() - TargetEyePixels;
        qreal d  = sqrt(dH * dH) / 25.0 + sqrt(dE * dE);
        return d;
    }
    else
        return -1.0;
} // distance()

int HeightGrid::confidence(void) const
{
    qreal d = distance();
    return (d < 0.0) ? 0 : qBound(1, qRound(1000.0 / (1.0 + d / 10.0)), 999);
} // confidence()

bool HeightGrid::isMatch(void) const
{
    int c = confidence();
    return c > MinConfidence;
} // isMatch()

QImage HeightGrid::gridImage(QImage image) const
{
    int gr = GridRows;
    int gc = GridCols;
    int un = HeightUnits;
    int ep = EyeLocation.distance();

    ImageMarker marker(&image);
    QSize cellSize(image.width() / gc, image.height() / gr);
    marker.setPen(Qt::cyan);
    for (int y = 0; y < gr; ++y)
        for (int x = 0; x < gc; ++x)
        {
            QRectF r(QPoint(x * cellSize.width(), y * cellSize.height()),
                     cellSize);
            int g = Grid.at(x, y) / un;
            if (g)
                marker.drawText(r, Qt::AlignCenter, QString::number(g));
        }
    marker.setPen(Qt::red);
    for (int y = 1; y < gr; ++y)
        marker.drawLine(QPoint(0, y * cellSize.height()),
                        QPoint(image.width(), y * cellSize.height()));
    for (int x = 1; x < gc; ++x)
        marker.drawLine(QPoint(x * cellSize.width(), 0),
                        QPoint(x * cellSize.width(), image.height()));

    marker.eyes(EyeLocation, Qt::yellow);
    marker.horizontalLine(headTop(), ep * 2, Qt::yellow, 2);
    QString s;
    Rectangle box(QSize(120, 12), QPoint(0, -7) + headTop());
    if (HeightEstimate_mm)
        s = QString("%1 (%4mm) E%2 C%3")
                       .arg(heightEstimate())
                       .arg(ep)
                       .arg(confidence())
                       .arg(rawHeight());
    else
        s = QString("E%1").arg(ep);
    marker.setPen(isMatch() ? Qt::yellow : Qt::lightGray);
    marker.fillRect(box, Qt::black);
    marker.drawText(box, Qt::AlignTop | Qt::AlignHCenter, s);
    marker.end();
    return image;
} // gridImage()

