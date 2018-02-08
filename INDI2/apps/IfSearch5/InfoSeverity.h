/** @file InfoSeverity.h
*	Declaration of InfoSeverity class for DDT core library
*
*	@author	Dynamic DeZigns Technology
*/
#pragma once
#include <qglobal.h>

#include <NamedEnum.h>

class  InfoSeverity : public NamedEnum<InfoSeverity>
{
    DECLARE_NAMEDENUM_CTORS(InfoSeverity)
    static const InfoSeverity Null;
    static const InfoSeverity Leave;
    static const InfoSeverity Enter;
    static const InfoSeverity Detail;
    static const InfoSeverity Debug;
    static const InfoSeverity Info;
    static const InfoSeverity Progress;
    static const InfoSeverity Warning;
    static const InfoSeverity Error;
    static const InfoSeverity Fatal;
    static const InfoSeverity Unknown;
}; // InfoSeverity
