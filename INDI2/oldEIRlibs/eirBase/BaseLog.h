#ifndef BASELOG_H
#define BASELOG_H
#include "eirBase.h"

#define BLOG_MIN(sev)       { BaseLog::setMinimum(sev); }
#define BLOG_FATAL(sev)     { BaseLog::setFatal(sev); }
#define BLOG(sev, fmt, ...) { BaseLog bli(sev, fmt, ##__VA_ARGS__); }

#define BTODO(fmt, ...)     BLOG(Severity::ToDo, "TODO: " fmt, ##__VA_ARGS__)
#define BDETAIL(fmt, ...)   BLOG(Severity::Detail,    fmt, ##__VA_ARGS__)
#define BWARNING(fmt, ...)  BLOG(Severity::Warning,   fmt, ##__VA_ARGS__)
#define BTRACE(fmt, ...)    BLOG(Severity::Trace,     fmt, ##__VA_ARGS__)
#define NOBTRACE(fmt, ...)  { (void)fmt; }
#define BDUMPVAR(var)       BLOG(Severity::DumpVar, "%1 = {%2}", #var, var)

#define BASSERT(exp)        { if ( ! exp) BLOG(Severity::Assert, "%1[%2] Assert Failed: %3", __FILE__, __LINE__, #exp) }
#define BOBJPOINTER(typ, p) { if (0 == qobject_cast<typ *>(p)) BLOG(Severity::Pointer, "%1[%2]: %3 is not a %4 QObject pointer", __FILE__, __LINE__, #p, #typ); }

#define BOBJNAME(obj)       ((obj) ? ((obj)->objectName().isEmpty() ? QString((obj)->metaObject()->className()) : (obj)->objectName()) : "{orphan}")
#define BCONNECT(a,b,c,d)   { if ( !    connect(a,b,c,d)) BLOG(Severity::Error, "FAILED connect: %s %s %s %s", #a, #b, #c, #d); }
#define BDISCONNECT(a,b,c,d){ if ( ! disconnect(a,b,c,d)) BLOG(Severity::Warning, "FAILED disconnect: %s %s %s %s", #a, #b, #c, #d); }
#define BEMIT(exp)          { BLOG(Severity::Signal, "emit %1->%2", BOBJNAME(this), #exp); emit exp; }

#include <QSharedDataPointer>

#include "Severity.h"

class QFile;

class BaseLogData;

void EIRBASESHARED_EXPORT myMsgHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

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
    BaseLog(const Severity severity,
                 const QString & format,
                 const QVariantList & qvl);
    BaseLog(const BaseLog & rhs);
    BaseLog & operator=(const BaseLog & rhs);
    ~BaseLog();

public: // static methods
    static void setMinimum(const Severity severity);
    static void setFatal(const Severity severity);
    static Severity fatalLevel(void);
    static bool isMinimum(const Severity severity);
    static bool isFatal(const Severity severity);
    static QFile * blogFile(void);
    static void setFile(const QString & fileName=QString());
    static void resetFile(void);

private:
    void staticCtor(void);

private:
    QSharedDataPointer<BaseLogData> data;

private: // static members
    static Severity min_severity;
    static Severity fatal_severity;
    static QFile * blog_file;
};

#endif // BASELOG_H


/*
#include "BasicLogItem.h"
#include "Severity.h"

#define BLOGVL(sev, fmt, q) { BasicLogItem bli(sev, fmt, q); }

#define BALLOC(fmt, ...)    BLOG(Severity::Alloc,     fmt, ##__VA_ARGS__)
#define BDATA(fmt, ...)     BLOG(Severity::Data,      fmt, ##__VA_ARGS__)
#define BDUMP(fmt, ...)     BLOG(Severity::Dump,      fmt, ##__VA_ARGS__)
#define BDETAIL(fmt, ...)   BLOG(Severity::Detail,    fmt, ##__VA_ARGS__)
#define BSTATE(fmt, ...)    BLOG(Severity::State,     fmt, ##__VA_ARGS__)
#define BINFO(fmt, ...)     BLOG(Severity::Info,      fmt, ##__VA_ARGS__)
#define BLOGMSG(fmt, ...)   BLOG(Severity::LogMsg,    fmt, ##__VA_ARGS__)
#define BPROGRESS(fmt, ...) BLOG(Severity::Progress,  fmt, ##__VA_ARGS__)
#define BNOTICE(fmt, ...)   BLOG(Severity::Notice,    fmt, ##__VA_ARGS__)
#define BSTERN(fmt, ...)    BLOG(Severity::Stern,     fmt, ##__VA_ARGS__)
#define BERROR(fmt, ...)    BLOG(Severity::Error,     fmt, ##__VA_ARGS__)
#define BFATAL(fmt, ...)    BLOG(Severity::Fatal,     fmt, ##__VA_ARGS__)

#define BWATCH(exp)         { if ( ! exp) BLOG(Severity::Watch,  "%1[%2] Watch  Failed: %3", __FILE__, __LINE__, #exp); }

#define BGVIPOINTER(typ, p) { if (0 == qgraphicsitem_cast<typ *>(p)) BLOG(Severity::Pointer, "%1[%2]: %3 is not a %4 QGraphicsItem pointer", __FILE__, __LINE__, #p, #typ)


  */
