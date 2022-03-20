/** @file AttributeSet.h
*	Declaration of AttributeSet class for DDT core library
*
*	@author	Dynamic DeZigns Technology
*/

#pragma once
#include <qglobal.h>

#include <QMap>
#include <QObject>
#include <QString>

class AttributeSet
{
public:
    AttributeSet(void);
    ~AttributeSet();
    AttributeSet & operator=(const AttributeSet & that);

    void set(const QString & section, const QString & variable,
             const QString & value, const QString & detail=QString());
    bool isEmpty(void);
    void clear(void) { map.clear(); }

private:
    QMap<QString,QString> map;
}; // class AttributeSet

