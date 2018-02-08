#include "SeverityMask.h"

#include <QtCore/QStringList>

#include "BaseLog.h"

SeverityMask::SeverityMask(void)
    : mask_u32(0)
{
}

SeverityMask::SeverityMask(const quint32 mask)
    : mask_u32(mask)
{
}

SeverityMask::SeverityMask(QString names)
    : mask_u32(0)
{
    set(names);
}


bool SeverityMask::isEmpty(void) const
{
    return 0 == mask_u32;
}


bool SeverityMask::is(const Severity sev) const
{
    return mask_u32 & (1 << sev.value());
}


void SeverityMask::clear(void)
{
    mask_u32 = 0;
}


void SeverityMask::set(const Severity sev,
         bool on)
{
    if (on)
        mask_u32 |= (1 << sev.value());
    else
        mask_u32 &= ~ (1 << sev.value());
}


void SeverityMask::set(const Severity from,
         const Severity to,
         bool on)
{
    Severity sev = from;
    do
    {
        set(sev, on);
        ++sev;
    }   while (sev <= to);
}


void SeverityMask::set(QString names, const bool clearFirst)
{
    if (clearFirst) clear();
    names.remove(QRegExp("![0-9A-Za-z -~]"));
    QStringList sl(names.simplified().split(" "));
    foreach(QString s, sl)
    {
        Severity sev;
        bool neg = false;

        if (s.startsWith('-'))
        {
            neg = true;
            s = s.mid(1);
        }

        if (s.contains('~'))
        {
            Severity min = Severity::Minimum;
            Severity max = Severity::Maximum;

            if (s.startsWith('~') && s.endsWith('~'))
                ;
            else if (s.startsWith('~'))
                max = Severity(s.mid(1));
            else if (s.endsWith('~'))
                min = Severity(s.left(s.size()-1));
            else
            {
                int x = s.indexOf('~');
                min = Severity(s.left(x-1));
                max = Severity(s.mid(x));
            }
            if (min.isValid() && max.isValid())
                set(min, max, ! neg);
        }
        else
        {
            sev = Severity(s);
            if (sev.isValid())  set(sev, ! neg);
        }
    }
}


quint32 SeverityMask::mask(void) const
{
    return mask_u32;
}


QStringList SeverityMask::nameList(void) const
{
    QStringList r;
    quint32 m = 1;
    for (Severity sev = Severity::Minimum;
         sev < Severity::Maximum;
         sev++)
    {
        if (mask_u32 & m)   r << sev.name();
        m <<= 1;
    }
    return r;
}



QString SeverityMask::names(const QString & join) const
{
    return nameList().join(join);
}

