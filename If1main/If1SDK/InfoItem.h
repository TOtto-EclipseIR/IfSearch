/** @file InfoItem.h
*	Internal declaration of InfoItem class for DDT core library
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

#include <InfoSeverity.h>

#include <QString>
#include <QTime>
#include <QVariant>
class QTextStream;
class QThread;

namespace DDT
{
	class DDTCORE_EXPORT InfoItem
	{
	public:
		InfoItem(void);
		InfoItem(InfoSeverity Sev, const QString & File, const int Line, const QString & Msg, QVariant V1=QVariant(), 
			   QVariant V2=QVariant(), QVariant V3=QVariant(), QVariant V4=QVariant());
		InfoItem(InfoSeverity Sev, const QString & Msg, QVariant V1=QVariant(), 
			   QVariant V2=QVariant(), QVariant V3=QVariant(), QVariant V4=QVariant());
		~InfoItem() {};
		InfoSeverity severity(void) const { return sev; }
		void clear(void) { *this = InfoItem(); }
		void setReturnCode(int rtn) { rc = rtn; }
		int returnCode(void) const { return rc; }
		int msecSince(void) const;
		QTime time(void) const { return ts; }
		QString stringOnly(void) const;
		QString toString(bool withTime=true, bool richText=true) const;

	private:
		QThread *	thread;
		QTime		ts;
		int			rc;
		InfoSeverity sev;
		QString		file;
		int			line;
		QString		msg;
		QVariant	v1;
		QVariant	v2;
		QVariant	v3;
		QVariant	v4;
	}; // class InfoItem
} // namespace DDT

