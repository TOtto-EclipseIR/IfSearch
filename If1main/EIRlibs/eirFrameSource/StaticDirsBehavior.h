#ifndef STATICDIRSBEHAVIOR_H
#define STATICDIRSBEHAVIOR_H
#include "eirFrameSource.h"

#include "FrameSourceBehavior.h"

#include <QtCore/QFileInfo>
#include <QtCore/QMultiMap>

#include "../eirFile/QQDir.h"

class FrameSourcePlugin;

class EIRFRAMESOURCESHARED_EXPORT StaticDirsBehavior
        : public FrameSourceBehavior
{
public:
    StaticDirsBehavior(void);
    FrameSourceBehavior * brother(void);
    QString name() const;
    QStringList schemeList(void);
    bool supports(const QUrl & url);
    bool supportsScheme(const QString & scheme);
    bool canGrab(void);

private:
    void processDir(const QString & dirname,
                    const int depth=-1);
    QString sort(const QFileInfo fi) const;

public slots:
    void enterConstruct(void);
    void enterConnecting(void);
    void enterConfiguring(void);
    void enterStarted(void);
    void enterGrabbing(void);

private:
    QQDir base_dir;
    QMultiMap<QString, QFileInfo> sort_fi_mmap;
};

#endif // STATICDIRSBEHAVIOR_H
