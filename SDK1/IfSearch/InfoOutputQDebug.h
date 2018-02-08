/** @file InfoOutputQDebug.h
*	Internal declaration of InfoOutputFile class for DDT core library
*
*	@author	Dynamic DeZigns Technology
*/
#pragma once
#include <qglobal.h>
#ifdef DDTCORE_LIB
# define DDTCORE_EXPORT Q_DECL_EXPORT
#else
# define DDTCORE_EXPORT Q_DECL_IMPORT
#endif

#include <InfoOutputBase.h>

#include <QFile>
#include <QTextStream>

class DDTCORE_EXPORT InfoOutputQDebug : public InfoOutputBase
{
public:
    InfoOutputQDebug(void) {}
    ~InfoOutputQDebug() {}

protected:
    virtual void write(const InfoItem & item)
    {
        QWriteLocker wLock(lock());
        InfoSeverity sev = item.severity();
        QString str = item.toString();
        if (sev > Warning)				qCritical() << str;
        else if (sev < Warning)			qDebug() << str;
        else							qWarning() << str;
    }
    virtual bool isValid(void) const
    { return true; }

private:
}; // class InfoOutputQDebug
