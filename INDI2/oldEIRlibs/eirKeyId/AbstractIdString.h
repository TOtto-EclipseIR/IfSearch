#ifndef ABSTRACTIDSTRING_H
#define ABSTRACTIDSTRING_H
#include "eirKeyId.h"

#include <QString>
#include <QStringList>

class BaseIdBehavior;

class EIRKEYIDSHARED_EXPORT AbstractIdString
{
public:
    AbstractIdString(BaseIdBehavior * behavior,
                     QString string=QString());
    AbstractIdString(BaseIdBehavior * behavior,
                     const char * chars);
    bool isNull(void) const;
    bool operator == (const AbstractIdString & other);
    bool isSectioned(void) const;
    void set(const QString & string);
    void append(const QString & section);
    void prepend(const QString & section);
    QString data(void) const;
    QString operator() (void) const;
    QString sortable(void) const;
    QString sectionSeparator(void) const;
    QString section(int index, int repeatQutoed=0) const;
    QStringList sectionList(int first, int last=-1) const;
    QString sections(int first, int last=-1) const;
    int sectionCount(void) const;

protected:

private:
    BaseIdBehavior * _behavior;
    QString _string;
};

#endif // ABSTRACTIDSTRING_H
