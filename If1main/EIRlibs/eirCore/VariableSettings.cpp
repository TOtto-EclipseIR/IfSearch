#include "VariableSettings.h"


#include <QtDebug>

#include "../eirBase/BaseLog.h"
#include "VariableIdList.h"

VariableSettings::VariableSettings(QObject * parent)
    : QObject(parent)
    , VariableSet("VariableSettings")
    , _settings(new QSettings(parent))
{
    setObjectName("VariableSettings");
}

VariableSettings::VariableSettings(const QString & orgName,
                                   const QString & appName,
                                   QObject * parent)
    : QObject(parent)
    , _settings(new QSettings(orgName, appName, this))
{
    setName(settingName());
    setObjectName(settingName());
}

VariableSettings::VariableSettings(const QSettings::Format format,
                 const QString & fileName,
                 QObject * parent)
    : QObject(parent)
    , _settings(new QSettings(fileName, format, this))
{
    setName(settingName());
    setObjectName(settingName());
}

VariableSettings::VariableSettings(QString string,
                                   QObject * parent)
    : QObject(parent)
    , _settings(0)
{
    if (string.startsWith('@'))
    {
        QString fileName(string.mid(1));
        QFileInfo fi(fileName);
        QSettings::Format format = QSettings::NativeFormat;
        if ("ini" == fi.suffix()) format = QSettings::IniFormat;
        _settings = new QSettings(fileName, format, parent);
        return;
    }

    if (string.startsWith('%') | string.startsWith("$"))
    {
        QSettings::Scope scope = string.startsWith('%')
                                    ? QSettings::UserScope
                                    : QSettings::SystemScope;
        QString orgName;
        QString appName;

        string = string.mid(1);
        int x = string.indexOf('/');
        if (x < 0) // no slash
        {
            orgName = "EclipseIR";
            appName = string;
        }
        else
        {
            orgName = string.left(x);
            appName = string.mid(x+1);
        }

        _settings = new QSettings(scope, orgName, appName, parent);
        return;
    }

    _settings = new QSettings(parent);
    setName(string);
    setObjectName(string);
}

VariableSettings::VariableSettings(QSettings * settings,
                              const QString & group,
                              QObject * parent)
    : QObject(parent)
    , VariableSet(group.isEmpty() ? "VariableSettings" : group)
    , _settings(0)
{
    if (settings->fileName().isEmpty())
        _settings = new QSettings(settings->format(),
                                  settings->scope(),
                                  settings->organizationName(),
                                  settings->applicationName(),
                                  parent);
    else
        _settings = new QSettings(settings->fileName(),
                                  settings->format(),
                                  parent);
    BOBJPOINTER(QSettings, _settings);
    setName(settingName());
    setObjectName(settingName());

    _settings->beginGroup(group);
    setName(settingName());
    setObjectName(settingName());
}

QString VariableSettings::orgName(void) const
{
    BOBJPOINTER(QSettings, _settings);
    return _settings->organizationName();
}

QString VariableSettings::appName(void) const
{
    BOBJPOINTER(QSettings, _settings);
    return _settings->applicationName();
}

QSettings::Status VariableSettings::status(void) const
{
    BOBJPOINTER(QSettings, _settings);
    return _settings->status();
}


QString VariableSettings::settingName(void) const
{
    QString result;
    if ( ! _settings) return result;
    BOBJPOINTER(QSettings, _settings);
    if (_settings->organizationName().isEmpty())
        result = "@" + _settings->fileName();
    else
        result = QString("%1%2/%3")
                .arg(QSettings::UserScope == _settings->scope() ? "%" : "$")
                .arg(_settings->organizationName())
                .arg(_settings->applicationName());
    return result;
}

QVariant VariableSettings::read(const QString & key,
                                    const QVariant & defaultValue)
{
    BOBJPOINTER(QSettings, _settings);
    return _settings->value(key, defaultValue);
}

void VariableSettings::write(const QString & key,
                                 const QVariant & newValue)
{
    BOBJPOINTER(QSettings, _settings);
    _settings->setValue(key, newValue);
}

void VariableSettings::read(void)
{
    BOBJPOINTER(QSettings, _settings);
    foreach(QString key, _settings->allKeys())
    {
        set(VariableId(key), _settings->value(key));
    }
}

void VariableSettings::write(void) const
{
    BOBJPOINTER(QSettings, _settings);
    foreach (VariableId vid, VariableSet::ids())
    {
        _settings->setValue(vid, VariableSet::value(vid));
    }
}

void VariableSettings::remove(const QString & key) const
{
    BOBJPOINTER(QSettings, _settings);
    _settings->remove(key);
}

void VariableSettings::removeAll(void) const
{
    BOBJPOINTER(QSettings, _settings);
    foreach(QString key, _settings->allKeys())
    {
        _settings->remove(key);
    }
}

void VariableSettings::update(void)
{

}
