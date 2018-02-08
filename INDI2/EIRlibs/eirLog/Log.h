/*! @file Log.h Log.h contains preprocessor macros to using the INDI logging facility.
  *
  * @todo Add MUSTDO macro and class Severity level for a fatal TODO
  *
  * @todo Add QList<LogItem> todo_list;
  * to ignore duplicated reports of the same todo item
  *
  */
#ifndef LOG_H
#define LOG_H
#ifndef __MODULE__
# pragma Compile_Time_Warning (true, "__MODULE__ not defined")
# define __MODULE__ "{unknown}"
#endif

#include <QtCore/QTimer>

#include <eirBase/Severity.h>

#include "Logger.h"
#include "LogFunction.h"
#include "LogItem.h"

#define LOG(exp) {if (Logger::instance()) Logger::instance()->exp;}
#define LOGADD(sev, fmt, ...)   LOG(add(LogItem(sev, __MODULE__, __FILE__, __LINE__, Q_FUNC_INFO, fmt, ##__VA_ARGS__)))
#define OBJNAME(obj) ((obj) ? ((obj)->objectName().isEmpty() ? QString((obj)->metaObject()->className()) : (obj)->objectName()) : "{orphan}")

#define ALLOC(fmt, ...)     LOGADD(Severity::Alloc,     fmt, ##__VA_ARGS__)
#define DATA(fmt, ...)      LOGADD(Severity::Data,      fmt, ##__VA_ARGS__)
#define DUMP(fmt, ...)      LOGADD(Severity::Dump,      fmt, ##__VA_ARGS__)
#define DETAIL(fmt, ...)    LOGADD(Severity::Detail,    fmt, ##__VA_ARGS__)
#define TRACE(fmt, ...)     LOGADD(Severity::Trace,     fmt, ##__VA_ARGS__)
#define NOTRACE(fmt, ...)   { (void)fmt; }
#define STATE(fmt, ...)     LOGADD(Severity::State,     fmt, ##__VA_ARGS__)
#define INFO(fmt, ...)      LOGADD(Severity::Info,      fmt, ##__VA_ARGS__)
#define LOGMSG(fmt, ...)    LOGADD(Severity::LogMsg,    fmt, ##__VA_ARGS__)
#define PROGRESS(fmt, ...)  LOGADD(Severity::Progress,  fmt, ##__VA_ARGS__)
#define TODO(txt)           LOGADD(Severity::ToDo,    "TODO: %1 in %4 [%2:%3]", txt, __FILE__, __LINE__, Q_FUNC_INFO)
#define MUSTDO(txt)         LOGADD(Severity::Stern, "MUSTDO: %1 in %4 [%2:%3]", txt, __FILE__, __LINE__, Q_FUNC_INFO)
#define NOTICE(fmt, ...)    LOGADD(Severity::Notice,    fmt, ##__VA_ARGS__)
#define WARNING(fmt, ...)   LOGADD(Severity::Warning,   fmt, ##__VA_ARGS__)
#define STERN(fmt, ...)     LOGADD(Severity::Stern,     fmt, ##__VA_ARGS__)
#define ERROR(fmt, ...)     LOGADD(Severity::Error,     fmt, ##__VA_ARGS__)
#define FATAL(fmt, ...)     LOGADD(Severity::Fatal,     fmt, ##__VA_ARGS__)

#define DUMPVAR(var)        LOGADD(Severity::DumpVar, "%1 = {%2}", #var, var)
#define DUMPVARHEX(var)     LOG(dumpHex(__MODULE__, __FILE__, __LINE__, Q_FUNC_INFO, #var, &var, sizeof(var)))
#define DUMPHEX(ptr, bytes) LOG(dumpHex(__MODULE__, __FILE__, __LINE__, Q_FUNC_INFO, #ptr, ptr, bytes))
#define RESULT(res)         LOG(add(res))
#define RESULTS(rset)       LOG(add(rset))
#define DUMPVSET(vset)      LOG(dump(vset))
#define DUMPVSL(vsl)        LOG(dump(vsl))

