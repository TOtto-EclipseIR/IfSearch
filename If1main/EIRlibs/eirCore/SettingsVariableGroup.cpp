#include "SettingsVariableGroup.h"

#include <QtCore/QFileSystemWatcher>
#include <QtCore/QTimer>
#include <QtCore/QSettings>

#include "../eirBase/BasicLog.h"

SettingsVariableGroup::SettingsVariableGroup(QSettings * settings,
                                             const QString & group,
                                             const Flags f)
    : VariableGroup(group)
    , _settings(settings)
{
    qDebug() << "SettingsVariableGroup ctor"
             << _settings->objectName()
             << sectionId() << f;
    OBJPOINTER(QSettings, _settings);
    if (f & autoClear)
        removeAll();
    else if (f & autoRead)
        read();
    if (f & autoWrite)
        write();
}

QString SettingsVariableGroup::settingsFileName(void) const
{
    OBJPOINTER(QSettings, _settings);
    return _settings->fileName();
}

QString SettingsVariableGroup::settingsOrgName(void) const
{
    OBJPOINTER(QSettings, _settings);
    return _settings->organizationName();
}

QString SettingsVariableGroup::settingsAppName(void) const
{
    OBJPOINTER(QSettings, _settings);
    return _settings->applicationName();
}

void SettingsVariableGroup::remove(const VariableId & id)
{
    OBJPOINTER(QSettings, _settings);
    _settings->remove(id);
    VariableGroup::remove(id);
}

void SettingsVariableGroup::removeAll(void)
{
    OBJPOINTER(QSettings, _settings);
    if ( ! sectionId().isNull())
        _settings->beginGroup(sectionId());
    foreach(QString key, _settings->allKeys())
        remove(key);
    if ( ! sectionId().isNull())
        _settings->endGroup();
}

QVariant SettingsVariableGroup::read(VariableId id)
{
    OBJPOINTER(QSettings, _settings);
    qDebug() << "Reading" << id
             << "from" << _settings->applicationName();
    id.prepend(sectionId());
    QVariant setting = _settings->value(id);
    if (contains(id))
        set(id, setting);
    else
        add(Variable(id, setting));
    return setting;
}
void SettingsVariableGroup::write(VariableId id, QVariant var)
{
    OBJPOINTER(QSettings, _settings);
    qDebug() << "Writing" << id
             << "to" << _settings->applicationName();
    id.prepend(sectionId());
    _settings->setValue(id, var);
    if (contains(id))
        set(id, var);
    else
        add(Variable(id, var));
}

int SettingsVariableGroup::read(void)
{
    // qDebug() << objectName() << "Reading from" << _settings->applicationName();
    OBJPOINTER(QSettings, _settings);
    int changed = 0;
    if ( ! sectionId().isNull())
        _settings->beginGroup(sectionId());

    QStringList keys = _settings->allKeys();
    foreach(QString key, keys)
    {
        if (contains(key))
        {
            QVariant current = value(key);
            QVariant setting = _settings->value(key);
            if (setting != current)
            {
                set(key, setting);
                ++changed;
            }
        }
        else
        {
            VariableGroup::add(Variable(key, _settings->value(key)));
            ++changed;
        }
    }

    if ( ! sectionId().isNull())
        _settings->endGroup();
    return changed;
} // read()

int SettingsVariableGroup::write(void)
{
    OBJPOINTER(QSettings, _settings);
    qDebug() << sectionId() << "Writing to" << _settings->applicationName();
    int changed = 0;
    if ( ! sectionId().isNull())
        _settings->beginGroup(sectionId());

    VariableIdList ids = this->ids();
    foreach(VariableId id, ids)
    {
        QVariant current = value(id);
        if (_settings->contains(id))
        {
            if (current != _settings->value(id))
            {
                _settings->setValue(id, current);
                qDebug() << _settings->applicationName() << id << "<" << current;
                ++changed;
            }
        }
        else
        {
            _settings->setValue(id, current);
            qDebug() << _settings->applicationName() << id << "+<" << current;
            ++changed;
        }
    }

    if ( ! sectionId().isNull())
        _settings->endGroup();
    return changed;
} // write()
