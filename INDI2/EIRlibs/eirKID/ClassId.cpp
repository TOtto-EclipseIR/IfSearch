#include "ClassId.h"

#include "BaseIdBehavior.h"

class EIRKIDSHARED_EXPORT ClassIdBehavior : public BaseIdBehavior
{
public:
    static ClassIdBehavior * instance(void)
    {
        if ( ! singleton)
            singleton = new ClassIdBehavior;
        return singleton;
    }

private:
    ClassIdBehavior(void)
        : BaseIdBehavior(QString(), lower + upper + number)
    {
    }

    static ClassIdBehavior * singleton;
};

ClassIdBehavior * ClassIdBehavior::singleton=0;

ClassId::ClassId(const QString & inString)
    : BaseIdString(ClassIdBehavior::instance(), inString)
{
}

ClassId::ClassId(const char * chars)
    : BaseIdString(ClassIdBehavior::instance(), QString(chars))
{
}

ClassId::ClassId(const char * chars,
                       const char * chars1,
                       const char * chars2,
                       const char * chars3)
    : BaseIdString(ClassIdBehavior::instance())
{
    set(QString(chars));
    append(QString(chars1));
    append(QString(chars2));
    append(QString(chars3));
}

int ClassId::metaType(void)
{
    return QMetaType::type("ClassId");
}

const char * ClassId::metaTypeName(void)
{
    return QMetaType::typeName(metaType());
}
