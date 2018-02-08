#ifndef VARIABLEGROUP_H
#define VARIABLEGROUP_H
#include "eirTypes.h"

#include <QtCore/QMap>
#include <QtCore/QString>

#include "Variable.h"
#include "VariableId.h"

class EIRTYPESSHARED_EXPORT VariableGroup
{
public:
    explicit VariableGroup(const VariableId & sectionId=VariableId());
    //VariableGroup(const QDomElement & de);
    VariableId sectionId(void) const;
    bool isEmpty(void) const;
    int size(void) const;
    bool contains(const VariableId & id) const;
    void add(const Variable & var);
    void add(VariableGroup * other,
             VariableId sectionId=VariableId());
    Variable at(const VariableId & id) const;
    QVariant value(const VariableId & id) const;
    bool set(const VariableId & id, const Variable & var);
    bool set(const VariableId & id, const QVariant & var);
    void set(const VariableGroup * vars);
    void reset(const VariableId & id);
    void reset(void);
    void clear(void);
    void remove(const VariableId & id);
    VariableIdList ids(void) const;
    QStringList sectionIds(const VariableId & within=VariableId());
    int implant(const VariableId & sectionId,
                const VariableGroup * vars);
    QList<Variable> all(void) const;
    void dump(void) const;
    //QDomElement writeXml(void) const;
    //bool readXml(const QDomElement & de);
    int csvHeadingRowCount(void) const;

private:
    VariableId section_id;
    QMap<QString, Variable> var_map;
    VariableIdList id_list;
    int csvRows;
};

#define GROUPVARIABLES_DECLARE(ID, TYPE, DFLT) \
        void reset##ID(void) { reset(VariableId(#ID)); } \
        TYPE get##ID(void) const { return value(VariableId(#ID)).value<TYPE>(); } \
        void set##ID(TYPE v) { set(VariableId(#ID), QVariant(v)); } \

#define GROUPVARIABLES_CTOR(ID, TYPE, DFLT) \
        VariableGroup::add(Variable(VariableId(#ID), QVariant((TYPE)DFLT))); \

#define DECLARE_GROUPVARIABLES(DEF) \
        DEF(GROUPVARIABLES_DECLARE) \

#define DEFINE_GROUPVARIABLES_CTORS(DEF) \
        DEF(GROUPVARIABLES_CTOR) \

#endif // VARIABLEGROUP_H
