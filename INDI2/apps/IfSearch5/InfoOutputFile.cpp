/** @file InfoOutputFile.cpp
*	Definition of InfoOutputFile class for DDT core library
*
*	@author	Dynamic DeZigns Technology
*
*/

#include <Info.h>



InfoOutputFile::InfoOutputFile(QString fileName)	
	: stream(&file) 
{
	fileName.replace(QChar('@'), Info::timeBaseString());
	QFileInfo fi(fileName);
	if ( ! fi.dir().exists())
		fi.dir().mkpath(".");
	file.setFileName(fileName);
        file.open(QIODevice::WriteOnly | QIODevice::Text);
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

void InfoOutputFile::rollover(void)
{
    QString origFileString(file.fileName());
    QFileInfo fi(file);
    QString oldFileName(fi.completeBaseName());
    if (oldFileName.isEmpty()) // is stdio?
        return;

    QString newFileName(oldFileName);
    newFileName += QDateTime::currentDateTime().toString("@DyyyyMMdd-Thhmm");
    fi.setFile(fi.dir(), newFileName + "." + fi.completeSuffix());

    file.close();
    file.rename(fi.filePath());
    file.setFileName(origFileString);
    file.open(QIODevice::WriteOnly | QIODevice::Text);

    int keep = Info::rolloverKeep();
    if (keep)
    {
        QStringList qsl;
        qsl << oldFileName + "@*." + fi.completeSuffix();
        QStringList fileNames = fi.dir().entryList(qsl,
                                                   QDir::Files,
                                                   QDir::Time | QDir::Reversed);
        while (fileNames.size() > keep)
        {
            QString removeFileName = fileNames.takeFirst();
            fi.dir().remove(removeFileName);
        }
    }
}
