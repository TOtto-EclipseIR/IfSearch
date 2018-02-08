/** @file InfoOutputFile.cpp
*	Definition of InfoOutputFile class for DDT core library
*
*	@author	Dynamic DeZigns Technology
*
*/

#include <Info.h>

using namespace DDT;

InfoOutputFile::InfoOutputFile(QString fileName)	
	: stream(&file) 
{
	fileName.replace(QChar('@'), Info::timeBaseString());
	QFileInfo fi(fileName);
	if ( ! fi.dir().exists())
		fi.dir().mkpath(".");
	file.setFileName(fileName);
	file.open(QIODevice::WriteOnly); 
}

void InfoOutputFile::write(const InfoItem & item)
{ 
	QWriteLocker wLock(lock()); stream << item.toString() << "\r\n"; 
}

void InfoOutputFile::flush(void) 
{ 
	QWriteLocker wLock(lock()); stream.flush(); 
}

bool InfoOutputFile::isValid(void) const
{ 
	return stream.device() ? stream.device()->isWritable() : false; 
}
