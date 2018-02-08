#ifndef VERSIONINFO_H
#define VERSIONINFO_H

#include <QDate>
#include <QString>

#include "Property.h"

// stupid preprocessor tricks follow
#define DEFSTRING2(PDEF) #PDEF
#define DEFSTRING(DEF) DEFSTRING2(DEF)

#define VERSIONINFO_PROPERTIES(TND) \
        TND(QString,    String,        QString()) \
        TND(QString,    OrgName,       QString()) \
        TND(QString,    AppName,       QString()) \
        TND(QString,    Copyright,     QString()) \
        TND(quint8,     Major,         0) \
        TND(quint8,     Minor,         0) \
        TND(quint8,     Branch,        0) \
        TND(quint8,     Release,       0) \

class VersionInfo
{
    DECLARE_PROPERTIES(VERSIONINFO_PROPERTIES);

public:
    VersionInfo(void);
    VersionInfo(const unsigned char major,
                const unsigned char minor,
                const unsigned char branch,
                const unsigned char release,
                const QString & string,
                const QString & copyright,
                const QString & orgname,
                const QString & appname);

    bool isNull(void) const;
    void setVersion(const QString & string);
    void addNotice(const QString & string);
    void setDateTime(const QString & string);
    QString toString(void) const;
    QString dottedString(void) const;
    quint32 toDWord(void) const;
    QString notice(void) const;
    QStringList noticeList(void) const;
    QString dateTimeString(void) const;
    void check(quint32 key) const;

private:
    QString noti;
    QString dts;
};

#endif // VERSIONINFO_H
