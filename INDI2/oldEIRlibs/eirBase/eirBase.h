#ifndef EIRBASE_H
#define EIRBASE_H
#include "eirBase_global.h"
#ifndef __MODULE__
# define __MODULE__ "eirBase"
#endif

#include <QtCore/QVariantList>

#include "Library.h"

/*! eirBase library houses templates and class mechanisms upon which other libraries can be based.
  *
  * Management of keys (unique numeric identifiers) and ids (unique strings within a key type) is
  * supported with the AbstractIdString, BaseIdBehavior, KeyTemplate classes.
  *
  * Enumeration and Named allow other classes to name their values.
  *
  * DataProperty, Property, and QProperty headers allow automation of getters, setters, and declaration
  *in QSharedData, regular classes, and QObject implementations.
  */
class eirBase : public Library
{
public:
    eirBase(void);
    static int powerOfTwo(const int n);
    static QString formatMessage(const QString & format,
                                 const QVariantList & varList);

public:
    static eirBase instance;

private:

private:
};

#endif // EIRBASE_H
