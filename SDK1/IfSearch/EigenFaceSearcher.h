#ifndef EIGENFACESEARCHER_H
#define EIGENFACESEARCHER_H
#include <qglobal.h>

#include <QtCore/QMultiHash>

#include "EigenFaceSearchResultList.h"
#include "EigenFaceSearchSettings.h"
#include "EigenFaceVector.h"

class Return;

class EigenFaceSearchResultList;
class EigenFaceTemplate;

class EigenFaceSearcher
{
public:
    EigenFaceSearcher(void);
    void clear(void);
    bool contains(const int faceKey) const;
    int size(void) const;
    QString deleteFace(const int faceKey);
    void enroll(const EigenFaceTemplate & tpl,
                       const int faceKey,
                       const int personKey);
    EigenFaceSearchResultList search(const EigenFaceSearchSettings::Mode mode,
                       const EigenFaceTemplate & tpl,
                       EigenFaceSearchSettings * settings,
                       const QList<int> & faceKeyList=QList<int>());
    int bestConfidence(void) const;
    qreal leastDistance(void) const;

private:
    EigenFaceSearchResultList computeDistance(const QList<int> & faceKeyList,
                                              const EigenFaceTemplate & tpl,
                                              qreal maxDistance,
                                              qreal minDistance);
    EigenFaceSearchResultList rankResults(const EigenFaceSearchResultList & distanceList,
                                          const int maxFaces);
    EigenFaceSearchResultList collectPersons(const EigenFaceSearchResultList & personList);
    EigenFaceSearchResultList removeDuplicatePersons(const EigenFaceSearchResultList & personList,
                                                 EigenFaceSearchSettings * settings);
    EigenFaceSearchResultList combineBySearchKey(const EigenFaceSearchResultList & personList,
                                                 EigenFaceSearchSettings * settings);
    EigenFaceSearchResultList combineBest(const EigenFaceSearchResultList & personList,
                                          EigenFaceSearchSettings * settings);
    EigenFaceSearchResultList combineByEnroll(const EigenFaceSearchResultList & personList,
                                                EigenFaceSearchSettings * settings);
    EigenFaceSearchResultList combineByVector(const EigenFaceSearchResultList & personList,
                                                EigenFaceSearchSettings * settings);
    EigenFaceSearchResultList combineByRank(const EigenFaceSearchResultList & personList,
                                            EigenFaceSearchSettings * settings);
    EigenFaceSearchResultList combineAll(const EigenFaceSearchResultList & distanceList,
                                         EigenFaceSearchSettings * settings);
    EigenFaceSearchResultList
        computePersonDistance(const EigenFaceSearchResultList personList,
                              const int minConfidence,
                              const int maxPersonFaces,
                              const int topRank);
    EigenFaceSearchResultList sortResults(const EigenFaceSearchResultList & combinedList,
                                          EigenFaceSearchSettings * settings);
    EigenFaceSearchResultList setFixedTiers(const EigenFaceSearchResultList & sortedList,
                                            EigenFaceSearchSettings * settings);

private:
    QMultiHash<int, EigenFaceVector> faceKey_vector_mhash;
    int bestConfidence_i;
    qreal leastDistance_r;
};

#endif // EIGENFACESEARCHER_H
