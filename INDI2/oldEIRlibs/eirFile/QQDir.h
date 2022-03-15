#ifndef QQDIR_H
#define QQDIR_H
#include "eirFile.h"

#include <QtCore/QDir>
#include <QtCore/QString>

#include "../eirTypes/MillisecondTime.h"

class QQDir : public QDir
{
public:
    QQDir(void);
    QQDir(const QString & path,
          const MillisecondTime base=MillisecondTime::base());
    QQDir(const QDir & other);
    bool isNull(void) const;
    bool cd(const QString & dirName);
    bool mkdir(const QString & dirName);
    bool mkpath(const QString & dirName);
    bool rmdir(const QString & dirName);
    void setPath(const QString & dirName);
    bool moveFrom(const QString & filePath);
    bool removeAll(const Filters filters=Dirs|Files);
    QString checkChange(void);

public:
    static bool removeAll(QDir dir,
                          const Filters filters=Dirs|Files);

private:
    QString parse(const QString & path,
                  const MillisecondTime mst=MillisecondTime()) const;

public:
    static QQDir nullDir;

private:
    MillisecondTime base_mst;


};

#endif // QQDIR_H
