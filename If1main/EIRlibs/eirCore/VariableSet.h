#ifndef VARIABLESET_H
#define VARIABLESET_H
#include "eirCore.h"

#include <QtCore/QSharedData>
#include <QtCore/QSharedDataPointer>

#include "Variable.h"
class VariableIdList;

class VariableSetData : public QSharedData
{
public:
    VariableSetData(void) : key_u64(0) {}
    VariableSetData(const VariableSetData & other);

    QString name_s;
    quint64 key_u64;
    QString id_s;
    QMap<QString, Variable> vbl_map;
    QList<QVariant> var_list;
    QByteArray _ba;
};


/*! @class VariableSet The VariableSet class contains three pieces of data identified by a key and id.
  *
  * VariableSet is a core part of the EclipseIR libraries.
  * Many of the support, interface, and data transfer mechanisms will be based upon or
  * will make use of VariableSet.
  *
  * @section Id Identification
  *
  * The INDIbase system is based upon a set of unique numeric keys and unique string
  * ids within a key type. The VariableSet is identified by a dbKey() (INDIbase unique database key)
  * a 64-bit key() that can be translated to a particular key class and type, and
  * a string id(). It also has a name() string that can be used to identify its contents.
  * These values can be set by setDbKey(), setKey(), setId(), and setName()
  * and retrieve with dbKey(), key(), id(), and name().
  *
  * @section Data Data
  *
  * VariableSet contains three data elements.
  *     # A map of Variable (essentially a named QVariant) elements
  *     # A list of QVariant
  *     # A QByteArray for pure binary data
  *
  * The isNull() function returns true only if <i> all </i> three are empty.
  *
  * @subsection Map Variable Map
  *
  * Variable items can be added to the map with
  * set(const VariableId & vid, const QVariant & var) or
  * set(const Variable & vbl).
  * They can be retrived from the list with
  * at(const VariableId & vid) (returning a Variable) or
  * value(const VariabkeId & vid) (returning the contained QVariant).
  *
  * @subsection List QVariantList
  *
  * Item can be added to the list with append(const QVariant & var)
  * and prepent(const QVariant & var).
  * They can be retrieved with at(const int index).
  *
  * @subsection ba QByteArray
  *
  * The binary data is set with set(const QByteArray & ba)
  * and retrieved by binary(void).
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


#define DERIVED_VARIABLESET(TIVD) \
    TIVD(int, Sequence, int, 0) \
    TIVD(QString, Mode, QString, QString()) \

class EIRSDK1SHARED_EXPORT Derived : public VariableSet
{
public:
    DECLARE_VARIABLESET(DERIVED_VARIABLESET);
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
    : VariableSet("Derived")
{
    DEFINE_VARIABLESET_CTORS(DERIVED_VARIABLESET);
}
  * @endcode
  *
  */
class EIRCORESHARED_EXPORT VariableSet
{
public:
    VariableSet(const QString & name=QString("VariableSet"));
    VariableSet(const quint64 key,
                const QString & id,
                const QString & name=QString("VariableSet"));
    VariableSet(const VariableSet & other);
    VariableSet &operator=(const VariableSet &);
    ~VariableSet();

    QString name(void) const;
    quint64 key(void) const;
    QString id(void) const;
    void setName(const QString & newName);
    void setKey(const quint64 key);
    void setId(const QString & id);

    bool isEmpty(void) const;
    void clear(void);
    void reset(void);
    int binarySize(void) const;
    int listSize(void) const;
    int mapSize(void) const;
    bool contains(const VariableId & vid) const;
    void blog(void) const;

    void set(const QVariantList & vl);
    void set(const Variable & vbl);
    void set(const VariableId & vid,
             const QVariant & value);
    void set(const int index,
             const QVariant & value);
    void set(const QByteArray & ba);
    void reset(const VariableId & id);
    void append(const QVariant & value);
    Variable at(const VariableId & id) const;
    QVariant value(const VariableId & id,
                   const QVariant &defaultValue=QVariant()) const;
    QVariant value(const int index) const;
    QByteArray value(void) const;
    QVariantList values(void) const;

    VariableSet exportSection(const VariableId & sectionId) const;
    VariableIdList sectionIds(const VariableId & within=VariableId()) const;
    VariableIdList ids(const VariableId & within=VariableId()) const;
    void import(const VariableSet & other,
                const VariableId & sectionId=VariableId());
    QList<Variable> all(void) const;

private:

private:
    QSharedDataPointer<VariableSetData> data;
};
Q_DECLARE_TYPEINFO(VariableSet, Q_PRIMITIVE_TYPE);
Q_DECLARE_METATYPE(VariableSet)

#define VARIABLESET_DECLARE(TYPE, ID, VTYPE, DFLT) \
        void reset##ID(void)        { reset(VariableId(#ID)); } \
        TYPE get##ID(void) const    { return (TYPE)value(VariableId(#ID)).value<VTYPE>(); } \
        void set##ID(TYPE v)        { set(VariableId(#ID), QVariant(VTYPE(v))); } \

#define VARIABLESET_CTOR(TYPE, ID, VTYPE, DFLT) \
        VariableSet::set(Variable(VariableId(#ID), QVariant((VTYPE)DFLT))); \

#define DECLARE_VARIABLESET(DEF) \
        DEF(VARIABLESET_DECLARE) \

#define DEFINE_VARIABLESET_CTORS(DEF) \
        DEF(VARIABLESET_CTOR) \


#endif // VARIABLESET_H
