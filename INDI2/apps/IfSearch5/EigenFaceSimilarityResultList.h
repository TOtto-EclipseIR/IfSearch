#ifndef EIGENFACESIMILARITYRESULTLIST_H
#define EIGENFACESIMILARITYRESULTLIST_H
#include <qglobal.h>

#include <QtCore/QList>
#include "EigenFaceSimilarityResult.h"

class  EigenFaceSimilarityResultList
        : public QList<EigenFaceSimilarityResult>
{
public:
    EigenFaceSimilarityResultList();
};

#endif // EIGENFACESIMILARITYRESULTLIST_H
