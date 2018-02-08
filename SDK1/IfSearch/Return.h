/** @file Return.h
*	Declaration of Return class for DDT core library
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

#include <QFile>
#include <QMap>
#include <QString>
#include <QVariant>

#include <DDTcore.h>

class DDTCORE_EXPORT Return
{
    friend class StatusHandler;

public: // static variables
    const static Return True;
    const static Return False;
    const static int UnknownCode;
    const static int TrueCode;
    const static int FalseCode;
    const static int WindowsErrorCode;
    const static int ErrorStringCode;
    const static int QFileErrorCode;
    const static int QFileWarningCode;
    const static int QSqlErrorCode;

    enum Returns
    {
        NotImplemented = 0xC0DE8001,
        NullPointer,
        CantOpenFile,
        ReturnXmlParse
    };

public:
    Return(const int code=0, const QVariant & V1=QVariant(), const QVariant & V2=QVariant(),
           const QVariant & V3=QVariant(), const QVariant & V4=QVariant());
    Return(const Return & that);
    Return operator=(const Return & that);
    ~Return(void) {}
    void clear(void);
    bool isNull(void) const { return ! code; }
    bool isTrue(void) const { return TrueCode == code; }
    bool isFalse(void) const { return FalseCode == code; }
    bool isWarn(void) const { return severity >= Warning; }
    bool isError(void) const { return severity >= Error; }
    bool isFatal(void) const { return severity >= Fatal; }
    int returnCode(void) const { return code; }
    QString toString(void) const;
    QVariant operator[](int x) const;
    bool is(const int c) const { return code == c; }
    Severity ddtSeverity(void) const { return severity; }
    QString msg(void) const { return message; }

public: // static functions
    static void add(const int code, const QString & msg, const Severity sev);
#ifdef WINDOWS
    static Return windowsLastError(void);
#endif
    static Return qfileError(const QFile & file);
    static Return qfileWarning(const QFile & file);
    static Return errorString(const QString & errorMessage);
    static Return todo(const QString & string);

private:
    Severity severity;
    int code;
    QVariant var[4];
    QString message;

private: // static functions
    void staticCtor(void);

private: // static variables
    static QMap<int, QString>	messages;
    static QMap<int, Severity>	severities;
}; // class Return
