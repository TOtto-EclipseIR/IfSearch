/*! @file EnumerationMask.h Declarations for EnumerationMask template
  *
  */
#ifndef ENUMERATIONMASK_H
#define ENUMERATIONMASK_H

#include <QStringList>

/*! @class  EnumerationMask
  * @brief  Specializes DualMap for case insensitive strings
  */
template <class ENUM, class UINT> class EnumerationMask
{
public:
    EnumerationMask(void) : u(0) {}
    EnumerationMask(const UINT mask) : u(mask) {}
    EnumerationMask(QString names) { set(names); }

    bool isEmpty(void) const { return 0 == u; }
    bool is(const ENUM sev) const
    { return u & (1 << sev.value()); }
    void clear(void) { u = 0; }
    UINT mask(void) const { return u; }
    QString names(const QString & join=QString(" ")) const
    { return nameList().join(join); }

    QStringList nameList(void) const
    {
        QStringList r;
        for (ENUM sev = ENUM::first();
             sev < ENUM::last();
             sev++)
        {
            if (is(sev))   r << sev.name();
        }
        return r;
    }

    void set(const ENUM sev, bool on=true)
    {
        if (on)
            u |= (1 << sev.value());
        else
            u &= ~ (1 << sev.value());
    }

    void set(const ENUM from, const ENUM to, bool on=true)
    {
        Severity sev = from;
        do
        {
            set(sev, on);
            ++sev;
        }   while (sev <= to);
    }

    void set(QString names, const bool clearFirst=true)
    {
        if (clearFirst) clear();
        names.remove(QRegExp("![0-9A-Za-z -~]"));
        QStringList sl(names.simplified().split(" "));
        foreach(QString s, sl)
        {
            ENUM e;
            bool neg = false;

            if (s.startsWith('-'))
            {
                neg = true;
                s = s.mid(1);
            }

            if (s.contains('~'))
            {
                ENUM min = ENUM(e.first());
                ENUM max = ENUM(e.last());

                if (s.startsWith('~') && s.endsWith('~'))
                    ;
                else if (s.startsWith('~'))
                    max = ENUM(s.mid(1));
                else if (s.endsWith('~'))
                    min = ENUM(s.left(s.size()-1));
                else
                {
                    int x = s.indexOf('~');
                    min = ENUM(s.left(x-1));
                    max = ENUM(s.mid(x));
                }
                if (min.isValid() && max.isValid())
                    set(min, max, ! neg);
            }
            else
            {
                e = ENUM(s);
                if (e.isValid())  set(e, ! neg);
            }
        }
    }

    static bool checkPrerequisites(void)
    {
        ENUM e;
        return e.last() < sizeof(UINT);
    }

private:
    UINT u;
};

#endif // ENUMERATIONMASK_H
