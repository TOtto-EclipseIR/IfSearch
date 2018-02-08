#ifndef EIGENFACESIMILARITYRESULT_H
#define EIGENFACESIMILARITYRESULT_H
#include <qglobal.h>
#ifdef INDIEF_LIB
# define INDIEF_EXPORT Q_DECL_EXPORT
#else
# define INDIEF_EXPORT Q_DECL_IMPORT
#endif

#include <QtCore/QList>

#include "Property.h"
#include "EigenFaceSimilarityEntry.h"
#include "EigenFaceSearchTier.h"

#define EFSIMILARITYRESULT_PROPERTIES(TND) \
    TND(int, Rank, 0) \
    TND(qreal, Distance, -1.0) \
    TND(int, Confidence, 0) \
    TND(EigenFaceSearchTier, Tier, EigenFaceSearchTier::Unknown) \

class INDIEF_EXPORT EigenFaceSimilarityResult
{
    DECLARE_PROPERTIES(EFSIMILARITYRESULT_PROPERTIES)
public:
    EigenFaceSimilarityResult(void);
    EigenFaceSimilarityResult(const EigenFaceSimilarityEntry & entry);
    void calculate(void);
    EigenFaceSimilarityEntry first(void) const;

private:
    QList<EigenFaceSimilarityEntry> entry_list;
};

#endif // EIGENFACESIMILARITYRESULT_H
