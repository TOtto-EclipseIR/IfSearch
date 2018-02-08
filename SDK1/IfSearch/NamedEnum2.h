/*! \file NamedEnum.h Declarations for NamedEnum mechanism

    NamedEnum provides a way to parse and display names
    for enum values.

    \section    Usage

    \subsection secHeader  In the header file

    Define the names of the enum values using an arbitrary
    preprocesor macro (in this example, \c NV ) collected
    into another arbitrary macro (in this example,
    \c DAYSOFWEEK_ENUM . Use the \c DECLARE_NAMEDENUM
    macro to define the desired name of the declared
    enum and the collection macro.

    For example:

    \code

    // DaysOfWeek.h

    #define DAYSOFWEEK_ENUM(NV)     \
        NV(Monday,=1)               \
        NV(Tuesday,)                \
        NV(Wednesday,)              \
        NV(Thursday,)               \
        NV(Friday,)                 \
        NV(Saturday,)               \
        NV(Sunday,)

    DECLARE_NAMEDENUM(DaysOfWeek, DAYSOFWEEK_ENUM)

    // eof  DaysOfWeek.h
    \endcode

    Note that the commas are required in the \c NV macros
    and that the value may be set with an equals and
    a valid enum value.

    This will create an enum sytacticly equivalent to

    \code
    enum DaysOfWeek
    {
        Monday = 1,
        Tuesday,
        Wednesday,
        Thursday,
        Friday,
        Saturday,
        Sunday
    };
    \endcode

    But, with the significant addition of two global functions
    to convert between enum values and QStrings.

    \code
    QString nameOf(const DaysOfWeek v);
    DaysOfWeek valueOfDaysOfWeek(const QString & s);
    \endcode

    \subsection secSource  In the source file

    You only need to call the \c DEFINE_NAMEDENUM
    macro with the enum name, an export macro,
    and the collection macro
    to define the two utility functions.

    For example:
    \code
    // DaysOfWeek.cpp
    #include "DaysOfWeek.h"
    DEFINE_NAMEDENUM(DaysOfWeek, IJMCORESHARED_EXPORT, DAYSOFWEEK_ENUM)
    // eof DaysOfWeek.cpp
    \endcode
*/

#ifndef NAMEDENUM_H
#define NAMEDENUM_H

#include <QtCore/QString>

/*! \def    NAMEDENUM_VALUE(name, value)
    \brief  Used internally in the NamedEnum mechanism
*/
#define NAMEDENUM_VALUE(name, value) \
    name value,

/*! \def    NAMEDENUM_CASE(name, value)
    \brief  Used internally in the NamedEnum mechanism
*/
#define NAMEDENUM_CASE(name, value) \
    case name: return #name;

/*! \def    NAMEDENUM_COMPARE(name, value)
    \brief  Used internally in the NamedEnum mechanism
*/
#define NAMEDENUM_COMPARE(name, value) \
    if (0 == s.compare(#name, Qt::CaseInsensitive)) return name;

/*! \def    DECLARE_NAMEDENUM(type, def)
    \brief  Used in header file to declare a NamedEnum
    \see    secHeader
*/
#define DECLARE_NAMEDENUM(type, exp, def) \
    enum type { def(NAMEDENUM_VALUE) }; \
    QString exp nameOf(const type v); \
    type exp valueOf ## type(const QString & s); \

/*! \def    DEFINE_NAMEDENUM(type, def)
    \brief  Used in source file to define functions for a NamedEnum
    \see    secSource
*/
#define DEFINE_NAMEDENUM(type, def) \
    QString nameOf(const type v) \
    { \
        switch (v) \
        { \
            def(NAMEDENUM_CASE) \
        default: return QString(); \
        } \
    } \
    \
    type valueOf ## type(const QString & s) \
    { \
        def(NAMEDENUM_COMPARE) \
        return (type)-1; \
    } \

#endif // NAMEDENUM_H
