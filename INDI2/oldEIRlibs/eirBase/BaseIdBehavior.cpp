#include "BaseIdBehavior.h"

#include <QMapIterator>

const QString BaseIdBehavior::lower("abcdefghijklmnopqrstuvwxyz");
const QString BaseIdBehavior::upper("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
const QString BaseIdBehavior::number("0123456789");
const QString BaseIdBehavior::uriReserved("'();:@&=+$,/?#[]");
const QString BaseIdBehavior::fileReserved("\\/:*?\"<>|");
const QString BaseIdBehavior::fileSymbol("_-.#~+=");


BaseIdBehavior::BaseIdBehavior(const QString & separator,
                               const QString & allowed,
                               const QString & escaped,
                               const QString & ignored)
    : allowed_string(allowed)
    , separator_string(separator)
{
    foreach (QChar c, escaped)
        replace(QString(c), QString("%%1").arg(c.cell(), 2, 16, QChar('0')));
    foreach (QChar(c), ignored)
        replace(QString(c), QString());
}

QString BaseIdBehavior::ingest(const QString & inString,
                               const bool isSection) const
{
    QString s = inString.simplified();
    if (isSection) s.remove(separator_string);
    if ( ! replace_map.isEmpty())
    {
        QMapIterator<QString, QString> it(replace_map);
        while (it.hasNext())
        {
            it.next();
            s.replace(it.key(), it.value());
        }
    }
    if (allowed_string.isEmpty())
        return s;

    QString r;
    r.reserve(s.length());
    foreach (QChar c, s)
        if (allowed_string.contains(c))
            r += c;
    return r;
}

QString BaseIdBehavior::sectionSeparator(void) const
{
    return separator_string;
}

void BaseIdBehavior::replace(const QString & key,
                                const QString & with)
{
    replace_map.insert(key, with);
}
