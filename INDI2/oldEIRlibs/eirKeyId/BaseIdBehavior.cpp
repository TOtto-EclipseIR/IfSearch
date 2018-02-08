#include "BaseIdBehavior.h"

#include <QMapIterator>

const QString BaseIdBehavior::lower("abcdefghijklmnopqrstuvwxyz");

BaseIdBehavior::BaseIdBehavior(const QString & allowed,
                               const QString & separator,
                               const QString & escaped,
                               const QString & ignored)
    : allowed_string(allowed)
    , separator_string(separator)
    , escaped_string(escaped)
    , ignored_string(ignored)
{
}

QString BaseIdBehavior::ingest(const QString & inString,
                               const bool isSection) const
{
    (void)isSection;
    QString s = inString.simplified();
    if ( ! replace_map.isEmpty())
    {
        QMapIterator<QString, QString> it(replace_map);
        while (it.hasNext())
        {
            it.next();
            s.replace(it.key(), it.value());
        }
    }
    return s;
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
