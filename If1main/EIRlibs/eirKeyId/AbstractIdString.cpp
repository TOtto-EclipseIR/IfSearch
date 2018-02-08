#include "AbstractIdString.h"
#include "BaseIdBehavior.h"

AbstractIdString::AbstractIdString(BaseIdBehavior * behavior,
                                   QString string)
    : _behavior(behavior)
{
    set(string);
}

AbstractIdString::AbstractIdString(BaseIdBehavior * behavior,
                                   const char * chars)
    : _behavior(behavior)
{
    set(QString(chars));
}

bool AbstractIdString::operator == (const AbstractIdString & other)
{
    return _string == other._string;
}

bool AbstractIdString::isNull(void) const
{
    return _string.isEmpty() || ! _behavior;
}

bool AbstractIdString::isSectioned(void) const
{
    return ! _behavior->sectionSeparator().isEmpty();
}

void AbstractIdString::set(const QString & string)
{
    _string = _behavior->ingest(string);
}

void AbstractIdString::append(const QString & section)
{
    QString append = _behavior->ingest(section, true);
    if ( ! append.isEmpty())
        _string += _behavior->sectionSeparator() + append;
}

void AbstractIdString::prepend(const QString & section)
{
    QString prepend = _behavior->ingest(section, true);
    if ( ! prepend.isEmpty())
        _string = prepend + _behavior->sectionSeparator() + _string;
}

QString AbstractIdString::data(void) const
{
    return _string;
}

QString AbstractIdString::operator() (void) const
{
    return _string;
}

QString AbstractIdString::sortable(void) const
{
    return _string.toLower();
}

QString AbstractIdString::sectionSeparator(void) const
{
    return _behavior->sectionSeparator();
}

QString AbstractIdString::section(int index, int repeatQutoed) const
{
    if (repeatQutoed)
    {
        QString s(_string.section(_behavior->sectionSeparator(), index, index));
        QStringList qsl;
        while (repeatQutoed--) qsl << s;
        return "\"" + qsl.join("\",\"") + "\"";
    }
    else
        return _string.section(_behavior->sectionSeparator(), index, index);
}

QStringList AbstractIdString::sectionList(int first, int last) const
{
    return _string.section(_behavior->sectionSeparator(), first, last)
                  .split(_behavior->sectionSeparator());
}

QString AbstractIdString::sections(int first, int last) const
{
    return _string.section(_behavior->sectionSeparator(), first, last);
}

int AbstractIdString::sectionCount(void) const
{
    if (isSectioned() && ! isNull())
        return _string.count(_behavior->sectionSeparator()) + 1;
    else
        return 0;
}
