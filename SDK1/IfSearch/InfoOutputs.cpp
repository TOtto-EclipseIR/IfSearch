/** @file InfoOutputs.cpp
*	Definition of InfoOutputs class for DDT core library
*
*	@author	Dynamic DeZigns Technology
*
*/

#include "InfoOutputs.h"


#include <Info.h>
		
QList<InfoOutputBase *> InfoOutputs::outputs(InfoSeverity sev)
{
	QReadLocker lock(Info::outLock());
	return at(sev);
}

QSet<InfoOutputBase *> InfoOutputs::unique(void)
{
	QSet<InfoOutputBase *> outs;
	QReadLocker lock(Info::outLock());
#if 1
    for (int x = InfoSeverity::min(); x <= InfoSeverity::max(); ++x)
        foreach (InfoOutputBase * pIOB, at(x))
            outs |= pIOB;
#else
	for (int x = InfoSeverity::min(); x <= InfoSeverity::max(); ++x)
		outs |= at(x).toSet();
#endif
	return outs;
}

void InfoOutputs::clear(void)
{
	QWriteLocker lock(Info::outLock());
	for (int x = InfoSeverity::min(); x <= InfoSeverity::max(); ++x)
		value(x).clear();
}
