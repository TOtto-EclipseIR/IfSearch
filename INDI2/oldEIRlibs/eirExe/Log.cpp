/** @file Log.cpp Definitions of Log-related dumpers that can't be defined below eirExe lib.
  *
  */
#define EIRCORE_LIBRARY
#include "../eirCore/eirCore.h"
#include "../eirCore/VariableSet.h"
#include "Log.h"

/** @fn dump()
  *
  * @todo Stringify QVariants
  */
void VariableSet::dump(void) const
{
    DUMP("VariableSet: name=%1, key=%2 0x%3, id=%4",
         data->name_s,
         data->key_u64,
         QString::number(data->key_u64, 16),
         data->id_s);
    foreach (QString key, data->vbl_map.keys())
    {
        Variable vbl(data->vbl_map.value(key));
        DUMP("---%1 = {%2}", vbl.id(), vbl.var().toString());
    }
    for (int x = 0; x < data->var_list.size(); ++x)
        DUMP("---%1. {%2}", x, data->var_list.at(x).toString());
    DUMP("---%1 bytes of Binary", data->_ba.size());
}
