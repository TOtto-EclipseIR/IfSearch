#include "eirBase.h"

#include "version.h"
#include "../../../common/version.h"

QMap<QString, Library *> Library::name_ptr_map;

Library::Library(const QString & name)
    : name_s(name)
{
    SETVERSION();
    name_ptr_map.insert(name, this);
}

QString Library::name(void) const
{
    return name_s;
}

VersionInfo Library::version(void) const
{
    return _vi;
}

void Library::setVersion(const VersionInfo & vi)
{
    _vi = vi;
}
