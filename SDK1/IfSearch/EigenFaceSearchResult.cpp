#include "EigenFaceSearchResult.h"

EigenFaceSearchResult::EigenFaceSearchResult()
    : data(new EigenFaceSearchResultData)
{
}

EigenFaceSearchResult::EigenFaceSearchResult(const EigenFaceSearchResult &rhs)
    : data(rhs.data)
{
}

EigenFaceSearchResult &EigenFaceSearchResult::operator=(const EigenFaceSearchResult &rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

EigenFaceSearchResult::
    EigenFaceSearchResult(const int personKey,
                          const int faceKey,
                          const int searchKey,
                          const int enrollVector,
                          const int searchVector,
                          const qreal distance,
                          const int confidence)
    : data(new EigenFaceSearchResultData)
{
    setPersonKey(personKey);
    setFaceKey(faceKey);
    setSearchKey(searchKey);
    setEnrollVector(enrollVector);
    setSearchVector(searchVector);
    setDistance(distance);
    setConfidence(confidence);
}

EigenFaceSearchResult::~EigenFaceSearchResult()
{
}
