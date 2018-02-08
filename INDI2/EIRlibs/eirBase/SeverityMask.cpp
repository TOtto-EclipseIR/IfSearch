/*! \file SeverityMask.cpp Definitions for SeverityMask
  *
  */
#include "SeverityMask.h"

SeverityMask::SeverityMask(void)
    : EnumerationMask<Severity, quint32>()
{}

SeverityMask::SeverityMask(const quint32 mask)
    : EnumerationMask<Severity, quint32>(mask)
{}

SeverityMask::SeverityMask(QString names)
    : EnumerationMask<Severity, quint32>(names)
{}

