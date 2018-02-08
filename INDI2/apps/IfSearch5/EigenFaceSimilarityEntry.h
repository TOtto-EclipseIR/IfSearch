#ifndef EIGENFACESIMILARITYENTRY_H
#define EIGENFACESIMILARITYENTRY_H
#include <qglobal.h>

#include <QtCore/QSharedDataPointer>
#include <QtCore/QString>

#include "../../INDI2/EIRlibs/eirBase/DataProperty.h"

#define EFSIMILARITYENTRY_DATAPROPS(TND) \
    TND(int, Rank, 0) \
    TND(qreal, Distance, -1.0) \
    TND(int, Confidence, 0) \
    TND(int, PersonKey, 0) \
    TND(int, FaceKey1, 0) \
    TND(QString, FaceId1, QString()) \
    TND(int, FrameKey1, 0) \
    TND(QString, FrameId1, QString()) \
    TND(int, VectorKey1, 0) \
    TND(int, FaceKey2, 0) \
    TND(QString, FaceId2, QString()) \
    TND(int, FrameKey2, 0) \
    TND(QString, FrameId2, QString()) \
    TND(int, VectorKey2, 0) \

class EigenFaceTemplate;

class EigenFaceSimilarityEntryData : public QSharedData
{
    DECLARE_CHILD_DATAPROPS(EFSIMILARITYENTRY_DATAPROPS);
public:
    EigenFaceSimilarityEntryData(void)
    {
        DEFINE_DATAPROPS_CTORS(EFSIMILARITYENTRY_DATAPROPS);
    }
};

class  EigenFaceSimilarityEntry
{
    DECLARE_PARENT_DATAPROPS(EFSIMILARITYENTRY_DATAPROPS);
public:
    EigenFaceSimilarityEntry(void); // default c'tor
    EigenFaceSimilarityEntry(const EigenFaceSimilarityEntry &);
    EigenFaceSimilarityEntry &operator=(const EigenFaceSimilarityEntry &);
    EigenFaceSimilarityEntry(const qreal distance,
                             const EigenFaceTemplate & tpl1,
                             const EigenFaceTemplate & tpl2);
    ~EigenFaceSimilarityEntry();

private:
    QSharedDataPointer<EigenFaceSimilarityEntryData> data;
};


#endif // EIGENFACESIMILARITYENTRY_H
