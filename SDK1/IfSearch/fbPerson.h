/** @file fbPerson.h
*
*	Public declarations for INDI project fbPerson
*/

#pragma once
#include <qglobal.h>

#include <fbValue.h>

#include <QPair>
#include <QString>

#include <DualMap.h>
#include <Return.h>

class fbPerson
{
public:
    fbPerson(void) : Key(0), PrimaryFaceKey(0), Active(true) {}
    ~fbPerson() {}
    int key(void) const { return Key; }
    QString id(void) const { return Id; }
    QString description(void) const { return Description; }
    int primaryFaceKey(void) const { return PrimaryFaceKey; }
    void setKey(int v) { Key = v; }
    void setId(const QString s) { Id = s; }
    void setDescription(const QString s) { Description = s; }
    bool isNull(void) const { return ! Key; }
    void remove(void) { Active = false; }
    bool isActive(void) const { return Active; }

public:
    Return readXmlFile(const QString & fileName);
    Return writeXmlFile(const QString & fileName);

protected:
    int Key;
    QString Id;
    QString Description;
    int PrimaryFaceKey;
    bool Active;
    fbValueSet <fbCategory> categorySet;
}; // fbPerson
