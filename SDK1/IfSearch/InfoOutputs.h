/** @file InfoOutputBase.h
*	Internal declaration of InfoOutputBase class for DDT core library
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

#include <InfoSeverity.h>
#include <NamedArray.h>

class DDTCORE_EXPORT InfoOutputs : public NamedArray<InfoSeverity, QList<InfoOutputBase *> >
{
public:
    QList<InfoOutputBase *> outputs(InfoSeverity sev);
    QSet<InfoOutputBase *> unique(void);
    void clear(void);
}; // class InfoOutputs
