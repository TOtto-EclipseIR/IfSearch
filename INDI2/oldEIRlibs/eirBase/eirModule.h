#ifndef EIRMODULE_H
#define EIRMODULE_H
#include "eirBase_global.h"

#include <QtCore/QMetaType>
#include <QtCore/QStringList>

#include "StaticInitialize.h"

class VersionInfo;

class EIRBASESHARED_EXPORT eirModule : public StaticInitialize
{
public:
    eirModule(const QString & moduleName=QString());
    QString moduleName(void) const;
    static VersionInfo version(void);
    static QStringList names(void);

private:
    bool staticInitialize(const QString & moduleName);

private:
    QString name_s;
    static QStringList modules_qsl;
};
Q_DECLARE_TYPEINFO(eirModule, Q_PRIMITIVE_TYPE);
Q_DECLARE_METATYPE(eirModule)

#endif // EIRMODULE_H
