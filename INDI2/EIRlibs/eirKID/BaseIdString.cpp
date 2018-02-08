#include "BaseIdString.h"

#include <eirBase/BaseLog.h>
#include <eirType/Sortable.h>

#include "BaseIdBehavior.h"

BaseIdString::BaseIdString(void)
    : _behavior(0)
{
}

BaseIdString::BaseIdString(BaseIdBehavior * behavior,
                                   QString string)
    : _behavior(behavior)
{
    set(string);
}


bool BaseIdString::operator == (const BaseIdString & other) const
{
    return Sortable(QString(*this)) == Sortable(QString(other));
}

bool BaseIdString::isNull(void) const
{
    return QString::isEmpty() || ! _behavior;
}

bool BaseIdString::isSectioned(void) const
{
    return ! _behavior->sectionSeparator().isEmpty();
}

void BaseIdString::set(const QString & string)
{
    QString::operator = (_behavior->ingest(string));
}

void BaseIdString::append(const QString & section)
{
    QString append = _behavior->ingest(section, true);
    if ( ! append.isEmpty())
        QString::operator += (_behavior->sectionSeparator() + append);
}

void BaseIdString::append(const BaseIdString & section)
{
    if ( ! section.isNull())
        QString::append(QString(_behavior->sectionSeparator()).append(section));
}

void BaseIdString::prepend(const QString & section)
{
    QString prepend = _behavior->ingest(section, true);
    if ( ! prepend.isEmpty())
        QString::prepend(prepend + _behavior->sectionSeparator());
}

QString BaseIdString::sectionSeparator(void) const
{
    return _behavior->sectionSeparator();
}

QString BaseIdString::section(int index, int repeatQutoed) const
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

QStringList BaseIdString::sectionList(int first, int last) const
{
    return QString::section(_behavior->sectionSeparator(), first, last)
                    .split(_behavior->sectionSeparator());
}

QString BaseIdString::sections(int first, int last) const
{
    return QString::section(_behavior->sectionSeparator(), first, last);
}

int BaseIdString::sectionCount(void) const
{
    if (isSectioned() && ! isNull())
        return QString::count(_behavior->sectionSeparator()) + 1;
    else
        return 0;
}

bool BaseIdString::startsWith(const BaseIdString & section) const
{
    return QString::startsWith(QString(section).append(_behavior->sectionSeparator()));
}
