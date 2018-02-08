/*! @file BaseLog.h Declarations for BaseLog class
*
*/
#ifndef BASELOG_H
#define BASELOG_H
#include "eirBase.h"

#include "MemoryMonitor.h"

#define BLOG_MIN(sev)       { BaseLog::setMinimum(sev); }
#define BLOG_FATAL(sev)     { BaseLog::setFatal(sev); }
#define BLOG(sev, fmt, ...) { BaseLog bli(sev, fmt, ##__VA_ARGS__); }

#define BFUNCTION(...)      BLOG(Severity::Enter, "%1(%2, %3, %4)", Q_FUNC_INFO, ##__VA_ARGS__)
#define BFNRETURN(var)      BLOG(Severity::Leave, "%1 -> %2", Q_FUNC_INFO, var)
#define BALLOC(fmt, ...)    BLOG(Severity::Alloc,     fmt, ##__VA_ARGS__)
#define BLOGMSG(fmt, ...)   BLOG(Severity::LogMsg,    fmt, ##__VA_ARGS__)
#define BTODO(var)          BLOG(Severity::ToDo,    "TODO: %1 in %4 [%2:%3]", var, __FILE__, __LINE__, Q_FUNC_INFO)
#define BMUSTDO(var)        BLOG(Severity::Stern, "MUSTDO: %1 in %4 [%2:%3]", var, __FILE__, __LINE__, Q_FUNC_INFO)
#define BDETAIL(fmt, ...)   BLOG(Severity::Detail,    fmt, ##__VA_ARGS__)
#define BWARNING(fmt, ...)  BLOG(Severity::Warning,   fmt, ##__VA_ARGS__)
#define BTRACE(fmt, ...)    BLOG(Severity::Trace,     fmt, ##__VA_ARGS__)
#define BMEMORY(fmt, ...)   BLOG(Severity::Memory,    fmt, ##__VA_ARGS__)
#define NOBTRACE(fmt, ...)  { (void)fmt; }
#define BDUMPVAR(var)       BLOG(Severity::DumpVar, "%1 = {%2}", #var, var)

#define BEXPECTEQ(a, b)     { if (a != b) BLOG(Severity::Expect, "EXPECT Failed %1 (%2) not equal %3 (%4)", #a, a, #b, b) }
#define BEXPECTNE(a, b)     { if (a == b) BLOG(Severity::Expect, "EXPECT Failed %1 (%2)  is equal %3 (%4)", #a, a, #b, b) }
#define BASSERT(exp)        { if ( ! exp) BLOG(Severity::Assert, "%1[%2] Assert Failed: %3", __FILE__, __LINE__, #exp) }
#define BOBJPOINTER(typ, p) { if (0 == qobject_cast<typ *>(p)) BLOG(Severity::Pointer, "%1[%2]: %3 is not a %4 QObject pointer", __FILE__, __LINE__, #p, #typ); }
#define BNULPOINTER(p)      { if (0 == p) BLOG(Severity::Pointer, "%1[%2]: %3 is not a valid pointer", __FILE__, __LINE__, #p); }

#define BOBJNAME(obj)       ((obj) ? ((obj)->objectName().isEmpty() ? QString((obj)->metaObject()->className()) : (obj)->objectName()) : "{orphan}")
#define BCONNECT(a,b,c,d)   { if ( !    connect(a,b,c,d)) BLOG(Severity::Error, "FAILED connect: %s %s %s %s", #a, #b, #c, #d); }
#define BDISCONNECT(a,b,c,d){ if ( ! disconnect(a,b,c,d)) BLOG(Severity::Warning, "FAILED disconnect: %s %s %s %s", #a, #b, #c, #d); }
#define BEMIT(exp)          { BLOG(Severity::Signal, "emit %1->%2", BOBJNAME(this), #exp); emit exp; }
#define BUSERVAR(var, value) QVariant var; var.setValue(value);

//#define MEMMGR(cmd, ...)    { MemoryMonitor::instance()->cmd( ##_VA_LIST_ ); }
#define LMEMNEW(typ, var, parent) typ * var = new typ; MemoryMonitor::instance()->newMemory(var, sizeof(typ), parent, #var, #typ, Q_FUNC_INFO, __FILE__, __LINE__)
#define MEMNEW(typ, var, parent) var = new typ; MemoryMonitor::instance()->newMemory(var, sizeof(typ), parent, #var, #typ, Q_FUNC_INFO, __FILE__, __LINE__)
#define LMEMNEWA(typ, var, parent, ...) typ * var = new typ( __VA_ARGS__ ); MemoryMonitor::instance()->newMemory(var, sizeof(typ), parent, #var, #typ, Q_FUNC_INFO, __FILE__, __LINE__)
#define MEMNEWA(typ, var, parent, ...) var = new typ( __VA_ARGS__ ); MemoryMonitor::instance()->newMemory(var, sizeof(typ), parent, #var, #typ, Q_FUNC_INFO, __FILE__, __LINE__)
#define MEMFREE(ptr)        { MemoryMonitor::instance()->freeMemory(ptr, Q_FUNC_INFO); }
#define MEMBLOG(det)        { MemoryMonitor::instance()->blogReport(det); }

#include <QSharedDataPointer>
#include <QVariant>

#include "Severity.h"

class QFile;

void EIRBASESHARED_EXPORT myMsgHandler(QtMsgType t,
                                       const QMessageLogContext & context,
                                       const QString & pc);

/*! @class  BaseLog
 *  @brief  Bare-bone logging facility for early libraries
 *
 *  @note   The static variables minSeverity_i and fatalSeverity_i
 *          are implemented as integer values because the unlying
 *          Enumeration and Named<int> use a QMap<> facility
 *          that can't be used in static initialization.
 *
 *  @todo   BLOG_PUSH() and BLOG_PULL() to stack minimum and fatal severities.
 *
 *  @todo   Determine if we are done with static initialization
 *          and don't instanciate BaseLog in BLOG macro.
 *
 */
class EIRBASESHARED_EXPORT BaseLog
{
public:
    BaseLog(void);
    BaseLog(const Severity severity,
            const QString & format,
            const QVariant & value1=QVariant(),
            const QVariant & value2=QVariant(),
            const QVariant & value3=QVariant(),
            const QVariant & value4=QVariant());

public: // static methods
    static void setMinimum(const Severity severity);
    static void setFatal(const Severity severity);
    static Severity minimumSeverity(void);
    static Severity fatalSeverity(void);
    static bool isMinimum(const Severity severity);
    static bool isFatal(const Severity severity);
    static QFile * blogFile(void);
    static QString blogFileName(void);
    static void setFile(const QString & fileName=QString());
    static void resetFile(void);
    static QString fillString(const QString & format,
                              const QVariant & value1,
                              const QVariant & value2,
                              const QVariant & value3,
                              const QVariant & value4);
    static void write(const Severity severity,
                      const QString & message);
    static void flush(void);
    static QUrl url(void);

private: // static members
    static int minSeverity_i;
    static int fatalSeverity_i;
    static QFile * blog_file;
};

#endif // BASELOG_H
