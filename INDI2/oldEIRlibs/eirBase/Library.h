#ifndef LIBRARY_H
#define LIBRARY_H
#include "eirBase.h"

#include <QtCore/QMap>
#include <QtCore/QString>

#include "VersionInfo.h"

/*! @class Library The Library class manages the overall EIRC library collection.
  *
  */
class EIRBASESHARED_EXPORT Library
{
public:
    Library(const QString & name);
    QString name(void) const;
    VersionInfo version(void) const;
    void setVersion(const VersionInfo & vi);

private:
    QString name_s;
    VersionInfo _vi;
    static QMap<QString, Library *> name_ptr_map;
};

#endif // LIBRARY_H
