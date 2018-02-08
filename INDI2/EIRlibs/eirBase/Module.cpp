#include "eirBase.h"

#include <QStringList>

#include "BaseLog.h"

QMap<QString, Module *> Module::name_ptr_map;

Module::Module(const QString & name,
               const VersionInfo vi)
    : name_s(name)
    , _vi(vi)
{
    name_ptr_map.insert(name, this);
}

QString Module::name(void) const
{
    return name_s;
}

VersionInfo Module::version(void) const
{
    return _vi;
}

QStringList Module::moduleNames(void)
{
    return name_ptr_map.keys();
}
