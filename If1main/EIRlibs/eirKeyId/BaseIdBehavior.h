#ifndef BASEIDBEHAVIOR_H
#define BASEIDBEHAVIOR_H
#include "eirKeyId.h"

#include <QMap>
#include <QString>

class EIRKEYIDSHARED_EXPORT BaseIdBehavior
{
public:
    BaseIdBehavior(const QString & allowed=QString(),
                   const QString & separator=QString(),
                   const QString & escaped=QString(),
                   const QString & ignored=QString());
    virtual QString ingest(const QString & inString,
                           const bool isSection=false) const;
    virtual QString sectionSeparator(void) const;
    virtual void replace(const QString & key,
                         const QString & with);

public:
    static const QString lower;

private:
    QString allowed_string;
    QString separator_string;
    QString escaped_string;
    QString ignored_string;
    QMap<QString, QString> replace_map;
};

#endif // BASEIDBEHAVIOR_H
