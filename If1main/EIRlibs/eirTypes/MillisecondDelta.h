#ifndef MILLISECONDDELTA_H
#define MILLISECONDDELTA_H
#include "eirTypes.h"

#include <QtCore/QString>

class EIRTYPESSHARED_EXPORT MillisecondDelta
{
public:
    MillisecondDelta(void);
    MillisecondDelta(qint64 ms);
    bool isNull(void) const;
    operator qint64(void) const;
    QString toString(const QString & format=QString("hh:mm:ss.zzz"));

public:
    static const qint64 day_ms;

private:
    qint64 _ms;
};

#endif // MILLISECONDDELTA_H
