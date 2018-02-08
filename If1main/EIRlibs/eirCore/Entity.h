#ifndef ENTITY_H
#define ENTITY_H

#include "VariableSet.h"

class QReadWriteLock;

class EIRCORESHARED_EXPORT Entity
        : private VariableSet
{
public:
    Entity(const QString & name=QString("VariableSet"));
    Entity(const quint64 key,
            const QString & id,
            const QString & name=QString("VariableSet"));
    Entity(const VariableSet & other);
    Entity(const Entity & other);

private:
    QReadWriteLock * map_rwl;
    QReadWriteLock * vl_rwl;
    QReadWriteLock * ba_rwl;
};

#endif // ENTITY_H
