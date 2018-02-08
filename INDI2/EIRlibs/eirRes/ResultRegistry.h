#ifndef RESULTREGISTRY_H
#define RESULTREGISTRY_H
#include "eirRes.h"

#include <QtCore/QMap>
#include <QtCore/QString>

#include "ResultInfo.h"
#include "ResultCode.h"
#include "ResultId.h"


class EIRRESSHARED_EXPORT ResultRegistry
{
public:
    static void registerCode(const ResultInfo &ri);
    static ResultCode code(const ResultId & id);
    static ResultCode code(const ResultInfo & rc);
    static ResultInfo info(const ResultCode rc);
    static ResultInfo info(const ResultId & id);
    static void dump(void);

private:
    static QMap<ResultCode, ResultInfo> code_info_map;
    static QMap<QString, ResultCode> sortable_code_map;

};

#endif // RESULTREGISTRY_H
