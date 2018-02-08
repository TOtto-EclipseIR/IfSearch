#include "eirModule.h"

#include "version.h"
#include "../../common/version.h"
#include "VersionInfo.h"

VersionInfo ver(VER_MAJOR, VER_MINOR, VER_BRANCH, VER_RELEASE,
                VER_STRING, VER_COPYRIGHT, VER_ORGNAME, VER_APPNAME);

QStringList eirModule::modules_qsl;

eirModule::eirModule(const QString & moduleName)
{
}

QString eirModule::moduleName(void) const
{
    return name_s;
}

QStringList eirModule::names(void)
{
    return modules_qsl;
}

bool eirModule::staticInitialize(const QString & moduleName)
{
    if ( ! moduleName.isEmpty())
        modules_qsl.append(moduleName);
    return true;
}

VersionInfo eirModule::version(void)
{
    return ver;
}
