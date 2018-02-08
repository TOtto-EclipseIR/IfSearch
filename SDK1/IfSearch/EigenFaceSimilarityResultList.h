#ifndef EIGENFACESIMILARITYRESULTLIST_H
#define EIGENFACESIMILARITYRESULTLIST_H
#include <qglobal.h>
#ifdef INDIEF_LIB
# define INDIEF_EXPORT Q_DECL_EXPORT
#else
# define INDIEF_EXPORT Q_DECL_IMPORT
#endif

#include <QtCore/QList>
#include "EigenFaceSimilarityResult.h"

class INDIEF_EXPORT EigenFaceSimilarityResultList
        : public QList<EigenFaceSimilarityResult>
{
public:
    EigenFaceSimilarityResultList();
};

#endif // EIGENFACESIMILARITYRESULTLIST_H
