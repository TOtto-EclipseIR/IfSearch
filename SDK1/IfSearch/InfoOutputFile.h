/** @file InfoOutputFile.h
*	Internal declaration of InfoOutputFile class for DDT core library
*
*	@author	Dynamic DeZigns Technology
*/
#pragma once
#include <qglobal.h>

#include <InfoOutputBase.h>

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>

class InfoOutputFile : public InfoOutputBase
{
public:
    InfoOutputFile(QString fileName);
    InfoOutputFile(FILE * fileHandle)
        : stream(fileHandle, QIODevice::WriteOnly) {}
    ~InfoOutputFile() {}

protected:
    virtual void write(const InfoItem & item);
    virtual void flush(void);
    virtual bool isValid(void) const;
    virtual void rollover(void);

private:
    QFile file;
    QTextStream stream;
}; // class InfoOutputFile
