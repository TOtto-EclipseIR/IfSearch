/*! @file Parser.h Declarations for Parser class
*
*/
#ifndef PARSER_H
#define PARSER_H
#include "eirType.h"

#include <QVariant>

class EIRTYPESHARED_EXPORT Parsed : public QVariant
{
public:
    Parsed(const QString & s);
    operator QVariant (void);
};

#endif // PARSER_H
