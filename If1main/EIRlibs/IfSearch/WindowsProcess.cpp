/** @file WindowsProcess.cpp
*	Definition of WindowsProcess class for DDT core library
*
*	@author	Dynamic DeZigns Technology
*
*/
#include <WindowsProcess.h>

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QLibrary>
#include <QPluginLoader>
#include <QProcess>
#include <QStringList>

//#include <StatusHandler.h>
#include <InfoMacros.h>

#ifdef WINDOWS
#undef ERROR
#include <windows.h>
#include <winbase.h>
#include <psapi.h>
#endif

using namespace DDT;
WindowsProcess::WindowsProcess(QObject * parent)
	: QObject(parent)
{
	DDT::Return::add(ReturnStatusNull, "DDTcore Null Status", DDT::Warning);
	DDT::Return::add(ReturnMemory, "%1 Memory: %2 Total, %3 Available (%4%)", DDT::Detail);
	DDT::Return::add(ReturnLoadFail, "Library Load Failed for %1", DDT::Warning);
}

WindowsProcess::~WindowsProcess()
{
}

#ifdef WINDOWS
Return WindowsProcess::loadLibrary(const QString & name)
{
	QLibrary * dll = new QLibrary(name, qApp); NULLPTR(dll);
	dll->setLoadHints(QLibrary::ResolveAllSymbolsHint);
	if (dll->load())
		return Return();
	else
		return Return(ReturnLoadFail, name);
} // loadLibrary()

Return WindowsProcess::loadLibraries(const QStringList & names)
{
	QString errorNames;
	foreach (QString name, names)
		if (loadLibrary(name).isWarn())
			errorNames += (errorNames.isEmpty() ? "" : ", ") + name;
	return errorNames.isEmpty() ? Return() : Return(ReturnLoadFail, errorNames);
} // loadLibraries()


Return WindowsProcess::loadPlugin(const QString & name)
{
	QPluginLoader * dll = new QPluginLoader(name, qApp); NULLPTR(dll);
	dll->setLoadHints(QLibrary::ResolveAllSymbolsHint);
	if (dll->load())
		return Return();
	else
		return Return(ReturnLoadFail, name);
} // loadPlugin()

Return WindowsProcess::loadPlugins(const QStringList & names)
{
	QString errorNames;
	foreach (QString name, names)
		if (loadPlugin(name).isWarn())
			errorNames += (errorNames.isEmpty() ? "" : ", ") + name;
	return errorNames.isEmpty() ? Return() : Return(ReturnLoadFail, errorNames);
} // loadPlugins()

int WindowsProcess::removeDirTree(const QString & dirName)
{
	QStringList qsl;
	qsl << "/S" << "/Q" << QDir::toNativeSeparators(dirName);
	return QProcess::execute("RMDIR", qsl);
}

#ifndef IJM
Return WindowsProcess::physicalMemory(void)
{
	MEMORYSTATUSEX mem;
	mem.dwLength = sizeof(mem);
	if ( ! ::GlobalMemoryStatusEx(&mem))
		return Return::windowsLastError();
	else
		return Return(ReturnMemory, "Physical", mem.ullTotalPhys >> 20, mem.ullAvailPhys >> 20,
						100.0 * (qreal)mem.ullAvailPhys / (qreal)mem.ullTotalPhys);
}
#endif

QList<quint32> WindowsProcess::pidsFor(const QString & name) const
{
	return mapNameToPid.values(name.toLower());
}
				
QString WindowsProcess::nameOfPid(const quint32 pid) const
{
	if (mapPidToName.contains(pid))
		return mapPidToName.value(pid);
	else
		return QString();
}

Return WindowsProcess::killSiblings(void)
{
	DDT::Return rtn = enumerate();
	RETURN(rtn);
	if (rtn.isError())
		return rtn;
	else
	{
		qint64 pid = qApp->applicationPid();
		QString appPath = qApp->applicationFilePath();
		INFO(tr("%1 running at PID %2"), appPath, pid);

		QFileInfo fi(appPath);
		QList<quint32> pids = pidsFor(fi.fileName());
		foreach(quint32 p, pids)
			if (pid != p)
			{
				rtn = killPid(p);
				RETURN(rtn);
				if ( ! rtn.isError())
				{
					INFO(tr("Killed PID %1"), p);
				}
			}
	}
	return rtn;
}
		

Return WindowsProcess::killPid(quint32 pid)
{
	HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
	if ( ! hProcess)
		return Return::windowsLastError();

	if ( ! TerminateProcess(hProcess, 0))
		return Return::windowsLastError();

	return Return();
}

Return WindowsProcess::enumerate(void)
{
#ifdef IJM
	return Return(Return::NotImplemented, "WindowsProcess::enumerate()");
#else
	DWORD pids[1024], nPid;

	if ( ! EnumProcesses(pids, sizeof(pids), &nPid))
		return Return::windowsLastError();

	nPid /= sizeof(DWORD);
	mapPidToName.clear();
	mapNameToPid.clear();
	for (int i = 0; i < nPid; i++)
	{
		TCHAR name[MAX_PATH] = TEXT("{unknown}");
		HMODULE hMod;
		DWORD dw;
		HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pids[i]);
		if (hProcess)
		{
			if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &dw))
			{
				GetModuleBaseName(hProcess, hMod, name, MAX_PATH);
			}
		}

#ifdef UNICODE
		mapPidToName.insert(pids[i], QString::fromUtf16((ushort*)name));
		mapNameToPid.insert(QString::fromUtf16((ushort*)name).toLower(), pids[i]);
#else
		mapPidToName.insert(pids[i], QString::fromLocal8Bit(name));
		mapNameToPid.insert(QString::fromLocal8Bit(name).toLower(), pids[i]);
#endif
	}

	return Return();
#endif
}

#endif // def WINDOWS
