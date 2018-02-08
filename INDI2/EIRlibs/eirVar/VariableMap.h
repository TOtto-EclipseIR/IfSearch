#ifndef VARIABLEMAP_H
#define VARIABLEMAP_H

#include <QMap>
#include <eirType/Sortable.h>
#include "Variable.h"

/*! @class VariableMap The VariableMap class maps a Variable's id to its complete value.
  *
  * @section Usage Usage
  *
  * Derived classes can preset Variable values in the map.
  * To do so, add the following to the header and implementation files.
  *
  * @subsection hFile Derived .h file
  *
  * @code
#include "../eirCore/VariableSet.h"


#define DERIVED_VARIABLEMAP(TIVD) \
    TIVD(int, Sequence, int, 0) \
    TIVD(QString, Mode, QString, QString()) \

class EIRSDK1SHARED_EXPORT Derived : public VariableMap
{
public:
    DECLARE_VARIABLESET(DERIVED_VARIABLEMAP);
    Derived(void);
}
  * @endcode
  *
  * In the TIVD definition there are four elements:
  * @arg typename of the external values
  * @arg name of the value; void setName(typename v) and typename getName(void) const will be defined
  * @arg QVariant type stored in the map
  * @arg QVariant default value
  *
  * @subsection cppFile Derived .cpp file
  *
  * @code
#include "Derived.h"

Derived::Derived(void)
{
    DEFINE_VARIABLEMAP_CTORS(DERIVED_VARIABLEMAP);
}
  * @endcode
  */
class EIRVARSHARED_EXPORT VariableMap : public QMap<Sortable, Variable>
{
public:
    VariableMap(void);
    bool contains(const VariableId & id) const;
    void insert(const Variable var);
    VariableIdList keys(void) const;
    int remove(const VariableId & id);
    Variable take(const VariableId & id);
    Variable value(const VariableId & id,
                   const Variable defaultVar=Variable()) const;
    Variable operator [] (const VariableId & id);
    const Variable operator [] (const VariableId & id) const;
};

#define VARIABLEMAP_DECLARE(TYPE, ID, VTYPE, DFLT) \
        TYPE get##ID(void) const { return (TYPE)value(VariableId(#ID)).value<VTYPE>(); } \
        void set##ID(TYPE v)     { remove((VariableId(#ID); insert(VariableId(#ID), QVariant(VTYPE(v))); } \
        void reset##ID(void)     { remove((VariableId(#ID); insert(VariableId(#ID), QVariant(VTYPE(DFLT))); } \

#define VARIABLEMAP_CTOR(TYPE, ID, VTYPE, DFLT) \
        VARIABLEMAP::insert(Variable(VariableId(#ID), QVariant((VTYPE)DFLT))); \

#define DECLARE_VARIABLEMAP(DEF) \
        DEF(VARIABLEMAP_DECLARE) \

#define DEFINE_VARIABLEMAP_CTORS(DEF) \
        DEF(VARIABLEMAP_CTOR) \


#endif // VARIABLEMAP_H
