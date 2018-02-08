/** @file Property.h Declarations for Property mechanism
*/

#ifndef PROPERTY_H
#define PROPERTY_H

#define PROPERTY_DECLARE(TYPE, NAME, DFLT) \
    private: \
        TYPE TYPE##_##NAME; \
    public: \
        void init##NAME(void)      { TYPE##_##NAME = DFLT; } \
        void set##NAME(TYPE v)     { TYPE##_##NAME = v;    } \
        TYPE get##NAME(void) const { return TYPE##_##NAME; } \

#define PROPERTY_CTOR(TYPE, NAME, DFLT) \
        TYPE##_##NAME = DFLT; \

#define DECLARE_PROPERTIES(DEF) \
        DEF(PROPERTY_DECLARE) \

#define DEFINE_PROPERTIES_CTORS(DEF) \
        DEF(PROPERTY_CTOR) \


#endif // PROPERTY_H
