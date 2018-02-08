/** @file EigenFaceTemplate.h
*
*	Public declarations for INDI project EigenFace template
*/

#pragma once
#include <qglobal.h>
#ifdef INDIEF_LIB
# define INDIEF_EXPORT Q_DECL_EXPORT
#else
# define INDIEF_EXPORT Q_DECL_IMPORT
#endif


class QDomDocument;
class QDomElement;
class QImage;
#include <QMultiMap>

#include <EigenFaceVector.h>

namespace DDT
{
class Return;
}

namespace INDI
{
class INDIEF_EXPORT EigenFaceTemplate
{
    friend class iEigenFaceMatcherMemory;
    friend class EigenFaceGenerator;

public:
    static const QString eigenFaceTemplateTagName;
    static const QString eigenFaceVectorTagName;
    enum { StreamId=0x45465450 };

public:
    EigenFaceTemplate(void);
    EigenFaceTemplate(const QByteArray & ba);
    ~EigenFaceTemplate();

    bool isValid(void) const;
    int consistency(void) const;
    DDT::Return read(const QDomElement & de);
    DDT::Return read(const QByteArray & ba);
    DDT::Return read(QIODevice * io);
    DDT::Return read(const QImage & image);
    DDT::Return read(const QString & imageFilename);
    DDT::Return write(QDomElement * de) const;
    DDT::Return write(QDomDocument * doc) const;
    DDT::Return write(QByteArray * ba) const;
    DDT::Return write(QIODevice * io) const;
    QStringList idList(void) const;
    void setVectorId(const QString & id);
    void removeById(const QString & id);

    void clear(void)
    { distance_efVector_mmap.clear(); }
    void add(const EigenFaceTemplate & other);
    void insert(qreal dist, const EigenFaceVector & vec)
    { distance_efVector_mmap.insert(dist, vec); }
    bool isEmpty(void) const
    { return distance_efVector_mmap.isEmpty(); }
    int size(void) const
    { return distance_efVector_mmap.size(); }
    QMultiMap<qreal, EigenFaceVector> vectorMap(void) const
    { return distance_efVector_mmap; }
    EigenFaceVector firstVector(const int x) const
    { return distance_efVector_mmap.isEmpty()
                ? EigenFaceVector()
                : distance_efVector_mmap.value(distance_efVector_mmap.keys().first()); }

private:
    QMultiMap<qreal, EigenFaceVector> distance_efVector_mmap;
}; // EigenFaceTemplate
} // INDI
