#ifndef IMAGEDIRSINK_H
#define IMAGEDIRSINK_H
#include "eirSDK1.h"

#include <QObject>

#include <QtCore/QFileSystemWatcher>
#include <QtGui/QImage>

#include "../eirFile/QQDir.h"

class EIRSDK1SHARED_EXPORT ImageDirSink : public QObject
{
    Q_OBJECT
public:
    explicit ImageDirSink(const QString & dirName,
                          QObject *parent = 0);
    void setMoveAfter(QQDir moveAfter=QQDir::nullDir);

public:
    void clear(void);

signals:
    void gotImage(QImage image, QString id);

private slots:
    void checkDir(void);

private:
    QQDir _dir;
    QQDir after_dir;
    QFileSystemWatcher _watcher;
};

#endif // IMAGEDIRSINK_H
