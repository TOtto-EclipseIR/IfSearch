/*! @file Readable.h Declarations for Readable class
*
*/
#ifndef HEXDUMP_H
#define HEXDUMP_H
#include "eirType.h"

#include <QStringList>

class EIRTYPESHARED_EXPORT Hexdump : public QStringList
{
public:
    Hexdump(const QVariant & var,
            const QString & title=QString());
    operator QStringList (void);

public:
    static QStringList from(const QByteArray &ba,
                            const QString & title);
};

#endif // HEXDUMP_H
