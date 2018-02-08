/** @file InfoMacros.h
*	Declaration of macros for Info class for DDT core library
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
#ifdef TRACE_H
#pragma warning "Don't mix Trace and Info"
#endif

#include <Info.h>

#define	ASSERT(test) ((void)((test) ? DDT::Info::add(DDT::InfoItem(DDT::InfoSeverity::Fatal,__FILE__, __LINE__, "FATAL ERROR: %1", #test)) : 0))
#define NULLPTR(ptr) if(!ptr) DDT::Info::add(DDT::InfoItem(DDT::InfoSeverity::Fatal,__FILE__, __LINE__, "FATAL ERROR: %1 is NULL", #ptr));
#define EXPECT(value, test) (DDT::Info::expect(#value, value, #test, test, __FILE__, __LINE__))
#define EXPECTNOT(value, test) (DDT::Info::expectNot(#value, value, #test, test, __FILE__, __LINE__))
#define FATAL(msg, ...) DDT::Info::add(DDT::InfoItem(DDT::InfoSeverity::Fatal,__FILE__, __LINE__, msg, ##__VA_ARGS__))
#define ERROR(msg, ...) DDT::Info::add(DDT::InfoItem(DDT::InfoSeverity::Error,__FILE__, __LINE__, msg, ##__VA_ARGS__))
#define WARNING(msg, ...) DDT::Info::add(DDT::InfoItem(DDT::InfoSeverity::Warning,__FILE__, __LINE__, msg, ##__VA_ARGS__))
#define PROGRESS(msg, ...) DDT::Info::add(DDT::InfoItem(DDT::InfoSeverity::Progress, "", 0, msg, ##__VA_ARGS__))
#define INFO(msg, ...) DDT::Info::add(DDT::InfoItem(DDT::InfoSeverity::Info, QString(), 0, msg, ##__VA_ARGS__))
#define TRACE(msg, ...) DDT::Info::add(DDT::InfoItem(DDT::InfoSeverity::Debug,__FILE__, __LINE__, msg, ##__VA_ARGS__))
#define DETAIL(msg, ...) DDT::Info::add(DDT::InfoItem(DDT::InfoSeverity::Detail,__FILE__, __LINE__, msg, ##__VA_ARGS__))
#define ENTER(msg, ...) DDT::Info::add(DDT::InfoItem(DDT::InfoSeverity::Enter,__FILE__, __LINE__, "Entering " msg, ##__VA_ARGS__))
#define LEAVE(msg, ...) DDT::Info::add(DDT::InfoItem(DDT::InfoSeverity::Leave,__FILE__, __LINE__, "\\Leaving " msg, ##__VA_ARGS__))
#define DUMPVAR(var) DDT::Info::add(DDT::InfoItem(DDT::InfoSeverity::Debug, QString(), 0, "%1 = %2", #var, var))
#define DUMPVAL(sev,name,val) DDT::Info::add(DDT::InfoItem(sev, QString(), 0, "%1 = %2", name, val))
#define RETURN(rtn) if (!rtn.isNull()) DDT::Info::add(rtn);
#define FUNCTION(...) DDT::InfoFunction ddt_info_function_instance(Q_FUNC_INFO, __FILE__, __LINE__, ##__VA_ARGS__);
#define FNRETURN(var) ddt_info_function_instance.returnVar = var;
#define APPVERSION(ver) DDT::Info::applicationVersion(ver)
#define LIBVERSION(ver) DDT::Info::libraryVersion(ver)
