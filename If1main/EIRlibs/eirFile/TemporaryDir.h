#ifndef TEMPORARYDIR_H
#define TEMPORARYDIR_H
#include "eirFile.h"

#include "QQDir.h"

class EIRFILESHARED_EXPORT TemporaryDir : public QQDir
{
public:
    TemporaryDir(QDir baseDir=QDir::temp(),
                 const QString & baseName=QString(),
                 const bool deleteAfter=true);
    ~TemporaryDir();
    bool isWarning(void) const;
    QString warningString(void) const;

private:
    bool deleteAfter_bool;
    QString warning_string;
};

#endif // TEMPORARYDIR_H
