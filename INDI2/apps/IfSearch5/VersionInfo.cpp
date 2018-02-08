#include "VersionInfo.h"

#include <QCoreApplication>
#include <QStringList>

VersionInfo::VersionInfo(void)
{
    DEFINE_PROPERTIES_CTORS(VERSIONINFO_PROPERTIES);
    setDateTime(__DATE__ " " __TIME__);
}

VersionInfo::VersionInfo(const unsigned char major,
                         const unsigned char minor,
                         const unsigned char branch,
                         const unsigned char release,
                         const QString & string,
                         const QString & copyright,
                         const QString & orgname,
                         const QString & appname)
{
    DEFINE_PROPERTIES_CTORS(VERSIONINFO_PROPERTIES);
    setDateTime(__DATE__ " " __TIME__);
    setMajor(major);
    setMinor(minor);
    setBranch(branch);
    setRelease(release);
    setString(string);
    setCopyright(copyright);
    setOrgName(orgname);
    setAppName(appname);
    if (qApp)
    {
        qApp->setApplicationName(appname);
        qApp->setApplicationVersion(toString());
        qApp->setOrganizationName(orgname);
    }
}

bool VersionInfo::isNull(void) const
{
    return ! (getMajor() || getMinor() || getBranch() || getRelease());
}

void VersionInfo::setVersion(const QString & string)
{
    setString(string);
    QStringList qsl = string.split('.');
    if (qsl.size() > 0)     setMajor(qsl.at(0).toInt());
    if (qsl.size() > 1)     setMinor(qsl.at(1).toInt());
    if (qsl.size() > 2)     setBranch(qsl.at(2).toInt());
    if (qsl.size() > 3)     setRelease(qsl.at(3).toInt());
}

void VersionInfo::addNotice(const QString & string)
{
    noti += string + "\n";
}

void VersionInfo::setDateTime(const QString & string)
{
    dts = string;
}

QString VersionInfo::toString(void) const
{
    QString sBranch, sRelease;
    if (getBranch())
        sBranch = QString("+B%1").arg(getBranch(), 2, 10, QChar('0'));
    if (getRelease() > 0 && getRelease() < 25)
        sRelease = QChar(0x40 + getRelease());
    else if (getRelease() > 24 && getRelease() < 50)
        sRelease = QString("X").append(QChar(0x40 + getRelease()-24));
    else if (getRelease() > 49 && getRelease() < 75)
        sRelease = QString("Y").append(QChar(0x40 + getRelease()-49));
    else if (getRelease() > 74 && getRelease() < 100)
        sRelease = QString("Z").append(QChar(0x40 + getRelease()-74));
    else if (getRelease() >= 0xA0 && getRelease() <= 0xA9)
        sRelease = QString("-Alpha%1").arg(getRelease() - 0xA0);
    else if (getRelease() >= 0xB0 && getRelease() <= 0xB9)
        sRelease = QString("-Beta%1").arg(getRelease() - 0xB0);
    else if (getRelease() >= 0xC0 && getRelease() <= 0xC9)
        sRelease = QString("-RC%1").arg(getRelease() - 0xC0);
    else if (getRelease() >= 0xF1 && getRelease() <= 0xF9)
        sRelease = QString("-Final%1").arg(getRelease() - 0xF0);
    else if (getRelease() != 0xF0 && getRelease() != 0xFF)
        sRelease = QString("-%1").arg(getRelease());
    return QString("v%1.%2%3%4 [%5]").arg(getMajor())
                                .arg(getMinor(), 2, 10, QChar('0'))
                                .arg(sBranch)
                                .arg(sRelease)
                                .arg(toDWord(), 8, 16, QChar('0'));
}

QString VersionInfo::dottedString(void) const
{
    return QString("%1.%2.%3.%4").arg(getMajor()).arg(getMinor())
                                 .arg(getBranch()).arg(getRelease());
}

quint32 VersionInfo::toDWord(void) const
{
    return ((quint32)getMajor() << 24)
            | ((quint32)getMinor() << 16)
            | ((quint32)getBranch() << 8)
            | (quint32)getRelease();
}

QString VersionInfo::notice(void) const
{
    return noti;
}

QStringList VersionInfo::noticeList(void) const
{
    return noti.isEmpty() ? QStringList() : noti.split('\n');
}

QString VersionInfo::dateTimeString(void) const
{
    return dts;
}

void VersionInfo::check(quint32 key) const
{
    QChar c1(0x7F & (key >> 24));
    QChar c2(0x7F & (key >> 16));
    QChar c3(0x7F & (key >>  8));
    QChar c4(0x7F &  key);
    QString s;
    s += c1;
    s += c2;
    s += c3;
    s += c4;
    if (getCopyright().contains(s))
        return;
    qFatal("Nice try");
}
