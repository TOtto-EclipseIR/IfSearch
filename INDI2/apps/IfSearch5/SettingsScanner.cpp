/** @file SettingsScanner.cpp
*	Definition of internal ImageCacheWriter class for DDT image library
*
*	@author	Dynamic DeZigns Technology
*
*/


#include "Settings.h"

SettingsScanner::SettingsScanner(Settings * parent)
    : settings(parent), done(false), QThread((QObject *)parent)
{
}

SettingsScanner::~SettingsScanner()
{
}

void SettingsScanner::run(void)
{
    int msSleep = 10000;

    for (;;)
    {
        if (done || 0 == msSleep)
        {
            if ( ! settings->keyForPollCount.isEmpty())
                settings->setValue(settings->keyForPollCount, "-1");
            return;
        }

        msleep(msSleep);

        if (done || 0 == msSleep)
        {
            if ( ! settings->keyForPollCount.isEmpty())
                settings->setValue(settings->keyForPollCount, "-1");
            return;
        }
        settings->scanForUpdate();
        if (0 == settings->updateMsec())
        {
            msSleep = 10000;
            continue;
        }
        settings->scan();

        msSleep = settings->updateMsec() ? settings->updateMsec() : 10000;
    }
} // run()


void Settings::scanForUpdate(void)
{
    if (keyForUpdateMsec.isEmpty())
        return;

    QVariant registry = value(keyForUpdateMsec);
    if (registry.toInt() != UpdateMsec)
        emit changeProperty(keyForUpdateMsec, registry);
} // scanForUpdate() slot

void Settings::scan(void)
{
    emit scanStart();


    foreach(Setting * var, vars)
    {
        if ( ! var->flags.testFlag(Settings::ReadOnly)
                && var->flags.testFlag(Settings::Dirty))
        {
            if (WriteBack)
            {
                QVariant current = *var;
                QVariant registry = value(var->key, current.toString());
                if (registry != current)
                    setValue(var->key, current.toString());
            }
            var->flags &= ~ Settings::Dirty;
        }

        if (var->flags.testFlag(Settings::Volatile)
                && ! var->flags.testFlag(Settings::Dirty))
        {
            QVariant current = *var;
            QVariant registry = value(var->key, current.toString());
            if (registry != current)
                var->setValue(registry);
        }
    } // foreach(var)

    foreach(SettingProperty * prop, properties)
    {
        if ( ! (prop->flags & Settings::ReadOnly) && (prop->flags & Settings::Dirty))
        {
            if (WriteBack)
            {
                QVariant current = prop->object->property(qPrintable(prop->propertyName));
                QVariant registry = value(prop->key, current.toString());
                if (registry != current)
                    setValue(prop->key, current.toString());
            }
            prop->flags &= ~ Settings::Dirty;
        }

        QVariant current = prop->object->property(qPrintable(prop->propertyName));
        if (prop->value != current)
        {
            prop->flags |= Settings::Dirty | Settings::Changed;
            prop->value = current;
            emitPropertyChanged(prop->key);
        }
        if ((prop->flags & Settings::Volatile) && ! (prop->flags & Settings::Dirty))
        {
            QVariant current = prop->object->property(qPrintable(prop->propertyName));
            QVariant registry = value(prop->key, current.toString());
            if (registry != current)
                emit changeProperty(prop->key, registry);
        }
    } // foreach(var)

    emit scanFinish();
} // scan() signal

void Settings::updatePollCount(void)
{
    this->setValue(keyForPollCount, QString::number(PollCount));
    ++PollCount;
}
