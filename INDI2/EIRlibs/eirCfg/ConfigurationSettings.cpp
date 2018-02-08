#include "ConfigurationSettings.h"

#include "Configuration.h"

ConfigurationSettings::ConfigurationSettings(const QString & name,
                                             Configuration * config)
    : QObject(config)
    , _config(config)
    , _settings(0)
{
    (void)name;
}
