/*! @file AbstractWorker.cpp Definitions for the AbstractWorker class.
  */
#include "AbstractWorker.h"

AbstractWorker::AbstractWorker(const Configuration & config,
                               const VariableSet & input)
    : _config(config)
    , input_vset(input)
    , minProgress_i(0)
    , progress_i(0)
    , maxProgress_i(0)
{
}

void AbstractWorker::getProgress(int * min, int * current, int * max)
{
    if (min)        *min = minProgress_i;
    if (current)    *current = progress_i;
    if (max)        *max = maxProgress_i;
}
/*
Results results(void) const
{
    return &Results(*this);
}
*/
