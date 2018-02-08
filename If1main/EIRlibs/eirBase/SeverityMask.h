#ifndef SEVERITYMASK_H
#define SEVERITYMASK_H
#include "eirBase.h"

#include "Severity.h"

class EIRBASESHARED_EXPORT SeverityMask
{
public:
    SeverityMask(void);
    SeverityMask(const quint32 mask);
    SeverityMask(QString names);

    bool isEmpty(void) const;
    bool is(const Severity sev) const;
    void clear(void);
    void set(const Severity sev,
             bool on=true);
    void set(const Severity from,
             const Severity to=Severity::Maximum,
             bool on=true);
    void set(QString names,
             const bool clearFirst=true);
    quint32 mask(void) const;
    QStringList nameList(void) const;
    QString names(const QString & join=QString(" ")) const;

private:
    quint32 mask_u32;
};

#endif // SEVERITYMASK_H
