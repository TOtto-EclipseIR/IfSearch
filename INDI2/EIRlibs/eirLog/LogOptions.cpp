/*! @file LogOptions.cpp  Declaration of LogLevel enumeration
*
*/

#include "LogOptions.h"
DEFINE_ENUMERATION(LogOptions, LOGOPTIONS_ENUM);

#include <QUrlQuery>

int LogOptions::maskFrom(const QUrlQuery & query)
{
    LogOptions lo;
    QString optionString(query.queryItemValue("options"));
    return lo.parseFlags(optionString.isEmpty()
                        ? "UserOpts"
                        : optionString);
}
