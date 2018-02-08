/*! @file Property.h Declarations for Property mechanism

In the header file specify a type, a name, and a default value:
@code
#define VERSIONINFO_PROPERTIES(TND) \
        TND(QString,    String,        QString()) \
        TND(QString,    OrgName,       QString()) \
        ...

class EIRCORESHARED_EXPORT VersionInfo
{
    DECLARE_PROPERTIES(VERSIONINFO_PROPERTIES);
@endcode

In the source file ctor:
@code
VersionInfo::VersionInfo(void)
{
    DEFINE_PROPERTIES_CTORS(VERSIONINFO_PROPERTIES);
    setDateTime(__DATE__ " " __TIME__);
}
@endcode

This will generate the following three public functions:
    Type initName(void) // set default
    void setName(Type value) // set a new value
    Type getName(void) const // get the current value
And a private data member:
    Type Name_Type;
The class c'tor will set each to the default value.
*/

#ifndef PROPERTY_H
#define PROPERTY_H

#define PROPERTY_DECLARE(TYPE, NAME, DFLT) \
    private: \
        TYPE NAME##_##TYPE; \
    public: \
        void init##NAME(void)      { NAME##_##TYPE = DFLT; } \
        void set##NAME(TYPE v)     { NAME##_##TYPE = v;    } \
        TYPE get##NAME(void) const { return NAME##_##TYPE; } \

#define PROPERTY_CTOR(TYPE, NAME, DFLT) \
        NAME##_##TYPE = DFLT; \

#define DECLARE_PROPERTIES(DEF) \
        DEF(PROPERTY_DECLARE) \

#define DEFINE_PROPERTIES_CTORS(DEF) \
        DEF(PROPERTY_CTOR) \


#endif // PROPERTY_H
