/** @file InfoOutputBase.h
*	Internal declaration of InfoOutputBase class for DDT core library
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

#include <QReadWriteLock>

#include <InfoSeverity.h>
#include <NamedArray.h>

class InfoItem;

class DDTCORE_EXPORT InfoOutputBase
{
public:
    InfoOutputBase(void) : rwLock(QReadWriteLock::Recursive) {}
    virtual ~InfoOutputBase() {}
    virtual void flush(void) {}
    virtual void write(const InfoItem & item) { (void)item; }
    virtual void clear(void) {}
    virtual bool isValid(void) const { return false; }
    virtual void rollover(void) {}

protected:
    QReadWriteLock * lock(void) { return & rwLock; }

private:
    QReadWriteLock rwLock;
}; // class InfoOutputBase
