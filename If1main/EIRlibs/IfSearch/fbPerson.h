/** @file fbPerson.h
*
*	Public declarations for INDI project fbPerson
*/

#pragma once
#include <qglobal.h>
#ifdef INDIFB_LIB
# define INDIFB_EXPORT Q_DECL_EXPORT
#else
# define INDIFB_EXPORT Q_DECL_IMPORT
#endif

#include <fbValue.h>

#include <QPair>
#include <QString>

#include <DualMap.h>
#include <Return.h>

namespace INDI
{
	class INDIFB_EXPORT fbPerson
	{
	public:
                fbPerson(void) : Key(0), PrimaryFaceKey(0), Active(true) {}
		~fbPerson() {}
		int key(void) const { return Key; }
		QString id(void) const { return Id; }
		QString description(void) const { return Description; }
		int primaryFaceKey(void) const { return PrimaryFaceKey; }
		void setKey(int v) { Key = v; }
		void setId(const QString s) { Id = s; }
		void setDescription(const QString s) { Description = s; }
		bool isNull(void) const { return ! Key; }
		void remove(void) { Active = false; }
                bool isActive(void) const { return Active; }

	public:
		DDT::Return readXmlFile(const QString & fileName);
		DDT::Return writeXmlFile(const QString & fileName);

	protected:
		int Key;
		QString Id;
		QString Description;
		int PrimaryFaceKey;
		bool Active;
		fbValueSet <fbCategory> categorySet;
	}; // fbPerson
} // INDI
