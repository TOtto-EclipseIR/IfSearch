#include "OptionSet.h"

#include <QtCore/QFileInfo>

#include "../eirCore/VariableSettingsList.h"
#include "Log.h"

OptionSet::OptionSet(QObject * parent)
    : VariableSettingsList(parent)
{
    FUNCTION();
    setObjectName("OptionSet");
}

void OptionSet::initiailize(const QString & orgName,
                            const QString & appName,
                            QStringList appArgs)
{
    FUNCTION(orgName, appName, appArgs.join(";"));

    VariableSettings * cmdLine_vsi = new VariableSettings(parent());
    exe_fi.setFile(appArgs.takeFirst());

    add("$" + orgName);
    add("$" + orgName + "/" + appName);
    add("%" + orgName);
    add("%" + orgName + "/" + appName);

    while ( ! appArgs.isEmpty())
    {
        QString arg = appArgs.takeFirst();
        if (arg.startsWith('/'))
            cmdLine_vsi->set(Variable::fromString(arg));
        if (arg.startsWith('%') || arg.startsWith('@'))
            add(arg);
        else
            args_qsl << arg;
    }

    if (cmdLine_vsi->isEmpty())
        delete cmdLine_vsi;
    else
        add(cmdLine_vsi);

    read();

    int updateMsec = value("Options/UpdateMsec").toInt();
    setUpdateMsec(updateMsec);
}

QFileInfo OptionSet::exeFileInfo(void) const
{
    return exe_fi;
}

QString OptionSet::arg(const int index) const
{
    return (index < 0 || index >= args_qsl.size())
            ? QString()
            : args_qsl.at(index);
}

QString OptionSet::takeFirstArg(void)
{
    return args_qsl.takeFirst();
}

int OptionSet::sizeArgs(void) const
{
    return args_qsl.size();
}

bool OptionSet::isEmptyArgs(void) const
{
    return args_qsl.isEmpty();
}

QStringList OptionSet::args(void) const
{
    return args_qsl;
}

