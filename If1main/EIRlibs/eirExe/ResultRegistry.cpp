#include "ResultRegistry.h"

#include "Log.h"

QMap<QString, ResultCode> ResultRegistry::sortable_code_map;
QMap<ResultCode, ResultInfo> ResultRegistry::code_info_map;

void ResultRegistry::registerCode(const ResultInfo & ri)
{
    QString sortid(ri.id().sortable());
    if (sortable_code_map.contains(sortid)) return;

    ResultCode rc = ri.code();
    code_info_map.insert(rc, ri);
    sortable_code_map.insert(sortid, rc);
    TRACE("Added rc=%1 for id=%2", rc(), ri.id());
}

ResultCode ResultRegistry::code(const ResultInfo & rc)
{
    EXPECT(sortable_code_map.contains(rc.id().sortable()));
    return sortable_code_map.value(rc.id().sortable());
}

ResultCode ResultRegistry::code(const ResultId & id)
{
    EXPECT(sortable_code_map.contains(id.sortable()));
    return sortable_code_map.value(id.sortable());
}

ResultInfo ResultRegistry::info(const ResultCode rc)
{
    if (EXPECTNOT(rc.isNull()))
        if ( ! EXPECT(code_info_map.contains(rc)))
            TRACE("No ResultInfo for ResultCode=%1", rc.toString());
    return code_info_map.value(rc);
}

ResultInfo ResultRegistry::info(const ResultId & id)
{
    EXPECT(sortable_code_map.contains(id.sortable()));
    ResultCode key = sortable_code_map.value(id.sortable(), ResultCode());
    return (key.isNull())
            ? code_info_map.value(key)
            : ResultInfo();
}

void ResultRegistry::dump(void)
{
    DUMP("===%1 ResultCodes registered", sortable_code_map.size());
    foreach (ResultCode rc, sortable_code_map.values())
    {
        ResultInfo ri = info(rc);
        QVariantList qvl;
        DUMP("---%1 [%2] %3", quint64(rc), rc.toString(), ri.id());
        if (ri.format().isEmpty())
        {
            foreach (QString s, ri.valueNames())
                DUMP("   Variable: %1", "{"+s+"}");
        }
        else
        {
            foreach (QString s, ri.valueNames()) qvl << "{"+s+"}";
            QString s = eirBase::formatMessage(ri.format(), qvl);
            DUMP("   Message Fmt: %1", s);
        }
        if ( ! ri.description().isEmpty())
            DUMP("   Description: %1", ri.description());
    }
}
