/** @file InfoFunction.h
*	Declaration of InfoFunction class for DDT core library
*
*	@author	Dynamic DeZigns Technology
*/
#pragma once
#include <qglobal.h>

#include <Info.h>

#include <QObject>
#include <QVariant>

class InfoFunction
{
public:
    InfoFunction(const QString & FnName, const QString & File, const int Line,
                 const QString & Msg=QString(), QVariant V1=QVariant(),
                 QVariant V2=QVariant(), QVariant V3=QVariant(), QVariant V4=QVariant());
    ~InfoFunction();
    QString fnName;
    QString file;
    int line;
    QVariant returnVar;
}; // InfoFunction
