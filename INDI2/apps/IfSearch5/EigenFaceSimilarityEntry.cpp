#include "EigenFaceSimilarityEntry.h"
#include <QSharedData>

#include "EigenFaceSearchSettings.h"
#include "EigenFaceTemplate.h"

EigenFaceSimilarityEntry::EigenFaceSimilarityEntry()
    : data(new EigenFaceSimilarityEntryData)
{
}

EigenFaceSimilarityEntry::EigenFaceSimilarityEntry(const EigenFaceSimilarityEntry &rhs)
    : data(rhs.data)
{
}

EigenFaceSimilarityEntry &
    EigenFaceSimilarityEntry::operator=(const EigenFaceSimilarityEntry &rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

EigenFaceSimilarityEntry::EigenFaceSimilarityEntry(const qreal distance,
                         const EigenFaceTemplate & tpl1,
                         const EigenFaceTemplate & tpl2)
    : data(new EigenFaceSimilarityEntryData)
{
    setDistance(distance);
    setConfidence(EigenFaceSearchSettings::confidence(distance));
    setFaceKey1(tpl1.getFaceKey());
    setFaceKey2(tpl2.getFaceKey());
    setFaceId1(tpl1.getFaceId());
    setFaceId2(tpl2.getFaceId());
}

EigenFaceSimilarityEntry::~EigenFaceSimilarityEntry()
{
}
