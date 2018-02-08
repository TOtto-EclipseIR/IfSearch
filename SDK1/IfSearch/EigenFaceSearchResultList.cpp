#include "EigenFaceSearchResultList.h"

#include "EigenFaceSearchPerson.h"

EigenFaceSearchResultList::
    EigenFaceSearchResultList(void)
    : bestConfidence_i(0)
    , leastDistance_r(999.9)
{
}

EigenFaceSearchResultList::
    EigenFaceSearchResultList(const EigenFaceSearchPerson & first)
    : bestConfidence_i(0)
    , leastDistance_r(999.9)
    , best_tier(EigenFaceSearchTier::Unknown)
{
    append(first);
}

int EigenFaceSearchResultList::totalVectors(void) const
{
    int k = 0;
    foreach (EigenFaceSearchPerson efsp, mid(0))
        k += efsp.size();
    return k;
}

int EigenFaceSearchResultList::bestConfidence(void) const
{
    return bestConfidence_i;
}

qreal EigenFaceSearchResultList::leastDistance(void) const
{
    return leastDistance_r;
}

EigenFaceSearchTier EigenFaceSearchResultList::bestTier(void) const
{
    return best_tier;
}

void EigenFaceSearchResultList::setBests(void)
{
    if (isEmpty())  return;
    EigenFaceSearchPerson efsp = first();
    bestConfidence_i = efsp.getConfidence();
    leastDistance_r = efsp.getDistance();
    best_tier = efsp.getTier();
}

void EigenFaceSearchResultList::setEachTier(const EigenFaceSearchTier tier)
{
    if (isEmpty())  return;
    int n = size();
    for (int x = 0; x < n; ++x)
        (*this)[x].setTier(tier);
}
