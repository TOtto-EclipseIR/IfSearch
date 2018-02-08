#include <Return.h>

using namespace DDT;

QMap<int, QString>			Return::messages;
QMap<int, DDT::Severity>	Return::severities;

const int Return::UnknownCode = 0xC0DEBAAD;
const int Return::TrueCode = 0xC0DE0001;
const int Return::FalseCode = 0xC0DE0000;
const int Return::ErrorStringCode = 0xC0DEEEEE;
const int Return::WindowsErrorCode = 0xC0DE4D24; // M$
const int Return::QSqlErrorCode = 0xC0DE5153; // QS
const int Return::QFileErrorCode = 0xC0DE5174; // Qt
const int Return::QFileWarningCode = 0xC0DE5154; // QT
const Return Return::True(Return::TrueCode);
const Return Return::False(Return::FalseCode);

Return::Return(int Code, const QVariant &  V1, const QVariant &  V2, const QVariant &  V3, const QVariant &  V4)
	: code(Code), severity(DDT::Null)
{
	if (0 == code)
		return;
	if ( ! messages.contains(code) || ! severities.contains(code))
		staticCtor();
	if ( ! messages.contains(code) || ! severities.contains(code))
		code = UnknownCode;
	message = messages[code];
	severity = severities[code];
	var[0] = V1, var[1] = V2, var[2] = V3, var[3] = V4;
}

Return::Return(const Return & that)
	: code(that.code), severity(that.severity)
{
	if (0 == code)
		return;

	if (messages.isEmpty())
		staticCtor();

	var[0] = that.var[0], var[1] = that.var[1], var[2] = that.var[2], var[3] = that.var[3];
	if ( ! messages.contains(code) || ! severities.contains(code))
		code = UnknownCode;
	message = messages[code];
	severity = severities[code];
}
		
Return Return::operator=(const Return & that)
{
	this->code = that.code,
	this->message = that.message,
	this->severity = that.severity,
	this->var[0] = that.var[0],
	this->var[1] = that.var[1],
	this->var[2] = that.var[2],
	this->var[3] = that.var[3];
	return *this;
}

void Return::staticCtor(void)
{
	add(UnknownCode, QObject::tr("Unknown Return Code: %1"), DDT::Unknown);
	add(TrueCode, QObject::tr("True"), DDT::Info);
	add(FalseCode, QObject::tr("False"), DDT::Info);
	add(ErrorStringCode, QObject::tr("Error: %1"), DDT::Error);
	add(WindowsErrorCode, QObject::tr("Windows System Error [%2]: %3"), DDT::Error);
	add(QSqlErrorCode, QObject::tr("SQL Engine Error: Number=%1 Driver=%2\n   Database=%3\n   SQL=%4"), DDT::Error);
	add(QFileErrorCode, QObject::tr("Error #%1 (%2) on file: %3"), DDT::Error);
	add(QFileWarningCode, QObject::tr("Error #%1 (%2) on file: %3"), DDT::Warning);
	add(NotImplemented, QObject::tr("Not Implemented: %1"), DDT::Error);
	add(NullPointer, QObject::tr("Null Pointer: %1"), DDT::Fatal);
	add(CantOpenFile, QObject::tr("Can't open file %1 for %2"), DDT::Error);
	add(ReturnXmlParse, QObject::tr("Error parsing xml in %1: %2 at %3,%4"), DDT::Error);
} // staticCtor()


void Return::clear(void)
{
	code = 0;
	severity = DDT::Null;
	var[0].clear();
	var[1].clear();
	var[2].clear();
	var[3].clear();
	message.clear();
} // clear()

void Return::add(const int Code, const QString & Msg, const DDT::Severity Sev)
{
	if ( ! messages.contains(Code))
	{
		messages[Code] = Msg;
		severities[Code] = Sev;
	}
}

QString Return::toString(void) const
{
	QString rtn;
	QString msg = messages.value(code);

	if (var[0].isNull())
		rtn = msg;
	else if (var[1].isNull())
		rtn = QObject::tr(qPrintable(msg)).arg(var[0].toString());
	else if (var[2].isNull())
		rtn = QObject::tr(qPrintable(msg)).arg(var[0].toString()).arg(var[1].toString());
	else if (var[3].isNull())
		rtn = QObject::tr(qPrintable(msg)).arg(var[0].toString()).arg(var[1].toString())
										  .arg(var[2].toString());
	else
		rtn = QObject::tr(qPrintable(msg)).arg(var[0].toString()).arg(var[1].toString())
										  .arg(var[2].toString()).arg(var[3].toString());
	return rtn;
}		

Return Return::qfileError(const QFile & file)
{
	return Return(QFileErrorCode, file.error(), file.errorString(), file.fileName());
} // qfileError()

Return Return::qfileWarning(const QFile & file)
{
	return Return(QFileWarningCode, file.error(), file.errorString(), file.fileName());
} // qfileWarning()

Return Return::errorString(const QString & errorMessage)
{
	return Return(ErrorStringCode, errorMessage);
}

Return Return::todo(const QString & string)
{
    return Return(NotImplemented, string);
}

#ifdef WINDOWS
#include <windows.h>
#include <winbase.h>
Return Return::windowsLastError(void)
{
	QString msg;
	TCHAR buf[1024] = L"\0";
	DWORD err = GetLastError();
	QString hex = QObject::tr("0x%1").arg(err, 8, 16, QChar('0'));

	if (FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, err, LANG_USER_DEFAULT, buf, 1024, NULL))
#ifdef UNICODE
		msg = QString::fromUtf16((ushort*)buf);
#else
		msg = QString::fromLocal8Bit(buf);
#endif
	else
		msg = QObject::tr("Unknown Windows Error").arg(err, 8, 16, QChar('0'));

	return Return(WindowsErrorCode, msg, (int)err, hex, msg);
} // windowsLastError()
#endif
