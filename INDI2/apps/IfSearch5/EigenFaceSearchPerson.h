/** @file EigenFaceSearchPerson.h
*
*	Public declarations for INDI project EigenFaceMatcher
*/

#ifndef EIGENFACESEARCHPERSON_H
#define EIGENFACESEARCHPERSON_H
#include <qglobal.h>

#include <QtCore/QList>

#include <eirBase/Property.h>

#include "EigenFaceSearchResult.h"
#include "EigenFaceSearchTier.h"

#define SEARCHPERSON_PROPERTIES(TND) \
    TND(int,    Rank,        0) \
    TND(int,    PersonKey,   0) \
    TND(qreal,  Distance,    999.9) \
    TND(int,    Confidence,  0) \
    TND(EigenFaceSearchTier, Tier, EigenFaceSearchTier::Unknown) \
    TND(int,    BestFaceKey, 0) \
    TND(qreal,  BestDistance, 999.9) \


class  EigenFaceSearchPerson
{
    DECLARE_PROPERTIES(SEARCHPERSON_PROPERTIES);
public:
    EigenFaceSearchPerson(void);
    EigenFaceSearchPerson(const int personKey,
                          const int faceKey,
                          const int searchKey,
                          const int enrollVector,
                          const int searchVector,
                          const qreal distance,
                          const int confidence);
    void append(const EigenFaceSearchPerson & other);
    void append(const EigenFaceSearchResult & result);
    bool isEmpty(void) const;
    int size(void) const;
    void clearResults(void);
    bool contains(const int faceKey) const;
    void setFirstRank(const int rank);
    int bestFaceKey(void) const;
    QList<EigenFaceSearchResult> results(void) const;
    void setResults(const QList<EigenFaceSearchResult> & resultList);

private:
    QList<EigenFaceSearchResult> result_list;
};

#endif // EIGENFACESEARCHPERSON_H
