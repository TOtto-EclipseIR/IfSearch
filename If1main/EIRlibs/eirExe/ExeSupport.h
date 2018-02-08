#ifndef EXESUPPORT_H
#define EXESUPPORT_H
#include "eirExe.h"

#include <QtCore/QFileInfo>
#include <QtCore/QStringList>

#include "../eirBase/VersionInfo.h"

class QCoreApplication;

class OptionSet;

class EIREXESHARED_EXPORT ExeSupport
{
public:
    explicit ExeSupport(QCoreApplication * parent=0);
    void setVersion(const VersionInfo & vi);
    void initializeOptions(void);
    void initializeLogger(void);

    VersionInfo version(void) const;
    OptionSet * opts(void) const;

public:
    static QStringList imageFileNameFilters(void);


private:
    QCoreApplication * parent_obj;
    VersionInfo _versionInfo;
    OptionSet * _optionSet;

private:
    static QStringList imageFileNameFilters_slist;
};

#endif // EXESUPPORT_H
