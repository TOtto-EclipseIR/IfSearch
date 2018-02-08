#ifndef DDTCORE_H
#define DDTCORE_H

#include <qglobal.h>
#ifdef DDTCORE_LIB
# define DDTCORE_EXPORT Q_DECL_EXPORT
#else
# define DDTCORE_EXPORT Q_DECL_IMPORT
#endif

#include <QList>
#include <QObject>
#include <QRect>
class QDomDocument;
class QDomElement;
class QFile;
class QVariant;

namespace DDT
{
    enum Severity
    {
        Null = 0,
        Leave,
        Enter,
        Detail,
        Debug,
        Info,
        Progress,
        Warning,
        Error,
        Fatal,
        Unknown,
    };

    class Return;
    class InfoOutputBase;
} // DDT

class DDTCORE_EXPORT DDTcore
{
public:
    DDTcore();
    ~DDTcore();
    static QVector<int> lugNuts(const int n);
    static QStringList lugNuts(const QStringList qsl);
    static bool copyProperties(QObject * to, QObject * from);
    static QStringList propertyNames(const QObject * obj);
    static bool appendDomElement(QDomElement * Element, const QString & Name, QList<DDT::InfoOutputBase *> dummy)
    { (void)Element, (void)Name, (void)dummy; return false;}
    static bool appendDomElement(QDomElement * Element, const QString & Name, const QVariant & Var);
    static bool appendDomElement(QDomElement * Element, const QString & Name, const QString & Var);
    static bool parseDomElement(QList<DDT::InfoOutputBase *> * dummy, const QDomElement & elementVar)
    { (void)dummy, (void)elementVar; return false;}
    static bool parseDomElement(QVariant * rtnVar, const QDomElement & elementVar);
    static bool parseDomElement(QString * rtnVar, const QDomElement & elementVar);
    static DDT::Return readXmlFile(QDomDocument * doc, const QString & fileName, const QString & docType=QString());
    static DDT::Return read(QDomDocument * doc, QFile * file, const QString & docType=QString());
    static DDT::Return write(QFile * file, const QDomDocument & doc, const QString & docType=QString());
    static DDT::Return writeXmlFile(const QString & fileName, const QDomDocument & doc);
    static QList<QRect> simplifyRoiList(QList<QRect> inputList, QRect boundingRect, QSize minSize);
    static DDT::Return writeEmptyFile(QString fileName);
private:

};

#define QQ_RW_PROPERTY(type, name, reader, writer)			\
    private:												\
        type name;											\
    public:													\
        type reader(void) const { return name; }			\
        void writer(type v) { name = v; }

#endif // DDTCORE_H
