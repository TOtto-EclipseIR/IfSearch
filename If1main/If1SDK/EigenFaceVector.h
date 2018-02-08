/** @file EigenFaceVector.h
*
*	Internal declarations for INDI project EigenFaceGenerator generation
*/

#pragma once
#include <qglobal.h>
#ifdef INDIEF_LIB
# define INDIEF_EXPORT Q_DECL_EXPORT
#else
# define INDIEF_EXPORT Q_DECL_IMPORT
#endif

class QDomElement;

#include <NamedEnum.h>

namespace DDT
{
class Return;
}

namespace INDI
{
enum EigenFaceVectorType
{
    Raw,
    Flipped,
    LightCorrected,
    LightCorrectedFlipped,
    FlipAverage,
    LightCorrectedFlipAverage,
}; // EigenFaceVectorType

class INDIEF_EXPORT EigenFaceVectorKey
{
public:
    EigenFaceVectorKey(EigenFaceVectorType Type=Raw, int MeanKey=0)
        : theType(Type), theMeanKey(MeanKey & 0x00FFFFFF) {}
    void clear(void) { theType = Raw, theMeanKey = 0; }
    EigenFaceVectorType type(void) const { return theType; }
    int meanId(void) const { return theMeanKey; }
    operator int(void) const { return theMeanKey << 8 || (int)theType; }
    int toInt(void) const { return theMeanKey << 8 || (int)theType; }
    void operator =(int i) { theType = (EigenFaceVectorType)(i & 0xFF), theMeanKey = i >> 8; }
    friend bool operator < (const EigenFaceVectorKey & lhs, const EigenFaceVectorKey & rhs)
    { return (lhs.theMeanKey << 8 || (int)lhs.theType) < (rhs.theMeanKey << 8 || (int)rhs.theType); }

private:
    EigenFaceVectorType theType;
    int theMeanKey;
}; // EigenFaceVectorId

class INDIEF_EXPORT EigenFaceVector
{
public:
    enum { StreamId=0x45465643 };

public:
    EigenFaceVector(void);
    EigenFaceVector(const QByteArray & ba);
    ~EigenFaceVector() {};
    void clear(void);
    DDT::Return read(const QDomElement & de);
    DDT::Return read(const QByteArray & ba);
    DDT::Return read(QIODevice * io);
    DDT::Return write(QDomElement * de) const;
    DDT::Return write(QByteArray * ba) const;
    DDT::Return write(QIODevice * io) const;
    qreal distance(const EigenFaceVector & other, const int max=0,
                   QVector<qreal> weights=QVector<qreal>());
    bool isValid(void) const
    { return true; } // TODO
    bool isValid(int ix) const
    { return ix >= 0 && ix < coefficients.size(); }
    int vectorKey(void) const
    { return VectorKey; }
    void setVectorKey(int key)
    { VectorKey = key; };
    QString id(void) const
    { return id_string; }
    void setId(const QString & id)
    { id_string = id; }
    void setKeys(const int faceKey, const int personKey=0)
    { FaceKey = faceKey, PersonKey = personKey; }
    int faceKey(void) const
    { return FaceKey; }
    void setFaceKey(const int v)
    { FaceKey = v; }
    int personKey(void) const
    { return PersonKey; }
    void setPersonKey(const int v)
    { PersonKey = v; }
    void reserve(int count)
    { coefficients.fill(0.0, count); };
    int count(void) const
    { return coefficients.count(); }
    qreal coefficient(int pos) const
    { return isValid(pos) ? coefficients[pos] : 0.0; }
    void setCoefficient(int pos, qreal coef)
    { if (isValid(pos)) coefficients[pos] = coef; }
    void setCoefficients(QVector<qreal> v)
    { coefficients = v; }
    void setResidual(qreal res)
    { Residual = res; }
    qreal residual(void) const
    { return Residual; }
    int consistency(void) const
    { return qIsNull(Residual) ? 0 : (int)(1000.0 / (1.0 + 8.0 * Residual)); }
    QVector<qreal> coefs(void) const
    { return coefficients; }
    QDataStream & operator<< (QDataStream & out) const;
    QDataStream & operator>> (QDataStream & in);

private:
    int	VectorKey;
    int PersonKey, FaceKey;
    qreal Residual;
    QVector<qreal> coefficients;
    QString id_string;

    friend QDataStream & operator>> (QDataStream & in, EigenFaceVector & vec);
    friend QDataStream & operator<< (QDataStream & out, const EigenFaceVector & vec);
}; // EigenFaceVector
} // INDI
