#ifndef ISTRIGGER_H
#define ISTRIGGER_H
#include "eirCore.h"

class EIRCORESHARED_EXPORT IsTrigger
{
public:
    IsTrigger(const QVariant value);
    operator bool(void) const;

private:
    bool truth;
};

#endif // ISTRIGGER_H
