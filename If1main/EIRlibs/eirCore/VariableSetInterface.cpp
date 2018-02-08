#include "VariableSetInterface.h"


#include <QtDebug>

#include "../eirBase/BasicLog.h"
#include "VariableIdList.h"

VariableSetInterface::VariableSetInterface(QObject * parent)
    : QObject(parent)
    , VariableSet("VariableSetInterface")
    , _settings(new QSettings(parent))
{
    BLOG(Severity::Enter, "VSI c'tor()");
    setObjectName("VariableSetInterface");
}

VariableSetInterface::VariableSetInterface(QSettings * settings,
                              const QString & group,
                              QObject * parent)
    : QObject(parent)
    , VariableSet(group.isEmpty() ? "VariableSetInterface" : group)
    , _settings(0)
{
    BLOG(Severity::Enter, "VSI c'tor(%s)", qPrintable(group));
    if (settings->fileName().isEmpty())
        _settings = new QSettings(settings->fileName(),
                                  settings->format(),
                                  parent);
    else
        _settings = new QSettings(settings->format(),
                                  settings->scope(),
                                  settings->organizationName(),
                                  settings->applicationName(),
                                  parent);
    OBJPOINTER(QSettings, _settings);

    _settings->beginGroup(group);
    BLOG(Severity::Detail, "VSI c'tor(%s) at group=%s",
         qPrintable(group),
         qPrintable(_settings->group()));
}

QString VariableSetInterface::orgName(void) const
{
    OBJPOINTER(QSettings, _settings);
    return _settings->organizationName();
}

QString VariableSetInterface::appName(void) const
{
    OBJPOINTER(QSettings, _settings);
    return _settings->applicationName();
}

QVariant VariableSetInterface::read(const QString & key,
                                    const QVariant & defaultValue)
{
    OBJPOINTER(QSettings, _settings);
    return _settings->value(key, defaultValue);
}

void VariableSetInterface::write(const QString & key,
                                 const QVariant & newValue)
{
    OBJPOINTER(QSettings, _settings);
    _settings->setValue(key, newValue);
}

void VariableSetInterface::read(void)
{
    OBJPOINTER(QSettings, _settings);
    BLOG(Severity::Detail, "VSI::read() at group=%1",
         qPrintable(_settings->group()));
    foreach(QString key, _settings->allKeys())
    {
        BLOG(Severity::Data, "VSI::read() key=%s value=%s",
             qPrintable(key),
             qPrintable(_settings->value(key).toString()));
        set(VariableId(key), _settings->value(key));
    }
}

void VariableSetInterface::write(void) const
{
    OBJPOINTER(QSettings, _settings);
    foreach (VariableId vid, VariableSet::ids())
        _settings->setValue(vid, VariableSet::value(vid));
}

void VariableSetInterface::remove(const QString & key) const
{
    OBJPOINTER(QSettings, _settings);
    _settings->remove(key);
}

void VariableSetInterface::removeAll(void) const
{
    OBJPOINTER(QSettings, _settings);
    foreach(QString key, _settings->allKeys())
        _settings->remove(key);
}

void VariableSetInterface::update(void)
{

}
