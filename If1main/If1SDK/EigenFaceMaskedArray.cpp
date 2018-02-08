#include <EigenFaceMaskedArray.h>
#include <EigenFace.h>
#include <EigenFaceData.h>

#include <QColor>
#include <QImage>
#include <QString>
#include <QVector>

#include <Return.h>

#include <math.h>

using namespace INDI;


EigenFaceMaskedArray::EigenFaceMaskedArray(const EigenFaceData * data, int id)
    : efData(data), Id(id)
{
    clear();
} // c'tor

EigenFaceMaskedArray::EigenFaceMaskedArray(const EigenFaceData * data, const QByteArray & ba)
    : efData(data)
{
    clear();
    if (data && data->isValid())
    {
        EigenFaceMaskedArray efma(data);
        efma.read(ba);
        if (efma.isValid())
            *this = efma;
    }
} // c'tor

EigenFaceMaskedArray::~EigenFaceMaskedArray()
{
} // d'tor

void EigenFaceMaskedArray::clear(void)
{
    Id = 0;
    VectorKey = 0;
    minData = INT_MAX, maxData = INT_MIN;
    Residual = 0.0;
    data.clear();
}

void EigenFaceMaskedArray::fill(qreal v)
{
    clear();
    minData = maxData = v;
    Residual = 0.0;
    if (efData)
        data.fill(v, efData->activePixels());
} // fill()

bool EigenFaceMaskedArray::isValid(void) const
{
    if ( ! efData)
        return false;
    if (! efData->mask().isNull())
    {
        if ( ! efData->mask().isValid())
            return false;
        if (efData->mask().count() != data.count())
            return false;
    }
    return true;
}

// IEIGENFACE_PRELIMINARY
/*
EigenFaceMaskedArray::EigenFaceMaskedArray(EigenFaceData * EFdata, int id, int length, float * fp)
    : efData(EFdata), Id(id), minData(INT_MAX), maxData(INT_MIN)
{
    if ( ! efData || ! efData->mask().isValid() || length != efData->mask().count())
        return;

    data.resize(length);
    int x = 0;
    while (length)
    {
        data[x] = *fp;
        if (*fp < minData)	minData = *fp;
        if (*fp > maxData)	maxData = *fp;
        ++fp, --length, ++x;
    }
}
*/


qreal EigenFaceMaskedArray::subtract(const qreal subtrahend)
{
    int n = data.size();
    qreal sumSquares = 0.0;
    minData = INT_MAX, maxData = INT_MIN;
    for (int x = 0; x < n; ++x)
    {
        qreal v = data[x] - subtrahend;
        data[x] = v;
        if (v < minData)	minData = v;
        if (v > maxData)	maxData = v;
        sumSquares += v * v;
    }
    return sumSquares;
} // subtract(c)

void EigenFaceMaskedArray::divideBy(const qreal divisor)
{
    int n = data.size();
    minData = INT_MAX, maxData = INT_MIN;
    for (int x = 0; x < n; ++x)
    {
        qreal v = data[x] / divisor;
        data[x] = v;
        if (v < minData)	minData = v;
        if (v > maxData)	maxData = v;
    }
} // divideBy()

// subtract other from this, return sum of square of result
qreal EigenFaceMaskedArray::subtract(const EigenFaceMaskedArray & other)
{
    int n = data.size();
    qreal sumSquares = 0.0;
    minData = INT_MAX, maxData = INT_MIN;
    for (int x = 0; x < n; ++x)
    {
        qreal v = data[x] - other.data[x];
        data[x] = v;
        if (v < minData)	minData = v;
        if (v > maxData)	maxData = v;
        sumSquares += v * v;
    }
    return sumSquares;
} // subtract(other)

EigenFaceVector EigenFaceMaskedArray::generateVector(int maxLayers)
{
    EigenFaceVector outVector;
    outVector.setVectorKey(vectorKey());

    if ( ! maxLayers || maxLayers > efData->layerCount())
        maxLayers = efData->layerCount();
    outVector.reserve(maxLayers);
    qreal finalResidual = Residual;

    for (int i = 0; i < maxLayers; ++i)
    {
        EigenFaceMaskedArray layer = efData->layer(i);
        qreal dp = dotProduct(layer);
        outVector.setCoefficient(i, dp);
        finalResidual -= dp * dp;
    }
    outVector.setResidual(finalResidual);

    return outVector;
}


DDT::Return EigenFaceMaskedArray::reconstruct(const EigenFaceVector & vector, int numCoefficients)
{
    if ( ! efData || ! efData->mask().isValid())
        return DDT::Return(EigenFace::ReturnInvalidStructure, "BitMask");
    if ( ! vector.isValid())
        return DDT::Return(EigenFace::ReturnInvalidStructure, "Vector");

    if ( ! numCoefficients || numCoefficients > vector.count())
        numCoefficients = vector.count();
    if (numCoefficients > efData->layerCount())
        numCoefficients = efData->layerCount();

    int meanFaceId = vector.vectorKey() >> 8;
    EigenFaceMaskedArray meanFace = efData->meanFace(meanFaceId);
    if ( ! meanFace.isValid())
        return DDT::Return(EigenFace::ReturnInvalidStructure, "MeanFace");
    data = meanFace.data;

    for (int i = 0; i < numCoefficients; ++i)
    {
        qreal coef = vector.coefficient(i);
        EigenFaceMaskedArray layer = efData->layer(i);
        if ( ! layer.isValid())
            return DDT::Return(EigenFace::ReturnInvalidStructure, "Layer");
        add(layer, coef);
    }

    return DDT::Return();
} // reconstruct()


qreal EigenFaceMaskedArray::dotProduct(const EigenFaceMaskedArray & other) const
{
    int n = data.size();
    qreal dp = 0.0;
    for (int x = 0; x < n; ++x)
        dp += data[x] * other.data[x];
    return dp;
} // dotProduct()

void EigenFaceMaskedArray::add(const EigenFaceMaskedArray & other, qreal coefficient)
{
    int n = data.size();
    minData = INT_MAX, maxData = INT_MIN;
    for (int x = 0; x < n; ++x)
    {
        qreal v = data[x] + coefficient * other.data[x];
        data[x] = v;
        if (v < minData)	minData = v;
        if (v > maxData)	maxData = v;
    }
} // add()
