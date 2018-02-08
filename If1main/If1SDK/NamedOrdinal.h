/** @file NamedOrdinal.h
*	Declaration of NamedOrdinal class for DDT core library
*
*	@author	Dynamic DeZigns Technology
*/

#pragma once
#include <qglobal.h>
#include <QStringList>

#include <DDTcore.h>

namespace DDT
{
#ifdef DDTCORE_LIB
	class Q_DECL_EXPORT NamedOrdinal
#else
	class NamedOrdinal
#endif
	{
	protected:
		NamedOrdinal(QStringList * list, const QString & Name) 
			: names(list)
			{
				iValue = names->indexOf(Name);
				if ( ! isValid())
				{
					names->append(Name);
					iValue = names->size();
				}
			}
		NamedOrdinal(int i, QStringList * list) 
			: iValue(i), names(list) {}

	public:
		NamedOrdinal(void) : iValue(-1), names(0) {}
		NamedOrdinal(const NamedOrdinal & other) 
			: iValue(other.iValue), names(other.names) {}
		static NamedOrdinal fromName(const QString & Name, QStringList * list)
			{ return NamedOrdinal(list->contains(Name) 
						? 1+list->indexOf(Name) : -1, list); }

	public:
		operator int() const		{ return iValue; };
		int toInt(void) const		{ return iValue; }
		bool isNull(void) const		{ return iValue == 0; }
		int size(void) const 		{ return names->size(); };
		bool isValid(void) const	
			{ return names && (iValue > 0) && (iValue <= names->size()); }
		QString name(void) const	
			{ return isValid() ? names->at(iValue-1) : QString(); }

	private:
		int iValue;
		QStringList * names;
	}; // class NamedOrdinal
//	bool operator== (const NamedOrdinal & lhs, const NamedOrdinal & rhs)
//		{ return lhs.toInt() == rhs.toInt(); }
} // DDT

#define DECLARE_NAMEDORDINAL_ID(NameType, Name) \
	const NameType NameType::Name(#Name);

#define DECLARE_NAMEDORDINAL_INSTANCE(NameType) \
const NameType NameType::Invalid; \
QStringList NameType::nameList;

#define DECLARE_NAMEDORDIAL_CTORS(NameType) \
	private: \
		NameType(const QString & name) : NamedOrdinal(&nameList, name) {}; \
		NameType(int i, QStringList * list) : NamedOrdinal(i, list) {}; \
		static QStringList nameList; \
	public: \
		NameType(void) : NamedOrdinal() {} \
		static NameType fromName(const QString & name) \
			{ return NameType(nameList.contains(name) \
						? 1+nameList.indexOf(name) : -1, &nameList); } \
		static const NameType Invalid; 
