/*! @file AbstractWorker.h Declaration of the AbstractWorker class
  *
  */
#ifndef ABSTRACTWORKER_H
#define ABSTRACTWORKER_H

#include <eirBase/Severity.h>

/* Temporary stubs */
class Configuration { public: Configuration() {} };
class VariableSet { public: VariableSet() {} };
class Results { public: Results() {} int r; };



class AbstractWorker : public Results
{
public:
    AbstractWorker(const Configuration & config,
                   const VariableSet & input);
    virtual Severity process(void) = 0;
    void getProgress(int * min, int * current, int * max);
    Results results(void) const;


private:
    Configuration _config;
    VariableSet input_vset;
    int minProgress_i;
    int progress_i;
    int maxProgress_i;
};

#endif // ABSTRACTWORKER_H
