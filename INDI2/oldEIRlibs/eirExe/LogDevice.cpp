#include "LogDevice.h"

#include "../eirBase/BaseLog.h"
#include "../eirCore/VariableIdList.h"
#include "Log.h"
#include "Logger.h"
#include "LogItem.h"
#include "LogDeviceNull.h"
#include "LogDeviceText.h"
#include "LogFilter.h"
#include "LogOptions.h"

DEFINE_ENUMERATION(LogDeviceType, LOGDEVICETYPE_ENUM);


LogDevice::LogDevice(const VariableSet & config,
                     Logger * parent)
    : QObject(parent)
    , _logger(parent)
    , config_vars(config)
    , _type(LogDeviceType::Invalid)
{
}

LogDevice::~LogDevice()
{
}

LogDevice * LogDevice::create(const VariableSet & config,
                              Logger * parent)
{
    LogDevice * result = 0;

    QString ldt_string(config.value("Device").toString());
    LogDeviceType ldt(ldt_string);
    if ( ! ldt.isValid())
    {
        return result;
    }

    switch (ldt)
    {
    case LogDeviceType::Null:
        result = new LogDeviceNull(config, parent);
        break;

    case LogDeviceType::StdIo:
    case LogDeviceType::TxtFile:
        result = new LogDeviceText(config, parent);
        break;

    case LogDeviceType::Widget:
    case LogDeviceType::EblFile:
    case LogDeviceType::System:
    case LogDeviceType::Sql:
    case LogDeviceType::Settings:
        BTODO("LogDevice {%1} type not implemented", ldt.name());
        break;
    }

    if (result && ! result->isValid())
    {
        delete result;
        result = 0;
    }
    else
    {
        result->_type = ldt;
        result->option_flags = LogOptions::base().
                               parseFlags(config.value("Options")
                                          .toString());
        result->loadFilters(config.exportSection("Filter"));
    }

    return result;
}

void LogDevice::loadFilters(const VariableSet & filterConfig)
{
    foreach (VariableId name, filterConfig.sectionIds())
    {
        LogFilter lf(filterConfig.exportSection(name));
        name_filter_map.insert(name, lf);
    }
}

bool LogDevice::evaluate(const LogItem & li)
{
    QList<LogFilter> filters = name_filter_map.values();
    while ( ! filters.isEmpty())
    {
        LogFilter lf = filters.takeLast();
        int i = lf.evaluate(li);
        if (i < 0)
            return false;
        else if (i > 0)
            return true;
    }
    return false;
}

quint32 LogDevice::options(void) const
{
    return option_flags;
}

bool LogDevice::isValid(void) const
{
    return false;
}

bool LogDevice::isTroll(void) const
{
    return false;
}

bool LogDevice::alreadyExists(void)
{
    return false;
}

void LogDevice::start(void)
{

}

void LogDevice::finish(void)
{

}

void LogDevice::rollover(void)
{

}

void LogDevice::flush(void)
{

}

void LogDevice::write(LogItem li)
{
    if (evaluate(li))
        write(li.getSeverity(), li.toString(option_flags));
}

void LogDevice::remove(qint64 beforeEms)
{
    TODO("Use %1", beforeEms);
}

void LogDevice::thin(qint64 beforeEms, int sev)
{
    TODO("Use %1 & %2", beforeEms, sev);
}

