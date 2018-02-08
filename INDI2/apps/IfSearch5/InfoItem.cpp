/** @file InfoItem.cpp
*	Definition of InfoItem class for DDT core library
*
*	@author	Dynamic DeZigns Technology
*
*/

#include <InfoItem.h>

#include <QFileInfo>
#include <QCoreApplication>
#include <QThread>


#include <Info.h>

InfoItem::InfoItem(void)
: thread(0), ts(QTime::currentTime()), sev(InfoSeverity::Null), rc(0), file(QString()), line(0)
, msg(QString()), v1(QVariant()), v2(QVariant()), v3(QVariant()), v4(QVariant())
{
}

InfoItem::InfoItem(InfoSeverity Sev, const QString & File, const int Line, const QString & Msg,
                       QVariant V1, QVariant V2, QVariant V3, QVariant V4)
    : ts(QTime::currentTime()), sev(Sev), rc(0), file(File), line(Line), msg(Msg), v1(V1), v2(V2), v3(V3), v4(V4)
{
    thread = (qApp && qApp->thread() == QThread::currentThread()) ? 0 : QThread::currentThread();
}

InfoItem::InfoItem(InfoSeverity Sev, const QString & Msg,
                       QVariant V1, QVariant V2, QVariant V3, QVariant V4)
    : ts(QTime::currentTime()), sev(Sev), rc(0), file(QString()), line(0), msg(Msg), v1(V1), v2(V2), v3(V3), v4(V4)
{
    thread = (qApp && qApp->thread() == QThread::currentThread()) ? 0 : QThread::currentThread();
}

int InfoItem::msecSince(void) const
{
    int ms = QTime::currentTime().msecsTo(ts);
    return -ms;
}

QString InfoItem::stringOnly(void) const
{
    QString rtn;
    if (v1.isNull())
        rtn = msg;
    else if (v2.isNull())
        rtn = QObject::tr(qPrintable(msg)).arg(v1.toString());
    else if (v3.isNull())
        rtn = QObject::tr(qPrintable(msg)).arg(v1.toString()).arg(v2.toString());
    else if (v4.isNull())
        rtn = QObject::tr(qPrintable(msg)).arg(v1.toString()).arg(v2.toString()).arg(v3.toString());
    else
        rtn = QObject::tr(qPrintable(msg)).arg(v1.toString()).arg(v2.toString()).arg(v3.toString()).arg(v4.toString());
    return rtn;
}

QString InfoItem::toString(bool withTime, bool richText) const
{
    QString rtn = stringOnly();

    // TODO: withTime, richText based upon Severity
    (void)richText;

    if (thread)
        rtn = QObject::tr("[@0x%1] ").arg((ulong)thread, 8, 16, QChar('0')) + rtn;

    if ( ! file.isEmpty() && line)
    {
        QFileInfo fi(file);
        QString s = fi.completeBaseName();
        if (s.length() > 16)
            s = "-" + s.right(15);
        QString l = QObject::tr(" %1").arg(line, -4);
        rtn = s + l + rtn;
    }

    if (withTime)
    {
        static QString chars("?)(.~-+>*#");
        QString s = QString(chars[(sev < chars.size()) ? sev : 0]);
#if 0
        int ms = Info::timeBase().time().msecsTo(ts);
        QTime tm(0, 0, 0);
        tm = tm.addMSecs(ms);
        s += tm.toString("HH:mm:ss.zzz ");
#else
        s += ts.toString("HH:mm:ss.zzz ");
#endif
        rtn = s + rtn;
    }



    return rtn;
}
