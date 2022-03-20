/** @file InfoMacros.h
*	Declaration of macros for Info class for DDT core library
*
*	@author	Dynamic DeZigns Technology
*/
#pragma once
#include <qglobal.h>
#ifdef TRACE_H
#pragma warning "Don't mix Trace and Info"
#endif

#include <Info.h>

#define	ASSERT(test) ((void)((test) ? Info::add(InfoItem(InfoSeverity::Fatal,__FILE__, __LINE__, "FATAL ERRMSG: %1", #test)) : 0))
#define NULLPTR(ptr) if(!ptr) Info::add(InfoItem(InfoSeverity::Fatal,__FILE__, __LINE__, "FATAL ERRMSG: %1 is NULL", #ptr));
#define EXPECT(value, test) (Info::expect(#value, value, #test, test, __FILE__, __LINE__))
#define EXPECTNOT(value, test) (Info::expectNot(#value, value, #test, test, __FILE__, __LINE__))
#define FATAL(msg, ...) Info::add(InfoItem(InfoSeverity::Fatal,__FILE__, __LINE__, msg, ##__VA_ARGS__))
#define ERRMSG(msg, ...) Info::add(InfoItem(InfoSeverity::Error,__FILE__, __LINE__, msg, ##__VA_ARGS__))
#define WARNING(msg, ...) Info::add(InfoItem(InfoSeverity::Warning,__FILE__, __LINE__, msg, ##__VA_ARGS__))
#define PROGRESS(msg, ...) Info::add(InfoItem(InfoSeverity::Progress, "", 0, msg, ##__VA_ARGS__))
#define INFO(msg, ...) Info::add(InfoItem(InfoSeverity::Info, QString(), 0, msg, ##__VA_ARGS__))
#define TRACE(msg, ...) Info::add(InfoItem(InfoSeverity::Debug,__FILE__, __LINE__, msg, ##__VA_ARGS__))
#define DETAIL(msg, ...) Info::add(InfoItem(InfoSeverity::Detail,QString(), 0, msg, ##__VA_ARGS__))
#define ENTER(msg, ...) Info::add(InfoItem(InfoSeverity::Enter,__FILE__, __LINE__, "Entering " msg, ##__VA_ARGS__))
#define LEAVE(msg, ...) Info::add(InfoItem(InfoSeverity::Leave,__FILE__, __LINE__, "\\Leaving " msg, ##__VA_ARGS__))
#define DUMPVAR(var) Info::add(InfoItem(InfoSeverity::Debug, QString(), 0, "%1 = %2", #var, var))
#define DUMPVAL(sev,name,val) Info::add(InfoItem(sev, QString(), 0, "%1 = %2", name, val))
#define RETURN(rtn) if (!rtn.isNull()) Info::add(rtn);
#define FUNCTION(...) InfoFunction ddt_info_function_instance(Q_FUNC_INFO, __FILE__, __LINE__, ##__VA_ARGS__);
#define FNRETURN(var) ddt_info_function_instance.returnVar = var;
#define APPVERSION(ver) Info::applicationVersion(ver)
#define LIBVERSION(ver) Info::libraryVersion(ver)
