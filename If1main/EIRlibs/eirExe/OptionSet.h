#ifndef OPTIONSET_H
#define OPTIONSET_H
#include "eirExe.h"

#include <QtCore/QObject>
#include "../eirCore/VariableSettingsList.h"

#include <QtCore/QFileInfo>

#include "../eirCore/VariableIdList.h"


class EIREXESHARED_EXPORT OptionSet
        : public VariableSettingsList
{
    Q_OBJECT

public:
    explicit OptionSet(QObject * parent=0);
    void initiailize(const QString & orgName,
                                const QString & appName,
                                QStringList appArgs);

    QFileInfo exeFileInfo(void) const;
    QString arg(const int index) const;
    QString takeFirstArg(void);
    int sizeArgs(void) const;
    bool isEmptyArgs(void) const;
    QStringList args(void) const;

signals:
    
public slots:
    
private:
    QFileInfo exe_fi;
    QStringList args_qsl;
};

#endif // OPTIONSET_H
