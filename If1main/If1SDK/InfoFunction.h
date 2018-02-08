/** @file InfoFunction.h
*	Declaration of InfoFunction class for DDT core library
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

#include <Info.h>

#include <QObject>
#include <QVariant>

namespace DDT
{
	class DDTCORE_EXPORT InfoFunction
	{
	public:
		InfoFunction(const QString & FnName, const QString & File, const int Line, 
					 const QString & Msg=QString(), QVariant V1=QVariant(), 
					 QVariant V2=QVariant(), QVariant V3=QVariant(), QVariant V4=QVariant());
		~InfoFunction();
		QString fnName;
		QString file;
		int line;
		QVariant returnVar;
	}; // InfoFunction
} // DDT
