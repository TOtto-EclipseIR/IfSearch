/** @file EigenFaceSearchPerson.h
*
*	Public declarations for INDI project EigenFaceMatcher
*/

#ifndef EIGENFACESEARCHPERSON_H
#define EIGENFACESEARCHPERSON_H
#include <qglobal.h>
#ifdef INDIEF_LIB
# define INDIEF_EXPORT Q_DECL_EXPORT
#else
# define INDIEF_EXPORT Q_DECL_IMPORT
#endif

#include <QtCore/QList>

#include <eirBase/Property.h>

#include "EigenFaceSearchResult.h"
#include "EigenFaceSearchTier.h"



class INDIEF_EXPORT EigenFaceSearchPerson
{
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

public:
    int     Rank        = 0;
    int     PersonKey    = 0;
    qreal   Distance     = 999.9;
    int     Confidence =  0;
    EigenFaceSearchTier Tier = EigenFaceSearchTier::Unknown;
    int     BestFaceKey  = 0;
    qreal   BestDistance  = 999.9;


};

#endif // EIGENFACESEARCHPERSON_H
