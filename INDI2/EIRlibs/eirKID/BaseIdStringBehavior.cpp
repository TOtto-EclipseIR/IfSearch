#include "BaseIdStringBehavior.h"

BaseIdStringBehavior::BaseIdStringBehavior(void)
    : StringBehavior(intList() << QMetaType::type("BaseIdString"))
{
}
