/*! \file Severity.cpp Definitions for Severity Enumeration
  *
  */


#include "Severity.h"

DEFINE_ENUMERATION(Severity, SEVERITY_ENUM)

QChar Severity::toChar(void) const
{
    static const QString chars("})|([~+.XXX <>-=!*?$&$$#%%@@@@{");
    return (value() >= 0 && value() < chars.size())
            ? chars.at(value())
            : QChar('?');
}

bool Severity::isFatal(void) const
{
    return value() >= Pointer;
}

bool Severity::isError(void) const
{
    return value() >= Error;
}

bool Severity::isWarning(void) const
{
    return value() >= ToDo;
}

