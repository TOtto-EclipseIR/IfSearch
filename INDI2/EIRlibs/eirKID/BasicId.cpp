#include "BasicId.h"

#include "BaseIdBehavior.h"

class EIRKIDSHARED_EXPORT BasicIdBehavior : public BaseIdBehavior
{
public:
    static BasicIdBehavior * instance(void)
    {
        if ( ! singleton)
            singleton = new BasicIdBehavior;
        return singleton;
    }

private:
    BasicIdBehavior(void)
        : BaseIdBehavior("/", "/" + lower + upper + number)
    {
    }

    static BasicIdBehavior * singleton;
};

BasicIdBehavior * BasicIdBehavior::singleton=0;

BasicId::BasicId(const QString & inString)
    : BaseIdString(BasicIdBehavior::instance(), inString)
{
}

BasicId::BasicId(const char * chars)
    : BaseIdString(BasicIdBehavior::instance(), QString(chars))
{
}

BasicId::BasicId(const char * chars,
                       const char * chars1,
                       const char * chars2,
                       const char * chars3)
    : BaseIdString(BasicIdBehavior::instance())
{
    set(QString(chars));
    append(QString(chars1));
    append(QString(chars2));
    append(QString(chars3));
}

int BasicId::metaType(void)
{
    return QMetaType::type("BasicId");
}

const char * BasicId::metaTypeName(void)
{
    return QMetaType::typeName(metaType());
}
