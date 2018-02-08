#include "EigenFaceSimilarityResult.h"

EigenFaceSimilarityResult::EigenFaceSimilarityResult(void)
{
    DEFINE_PROPERTIES_CTORS(EFSIMILARITYRESULT_PROPERTIES);
}

EigenFaceSimilarityResult::EigenFaceSimilarityResult(
        const EigenFaceSimilarityEntry & entry)
{
    DEFINE_PROPERTIES_CTORS(EFSIMILARITYRESULT_PROPERTIES);
    entry_list.append(entry);
    setDistance(entry.getDistance());
    setConfidence(entry.getConfidence());
}

void EigenFaceSimilarityResult::calculate(void)
{
    // todo: combine individual results
}

EigenFaceSimilarityEntry EigenFaceSimilarityResult::first(void) const
{
    return entry_list.first();
}
