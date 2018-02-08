#ifndef VARIABLEIDLIST_H
#define VARIABLEIDLIST_H
#include "eirCore.h"

#include <QtCore/QList>
#include "VariableId.h"


class EIRCORESHARED_EXPORT VariableIdList : public QList<VariableId>
{
public:
    VariableIdList(void);

    operator QStringList(void) const;

};

#endif // VARIABLEIDLIST_H
