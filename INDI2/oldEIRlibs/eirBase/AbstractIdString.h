#ifndef ABSTRACTIDSTRING_H
#define ABSTRACTIDSTRING_H
#include "eirBase.h"

#include <QString>
#include <QStringList>

class BaseIdBehavior;

/*! @class AbstractIdString The AbstractIdString class provides a base for id strings.
  *
  * An IdString can be restricted to a specific set of characters
  * and can be segmented into sections via a delimiter.
  *
  * @see BaseIdBehavior
  *
  * @section Usage Usage
  *
  * @subsection hFile Derived .h file
  * @code
#include "../eirBase/AbstractIdString.h"

class EIRCORESHARED_EXPORT DerivedId : public AbstractIdString
{
public:
    DerivedId(const QString & inString=QString());
    DerivedId(const char * chars);
    DerivedId(const char * chars,
               const char * chars1,
               const char * chars2=0,
               const char * chars3=0);
};
  * @endcode
  *
  * @subsection cppFile Derived .cpp file
  * @code
#include "DerivedId.h"
#include "DerivedIdBehavior.h"

DerivedId::DerivedId(const QString & inString)
    : AbstractIdString(DerivedIdBehavior::instance(), inString)
{
}

DerivedId::DerivedId(const char * chars)
    : AbstractIdString(DerivedIdBehavior::instance(), QString(chars))
{
}

DerivedId::DerivedId(const char * chars,
                       const char * chars1,
                       const char * chars2,
                       const char * chars3)
    : AbstractIdString(DerivedIdBehavior::instance())
{
    set(QString(chars));
    append(QString(chars1));
    append(QString(chars2));
    append(QString(chars3));
}
  * @endcode
  */
class  AbstractIdString : public QString
{
public:
    AbstractIdString(BaseIdBehavior * behavior,
                     QString string=QString());
    bool isNull(void) const;
    bool operator == (const AbstractIdString & other) const;
    bool isSectioned(void) const;
    void set(const QString & string);
    void append(const QString & section);
    void append(const AbstractIdString & section);
    void prepend(const QString & section);
    QString sortable(void) const;
    QString sectionSeparator(void) const;
    QString section(int index, int repeatQutoed=0) const;
    QStringList sectionList(int first, int last=-1) const;
    QString sections(int first, int last=-1) const;
    int sectionCount(void) const;
    bool startsWith(const AbstractIdString & section) const;

protected:

private:
    BaseIdBehavior * _behavior;
};

#endif // ABSTRACTIDSTRING_H
