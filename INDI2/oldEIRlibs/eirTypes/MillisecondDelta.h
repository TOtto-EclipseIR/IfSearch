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
    qint64 msec(void) const;
    void clear(void);
    qint64 setToMinutes(qreal mins);
    qint64 addMinutes(qreal mins);
    qint64 setHours(qreal hrs);
    qint64 addHours(qreal hrs);
    QString toString(const QString & format=QString("hh:mm:ss.zzz"));

public:
    static const qint64 second_ms;
    static const qint64 minute_ms;
    static const qint64 hour_ms;
    static const qint64 day_ms;
    static const qint64 week_ms;

private:
    qint64 _ms;
};

#endif // MILLISECONDDELTA_H
