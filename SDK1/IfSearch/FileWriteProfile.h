/** @file FileWriteProfile.h
*	Declaration of FileWriteProfile class for DDT core library
*
*	@author	Dynamic DeZigns Technology
*/

#pragma once
#include <qglobal.h>
#ifdef DDTIMG_LIB
# define DDTIMG_EXPORT Q_DECL_EXPORT
#else
# define DDTIMG_EXPORT Q_DECL_IMPORT
#endif

#include <FileWriter.h>

class QFile;
class QImage;
#include <QDir>
#include <QMap>
#include <QString>
#include <QByteArray>

#include <DDTcore.h>
class Setting;

class DDTIMG_EXPORT FileWriteProfile : public QObject
{
    Q_OBJECT

    friend class FileWriter;

private:
    FileWriteProfile(const QString & name,
                     FileWriter::Flags f,
                     FileWriter * parent);

public:
    ~FileWriteProfile(void);
    FileWriter * writer(void) const;
    QString name(void) const	{ return _name; }
    bool isActive(void) const	{ return ! _dirs.isEmpty(); }
    bool hasPending(void) const	{ return ! _filesPending.isEmpty(); }
    int sizePending(void) const { return _filesPending.size(); }
    QString format(void) const  { return _format; }
    QDir dir(void) const        { return _dirs.isEmpty() ? QDir() : _dirs.at(0); }
    QList<QDir> dirs(void) const {return _dirs; }
    QString keyName(void) const;
    Return write(const QString & imageId);
    Return write(const QImage & image, const QString & baseName);
    Return write(QImage image, const QString & baseName,
                 const QDomDocument & doc);
    Return write(const QByteArray & ba, const QString & baseName);
    Return write(const QFile & file, QString baseName=QString());
    Return write(const QDomDocument & doc, const QString & baseName);
    QString filePath(const QString & baseName);
    void dump(void) const;

private:
    QString _name;
    FileWriter::Flags _flags;
    Setting * _opt;
    QList<QDir> _dirs;
    int _quality;
    QString _format;
    int _maxCache;
    QStringList _filesPending;
}; // class FileWriteProfile