#define WATCH(exp)          { if ( ! exp) LOGADD(Severity::Watch,  "Watch Failed:",     #exp); }
#define ASSERT(exp)         { if ( ! exp) LOGADD(Severity::Assert, "Assertion Failed:", #exp); }

#define EXPECT(exp)         (Logger::instance() ? Logger::instance()->expect(__MODULE__, __FILE__, __LINE__, Q_FUNC_INFO, false, exp, #exp) : exp)
#define EXPECTNOT(exp)      (Logger::instance() ? Logger::instance()->expect(__MODULE__, __FILE__, __LINE__, Q_FUNC_INFO, true,  exp, #exp) : exp)
#define EXPECTEQ(v1, v2)    (Logger::instance() ? Logger::instance()->expect(__MODULE__, __FILE__, __LINE__, Q_FUNC_INFO, false, (v1==v2), #v1, #v2) : (v1==v2))
#define EXPECTNE(v1, v2)    (Logger::instance() ? Logger::instance()->expect(__MODULE__, __FILE__, __LINE__, Q_FUNC_INFO, true,  (v1==v2), #v1, #v2) : (v1==v2))

#define EMIT(exp)           { LOGADD(Severity::Signal, "emit %1->%2", OBJNAME(this), #exp); emit exp; }
#define FUNCTION(...)       LogFunction log_function_instance(false, __MODULE__, __FILE__, __LINE__, Q_FUNC_INFO, ##__VA_ARGS__)
#define FNSLOT(...)         LogFunction log_function_instance(true,  __MODULE__, __FILE__, __LINE__, Q_FUNC_INFO, ##__VA_ARGS__)
#define FNRETURN(var)       log_function_instance.setReturn(var)
#define FNMSECS()           log_function_instance.msecs()
#define SHOOT(ms, op, slot) { LOGADD(Severity::Signal, "Timer: %2->%3 in %1ms", #ms, OBJNAME(op), #slot); QTimer::singleShot(ms, op, slot); }

#define LOCAL_NEW(TYPE, NAME, ...) TYPE NAME = new TYPE( ##__VA_ARGS__ ); if ( ! NAME) LOGADD(Severity::Memory, "Allocation Failure: %1 byte %2 for %3", sizeof(TYPE), #TYPE, #NAME);
#define NEW(TYPE, NAME, ...) NAME = new TYPE( ##__VA_ARGS__ ); if ( ! NAME) LOGADD(Severity::Memory, "Allocation Failure: %1 byte %2 for %3", sizeof(TYPE), #TYPE, #NAME);
#define NULPOINTER(ptr)          { if ( ! ptr) LOGADD(Severity::Pointer, "[0x%1] %2 is a null pointer", QString::number((qptrdiff)ptr, 16), #ptr); }
#define OBJPOINTER(typ, ptr)     { if (0 == qobject_cast<typ *>(ptr)) LOGADD(Severity::Pointer, "[0x%1] %2 is not a %3 QObject pointer", QString::number((qptrdiff)ptr, 16), #ptr, #typ); }
#define GVIPOINTER(typ, ptr)     { if (0 == qgraphicsitem_cast<typ *>(ptr)) LOGADD(Severity::Pointer, "[0x%1] %2 is not a %3 QGraphicsItem pointer", QString::number((qptrdiff)ptr, 16), #ptr, #typ); }
#define CONNECT(a, b, c, d)      { if (QObject::connect(a,b,c,d)) LOGADD(Severity::Signal, "Connected: %1->%2 to %3->%4", OBJNAME(a), #b, OBJNAME(c), #d) else LOGADD(Severity::Error, "FAILED connect: %1 %2 %3 %4", #a, #b, #c, #d); }
#define DISCONNECT(a, b, c, d)   { if (QObject::disconnect(a,b,c,d)) LOGADD(Severity::Signal, "Disconnected: %1->%2 to %3->%4", #a, #b, #c, #d) else LOGADD(Severity::Warning, "FAILED disconnect: %1 %2 %3 %4", #a, #b, #c, #d); }

#endif // LOG_H
