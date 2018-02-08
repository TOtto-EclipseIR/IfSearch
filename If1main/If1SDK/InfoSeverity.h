/** @file InfoSeverity.h
*	Declaration of InfoSeverity class for DDT core library
*
*	@author	Dynamic DeZigns Technology
*/
#pragma once
#include <qglobal.h>
#ifdef DDTCORE_LIB
# define DDTCORE_EXPORT Q_DECL_EXPORT
#else
# define DDTCORE_EXPORT Q_DECL_IMPORT
#endif

#include <NamedEnum.h>

namespace DDT
{
	class DDTCORE_EXPORT InfoSeverity : public DDT::NamedEnum<InfoSeverity>
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
} // namespace DDT

