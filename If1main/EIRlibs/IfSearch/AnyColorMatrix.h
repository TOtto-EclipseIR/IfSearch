/*! \file AnyColorMatrix.h Declarations for AnyColorMatrix class
  *
  */
#ifndef ANYCOLORMATRIX_H
#define ANYCOLORMATRIX_H

#include <QImage>
#include <QSize>

#include <AnyColor.h>
#include <ColorModel.h>
#include <Matrix.h>

class AnyColorMatrix
{
public:
    AnyColorMatrix(void);
    AnyColorMatrix(const ColorModel model, const QSize size);
    AnyColorMatrix(const QSize size, const AnyColor fill);
    AnyColorMatrix(const ColorModel model, QImage image);
    void fill(const QSize size, const AnyColor fill)
    { Data.fill(fill, size); }
    QSize size(void) const
    { return Data.size(); }
    int width(void) const
    { return Data.width(); }

    AnyColorMatrix operator += (const AnyColorMatrix & other);
    QImage toImage(QImage::Format format=QImage::Format_RGB32) const;

private:
    Matrix<AnyColor> Data;
};

#endif // ANYCOLORMATRIX_H
