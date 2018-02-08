#include "ResultInfo.h"
#include <QSharedData>

#include <QtCore/QString>

#include "../eirBase/Severity.h"
//#include "../eirExe/Log.h"

#include "ResultId.h"
#include "ResultCode.h"
#include "ResultRegistry.h"

class ResultInfoData : public QSharedData
{
public:
    ResultCode _rc;
    Severity _sev;
    ResultId _id;
    QString format_s;
    QString description_s;
    QStringList valueNames_list;
};

ResultInfo::ResultInfo()
    : data(new ResultInfoData)
{
}

ResultInfo::ResultInfo(const ResultInfo &rhs)
    : data(rhs.data)
{
}

ResultInfo::ResultInfo(const ResultCode rc,
                       const Severity sev,
                       const ResultId & id,
                       const QString & fmt,
                       const QString & desc,
                       const QStringList & names)
    : data(new ResultInfoData)
{
    data->_rc = rc;
    data->_sev = sev;
    data->_id = id;
    data->format_s = fmt;
    data->description_s = desc;
    data->valueNames_list = names;
    ResultRegistry::registerCode(*this);
}

ResultInfo & ResultInfo::operator=(const ResultInfo &rhs)
{
    if (this != &rhs)   data.operator=(rhs.data);
    return *this;
}

ResultInfo::~ResultInfo()
{
}

ResultCode ResultInfo::code(void) const
{
    return data ? data->_rc : ResultCode();
}

Severity ResultInfo::severity(void) const
{
    return data ? data->_sev : Severity();
}

ResultId ResultInfo::id(void) const
{
    return data ? data->_id : ResultId();
}

QString ResultInfo::format(void) const
{
    return data ? data->format_s : QString();
}

QString ResultInfo::description(void) const
{
    return data ? data->description_s : QString();
}

QStringList ResultInfo::valueNames(void) const
{
    return data ? data->valueNames_list : QStringList();
}

bool ResultInfo::isNull(void) const
{
    return id().isNull();

}
