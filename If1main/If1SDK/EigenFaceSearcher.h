#ifndef EIGENFACESEARCHER_H
#define EIGENFACESEARCHER_H
#include <qglobal.h>
#ifdef INDIEF_LIB
# define INDIEF_EXPORT Q_DECL_EXPORT
#else
# define INDIEF_EXPORT Q_DECL_IMPORT
#endif

#include <QtCore/QMultiHash>

#include "EigenFaceSearchResultList.h"
#include "EigenFaceSearchSettings.h"
#include "EigenFaceVector.h"

namespace DDT
{
    class Return;
};

namespace INDI
{
    class EigenFaceData;
    class EigenFaceParameters;
    class EigenFaceTemplate;
}

class EigenFaceSearchResultList;

class EigenFaceSearcher
{
public:
    EigenFaceSearcher(void);
    void clear(void);
    bool contains(const int faceKey) const;
    int size(void) const;
    QString deleteFace(const int faceKey);
    QString enroll(const INDI::EigenFaceTemplate & tpl,
                       const int faceKey,
                       const int personKey);
    EigenFaceSearchResultList search(const INDI::EigenFaceSearchSettings::Mode mode,
                       const INDI::EigenFaceTemplate & tpl,
                       INDI::EigenFaceSearchSettings * settings,
                       const QList<int> & faceKeyList=QList<int>());
    int bestConfidence(void) const;
    qreal leastDistance(void) const;

private:
    EigenFaceSearchResultList computeDistance(const QList<int> & faceKeyList,
                                              const INDI::EigenFaceTemplate & tpl,
                                              qreal maxDistance);
    EigenFaceSearchResultList rankResults(const EigenFaceSearchResultList & distanceList,
                                          const int maxFaces);
    EigenFaceSearchResultList collectPersons(const EigenFaceSearchResultList & personList);
    EigenFaceSearchResultList combineBySearchKey(const EigenFaceSearchResultList & personList,
                                                 INDI::EigenFaceSearchSettings * settings);
    EigenFaceSearchResultList combineBest(const EigenFaceSearchResultList & personList,
                                          INDI::EigenFaceSearchSettings * settings);
    EigenFaceSearchResultList combineByEnroll(const EigenFaceSearchResultList & personList,
                                                INDI::EigenFaceSearchSettings * settings);
    EigenFaceSearchResultList combineByVector(const EigenFaceSearchResultList & personList,
                                                INDI::EigenFaceSearchSettings * settings);
    EigenFaceSearchResultList combineByRank(const EigenFaceSearchResultList & personList,
                                            INDI::EigenFaceSearchSettings * settings);
    EigenFaceSearchResultList combineAll(const EigenFaceSearchResultList & distanceList,
                                         INDI::EigenFaceSearchSettings * settings);
    EigenFaceSearchResultList
        computePersonDistance(const EigenFaceSearchResultList personList,
                              const int minConfidence,
                              const int maxPersonFaces,
                              const int topRank);
    EigenFaceSearchResultList sortResults(const EigenFaceSearchResultList & combinedList,
                                          INDI::EigenFaceSearchSettings * settings);
    EigenFaceSearchResultList setFixedTiers(const EigenFaceSearchResultList & sortedList,
                                            INDI::EigenFaceSearchSettings * settings);

private:
    QMultiHash<int, INDI::EigenFaceVector> faceKey_vector_mhash;
    int bestConfidence_i;
    qreal leastDistance_r;
};

#endif // EIGENFACESEARCHER_H
