#ifndef EIGENFACESEARCHRESULTLIST_H
#define EIGENFACESEARCHRESULTLIST_H
#include <qglobal.h>

#include <QtCore/QList>

#include "EigenFaceSearchPerson.h"
#include "EigenFaceSearchTier.h"

class EigenFaceSearchResultList
        : public QList<EigenFaceSearchPerson>
{
public:
    EigenFaceSearchResultList(void);
    EigenFaceSearchResultList(const EigenFaceSearchPerson & first);
    int totalVectors(void) const;
    int bestConfidence(void) const;
    qreal leastDistance(void) const;
    EigenFaceSearchTier bestTier(void) const;
    void setBests(void);
    void setEachTier(const EigenFaceSearchTier tier);

private:
    int bestConfidence_i;
    qreal leastDistance_r;
    EigenFaceSearchTier best_tier;
};

#endif // EIGENFACESEARCHRESULTLIST_H
