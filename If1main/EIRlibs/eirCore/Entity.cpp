#include "Entity.h"

#include <QtCore/QReadWriteLock>

Entity::Entity(const QString & name)
    : VariableSet(name)
    , map_rwl(new QReadWriteLock(QReadWriteLock::Recursive))
    , vl_rwl(new QReadWriteLock(QReadWriteLock::Recursive))
    , ba_rwl(new QReadWriteLock(QReadWriteLock::Recursive))
{}

Entity::Entity(const quint64 key,
               const QString & id,
               const QString & name)
    : VariableSet(key, id, name)
    , map_rwl(new QReadWriteLock(QReadWriteLock::Recursive))
    , vl_rwl(new QReadWriteLock(QReadWriteLock::Recursive))
    , ba_rwl(new QReadWriteLock(QReadWriteLock::Recursive))
{}

Entity::Entity(const VariableSet & other)
    : VariableSet(other)
    , map_rwl(new QReadWriteLock(QReadWriteLock::Recursive))
    , vl_rwl(new QReadWriteLock(QReadWriteLock::Recursive))
    , ba_rwl(new QReadWriteLock(QReadWriteLock::Recursive))
{}

Entity::Entity(const Entity & other)
    : VariableSet(other)
    , map_rwl(other.map_rwl)
    , vl_rwl(other.vl_rwl)
    , ba_rwl(other.ba_rwl)
{}
