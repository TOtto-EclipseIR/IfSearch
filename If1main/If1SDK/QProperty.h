/*! \file QProperty.h Declarations for QProperty mechanism
*/

#ifndef QPROPERTY_H
#define QPROPERTY_H
#include <QVariant>

#define QPROPERTY_DECLARE(TYPE, NAME, DFLT) \
    public: \
        void init##NAME(void)      { setProperty(#NAME, QVariant(DFLT)); } \
        void set##NAME(TYPE v)     { setProperty(#NAME, QVariant(v)); } \
        TYPE get##NAME(void) const { return property(#NAME).value<TYPE>(); } \

#define QPROPERTY_DECLARE_RO(TYPE, NAME, DFLT) \
    private: \
        void init##NAME(void)      { setProperty(#NAME, QVariant(DFLT)); } \
        void set##NAME(TYPE v)     { setProperty(#NAME, QVariant(v)); } \
    public: \
        TYPE get##NAME(void) const { return property(#NAME).value<TYPE>(); } \

#define QPROPERTY_CTOR(TYPE, NAME, DFLT) \
        setProperty(#NAME, QVariant(DFLT)); \

#define DECLARE_QPROPERTIES(DEF) \
        DEF(QPROPERTY_DECLARE) \

#define DECLARE_QPROPERTIES_RO(DEF) \
        DEF(QPROPERTY_DECLARE_RO) \

#define DEFINE_QPROPERTIES_CTORS(DEF) \
        DEF(QPROPERTY_CTOR) \

#endif // QPROPERTY_H
