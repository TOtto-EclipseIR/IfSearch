#ifndef SETTINGSVARIABLEGROUP_H
#define SETTINGSVARIABLEGROUP_H
#include "eirTypes.h"

#include "VariableGroup.h"

#include <QtCore/QMultiMap>
class QSettings;

#include "VariableId.h"


class EIRTYPESSHARED_EXPORT SettingsVariableGroup : public VariableGroup
{
public:
    enum Flag
    {
        autoRead    = 0x00000001,
        autoClear   = 0x00000002,
        autoWrite   = 0x00000004,
    };
    Q_DECLARE_FLAGS(Flags, Flag)

public:
    explicit SettingsVariableGroup(QSettings * settings,
                                   const QString & group=QString(),
                                   const Flags f=(Flags)(autoRead|autoWrite));
    QString settingsFileName(void) const;
    QString settingsOrgName(void) const;
    QString settingsAppName(void) const;
    void remove(const VariableId & id);
    void removeAll(void);
    int read(void);
    QVariant read(VariableId id);
    void write(VariableId id, QVariant var);
    int write(void);

private:
    QSettings * _settings;
};
Q_DECLARE_OPERATORS_FOR_FLAGS(SettingsVariableGroup::Flags)

#endif // SETTINGSVARIABLEGROUP_H
