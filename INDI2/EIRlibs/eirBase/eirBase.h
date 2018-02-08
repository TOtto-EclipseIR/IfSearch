/*! @file eirBase.h Declarations for eirBase library
 *
  */
#ifndef EIRBASE_H
#define EIRBASE_H
#ifndef __MODULE__
# define __MODULE__ "eirBase"
#endif

#include "eirbase_global.h"

#include <QProcessEnvironment>

#include "Module.h"

/*! eirBase library houses templates and class mechanisms upon which other libraries can be based.
  *
  * Enumeration and Named templates allow other classes to name their values.
  *
  * DataProperty, Property, and QProperty headers allow automation of
  * getters, setters, and declaration
  * in QSharedData, regular classes, and QObject implementations.
  */
class EIRBASESHARED_EXPORT eirBase : public Module
{

public:
    eirBase(void);
    QString envVar(const QString & name,
                   const QString & defaultValue=QString()) const;

public:
    static eirBase * instance(void);

private:
    QProcessEnvironment environment;
    static eirBase * instance_p;
};
Q_DECLARE_METATYPE(eirBase);

#endif // EIRBASE_H
