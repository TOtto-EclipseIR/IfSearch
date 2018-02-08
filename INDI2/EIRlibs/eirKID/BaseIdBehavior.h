#ifndef BASEIDBEHAVIOR_H
#define BASEIDBEHAVIOR_H
#include "eirKID.h"

#include <QMap>
#include <QString>

/*! @class BaseIdBehavior The BaseIdBehavior class provides a base behavior for id strings.
  *
  * An IdString can be restricted to a specific set of characters
  * and can be segmented into sections via a delimiter.
  *
  * @see AbstractIdString
  *
  * @section Usage Usage
  *
  * @subsection hFile Derived .h file
  * @code
#include "../eirBase/BaseIdBehavior.h"

class EIRCORESHARED_EXPORT DerivedIdBehavior : public BaseIdBehavior
{
public:
    static DerivedIdBehavior * instance(void);

private:
    DerivedIdBehavior(void);
    static DerivedIdBehavior * singleton;
};
  * @endcode
  *
  * @subsection cppFile Derived .cpp file
  * @code
DerivedIdBehavior * DerivedIdBehavior::singleton=0;

DerivedIdBehavior * DerivedIdBehavior::instance(void)
{
    if ( ! singleton)
        singleton = new DerivedIdBehavior;
    return singleton;
}

DerivedIdBehavior::DerivedIdBehavior()
    : BaseIdBehavior("/")
{
    replace("_", "/");
}
  * @endcode
  *
  * @todo Plan for 16-bit unicode escaping
  */
class EIRKIDSHARED_EXPORT BaseIdBehavior
{
public:
    BaseIdBehavior(const QString & separator=QString(),
                   const QString & allowed=QString(),
                   const QString & escaped=QString(),
                   const QString & ignored=QString());
    virtual QString ingest(const QString & inString,
                           const bool isSection=false) const;
    virtual QString sectionSeparator(void) const;
    virtual void replace(const QString & key,
                         const QString & with);

public:
    static const QString lower;
    static const QString upper;
    static const QString number;
    static const QString uriReserved;
    static const QString fileSymbol;
    static const QString fileReserved;

private:
    QString allowed_string;
    QString separator_string;
    QMap<QString, QString> replace_map;
};

#endif // BASEIDBEHAVIOR_H
