#include "AbstractIdString.h"
#include "BaseIdBehavior.h"

AbstractIdString::AbstractIdString(BaseIdBehavior * behavior,
                                   QString string)
    : _behavior(behavior)
{
    set(string);
}


bool AbstractIdString::operator == (const AbstractIdString & other) const
{
    return sortable() == other.sortable();
}

bool AbstractIdString::isNull(void) const
{
    return QString::isEmpty() || ! _behavior;
}

bool AbstractIdString::isSectioned(void) const
{
    return ! _behavior->sectionSeparator().isEmpty();
}

void AbstractIdString::set(const QString & string)
{
    QString::operator = (_behavior->ingest(string));
}

void AbstractIdString::append(const QString & section)
{
    QString append = _behavior->ingest(section, true);
    if ( ! append.isEmpty())
        QString::operator += (_behavior->sectionSeparator() + append);
}

void AbstractIdString::append(const AbstractIdString & section)
{
    if ( ! section.isNull())
        QString::operator += (_behavior->sectionSeparator() + section);
}

void AbstractIdString::prepend(const QString & section)
{
    QString prepend = _behavior->ingest(section, true);
    if ( ! prepend.isEmpty())
        QString::prepend(prepend + _behavior->sectionSeparator());
}

QString AbstractIdString::sortable(void) const
{
    return QString::toLower();
}

QString AbstractIdString::sectionSeparator(void) const
{
    return _behavior->sectionSeparator();
}

QString AbstractIdString::section(int index, int repeatQutoed) const
{
    if (repeatQutoed)
    {
        QString s(QString::section(_behavior->sectionSeparator(), index, index));
        QStringList qsl;
        while (repeatQutoed--) qsl << s;
        return "\"" + qsl.join("\",\"") + "\"";
    }
    else
        return QString::section(_behavior->sectionSeparator(), index, index);
}

QStringList AbstractIdString::sectionList(int first, int last) const
{
    return QString::section(_behavior->sectionSeparator(), first, last)
                    .split(_behavior->sectionSeparator());
}

QString AbstractIdString::sections(int first, int last) const
{
    return QString::section(_behavior->sectionSeparator(), first, last);
}

int AbstractIdString::sectionCount(void) const
{
    if (isSectioned() && ! isNull())
        return QString::count(_behavior->sectionSeparator()) + 1;
    else
        return 0;
}

bool AbstractIdString::startsWith(const AbstractIdString & section) const
{
    return QString::startsWith(section + _behavior->sectionSeparator());
}
