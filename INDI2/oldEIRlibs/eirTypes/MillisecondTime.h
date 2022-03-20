#ifndef MILLISECONDS_H
#define MILLISECONDS_H
#include "eirTypes.h"

#include <QtGlobal>
#include <QtCore/QDateTime>

class MillisecondDelta;

class MillisecondTime
{
public:
    MillisecondTime(void);
    MillisecondTime(qint64 ems);
    MillisecondTime(const QDateTime & dt);
    static MillisecondTime current(void);
    static MillisecondTime fromIso(const QString & isoDateTime);
    static MillisecondTime fromString(const QString & dateTime);
    bool isNull(void) const;
    bool isBaseNull(void) const;
    static bool isUtc(void);
    bool isValid(void) const;
    operator qint64(void) const;
    MillisecondTime & operator += (const MillisecondDelta d);
    MillisecondTime & operator -= (const MillisecondDelta d);
    void set(const MillisecondTime other);
    void setNull(void);
    static qint64 base(void);
    static MillisecondTime null(void);
    MillisecondDelta delta(MillisecondTime ems=current()) const;
    static MillisecondDelta deltaBase(MillisecondTime ems=current());
    QDateTime toDateTime(void) const;
    QString toString(const QString & format=QString()) const;
    static QString baseString(const QString & format=QString());

private:
    qint64 _ems;
    static const QDateTime base_dt;
    static const MillisecondTime base_mst;
    static const MillisecondDelta tz_msd;
    static const MillisecondTime null_mst;
};

#endif // MILLISECONDS_H
