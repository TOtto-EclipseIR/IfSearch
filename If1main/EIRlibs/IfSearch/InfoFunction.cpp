/** @file InfoFunction.cpp
*	Definition of InfoFunction class for DDT core library
*
*	@author	Dynamic DeZigns Technology
*
*/

#include <Info.h>

using namespace DDT;

InfoFunction::InfoFunction(const QString & FnName, const QString & File, const int Line, 
					 const QString & Msg, QVariant V1, QVariant V2, QVariant V3, QVariant V4)
{
	Info::add(InfoItem(InfoSeverity::Enter, File, Line, 
			QObject::tr("Entering %1 %2").arg(FnName).arg(Msg), V1, V2, V3, V4));
	fnName = FnName;
	file = File;
	line = Line;
}
		
InfoFunction::~InfoFunction() 
{
	if (returnVar.isNull())
		Info::add(InfoItem(DDT::InfoSeverity::Leave, file, line, "\\Leaving %1 ", fnName));
	else
		Info::add(InfoItem(DDT::InfoSeverity::Leave, file, line, 
				"\\Leaving %1 returning %2", fnName, returnVar));
}
