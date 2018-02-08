#include "ExeSupport.h"

#include <QtCore/QCoreApplication>
#include <QtGui/QImageReader>

#include "../eirCore/Variable.h"
#include "../eirCore/VariableSet.h"
#include "../eirCore/VariableSettings.h"
#include "../eirCore/VariableSettingsList.h"
#include "Log.h"
#include "OptionSet.h"

QStringList ExeSupport::imageFileNameFilters_slist;

ExeSupport::ExeSupport(QCoreApplication * parent)
    : parent_obj(parent)
    , _optionSet(new OptionSet(parent))
{
}

void ExeSupport::setVersion(const VersionInfo & vi)
{
    FUNCTION();
    _versionInfo = vi;
    if (parent_obj)
    {
        OBJPOINTER(QCoreApplication, parent_obj);
        parent_obj->setApplicationName(vi.getAppName());
        parent_obj->setOrganizationName(vi.getOrgName());
        parent_obj->setApplicationVersion(vi.toString());
    }
}

VersionInfo ExeSupport::version(void) const
{
    return _versionInfo;
}

OptionSet * ExeSupport::opts(void) const
{
    return _optionSet;
}

void ExeSupport::initializeOptions(void)
{
    QString orgName = _versionInfo.getOrgName();
    QString appName = _versionInfo.getAppName();
    QStringList appArgs(parent_obj->arguments());
    _optionSet->initiailize(orgName, appName, appArgs);
    _optionSet->start();
    DUMPVSL(*_optionSet);
}

void ExeSupport::initializeLogger(void)
{
    FUNCTION();
    VariableSet logOpts = opts()->exportSection("Log");
    LOG(configure(logOpts));
    LOG(start());
    LOGMSG("***%1 %2",
           opts()->exeFileInfo().canonicalFilePath(),
           _versionInfo.toString());
    LOGMSG("---From %1", QDir::current().absolutePath());
    LOGMSG("---Supported image formats %1",
           imageFileNameFilters().join(" "));
    DUMPVSET(*opts());
}

QStringList ExeSupport::imageFileNameFilters(void)
{
    if (imageFileNameFilters_slist.isEmpty())
    {
        QList<QByteArray> lba = QImageReader::supportedImageFormats();
        foreach(QByteArray ba, lba)
            imageFileNameFilters_slist << "*." + ba;
    }
    return imageFileNameFilters_slist;
}
