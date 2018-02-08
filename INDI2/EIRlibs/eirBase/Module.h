#ifndef MODULE_H
#define MODULE_H
#if defined(EIRBASE_LIBRARY)
#  define EIRBASESHARED_EXPORT Q_DECL_EXPORT
#else
#  define EIRBASESHARED_EXPORT Q_DECL_IMPORT
#endif

#include <QtCore/QMap>
#include <QtCore/QString>

#include "VersionInfo.h"

/*! @class Module The Module class manages the overall EIRC library collection.
  *
  */
class EIRBASESHARED_EXPORT Module
{
public:
    Module(const QString & name,
           const VersionInfo vi);
    QString name(void) const;
    VersionInfo version(void) const;

public:
    static QStringList moduleNames(void);

private:
    QString name_s;
    VersionInfo _vi;
    static QMap<QString, Module *> name_ptr_map;
};
Q_DECLARE_TYPEINFO(Module, Q_PRIMITIVE_TYPE);

#endif // MODULE_H
