/** @file fbValue.h
*
*	Public declarations for INDI project fbValue
*/

#pragma once
#include <qglobal.h>
#ifdef INDIFB_LIB
# define INDIFB_EXPORT Q_DECL_EXPORT
#else
# define INDIFB_EXPORT Q_DECL_IMPORT
#endif

#include <QDomElement>
#include <QPair>
#include <QString>

#include <DualMap.h>
#include <Return.h>

class fbValue
{
protected:
    fbValue(void) : Key(0) {}

public:
    ~fbValue() {}
    int key(void) const { return Key; }
    bool isNull(void) const { return ! Key; }
    QString toString(void) const;
    QString value(void) const;
    QString subvalue(void) const;

public:
    static QString value(int x);
    static QString subvalue(int x);
    static QString toString(int x);
    static Return readXmlFile(const QString & fileName);
    static Return writeXmlFile(const QString & fileName);
    static int find(const QString & v, const QString & sv=QString());
    static int insert(const QString & v, const QString & sv=QString());

protected:
    int Key;
    static DualMap<int, QPair<QString, QString> >	valueMap;
    static bool dirty;
    static QChar PairDelimiter;
    static QChar SubDelimiter;
}; // fbValue

template <class T> class INDIFB_EXPORT fbValueSet : public QList<int>
{
public:
    bool insert(const QString & v, const QString & sv=QString());
    bool remove(const QString & v, const QString & sv=QString());
    bool parse(const QString & v);

    int parse(const QDomElement & de, const QString & tagName)
    {
        int k = 0;
        QDomElement deValue = de.firstChildElement(tagName);
        while ( ! deValue.isNull())
        {
            QString value = deValue.attribute("Value");
            QString subvalue = deValue.attribute("Subvalue");
            if ( ! value.isEmpty())
            {
                int x = T::find(value, subvalue);
                if ( ! x)
                    x = T::insert(value, subvalue);
                QList<int>::append(x);
                ++k;
            }
            deValue = deValue.nextSiblingElement();
        }
        return k;
    }

    int fill(QDomElement * de, const QString & tagName) const
    {
        int k = 0;
        foreach(int x, *this)
        {
            QString v = T::value(x);
            QString sv = T::subvalue(x);
            if ( ! v.isEmpty())
            {
                QDomElement deValue = de->ownerDocument().createElement(tagName);
                deValue.setAttribute("Key", x);
                deValue.setAttribute("Value", v);
                if ( ! sv.isEmpty())
                    deValue.setAttribute("Subvalue", sv);
                ++k;
                de->appendChild(deValue);
            }
        }
        return k;
    }

private:

}; // fbValueSet

class INDIFB_EXPORT fbCategory : public fbValue
{
public:
    fbCategory(void) : fbValue() {}
}; // fbCategory

