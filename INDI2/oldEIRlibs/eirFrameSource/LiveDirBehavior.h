#ifndef LIVEDIRBEHAVIOR_H
#define LIVEDIRBEHAVIOR_H
#include "eirFrameSource.h"

#include <QtCore/QStringList>

#include "../eirFile/QQDir.h"

#include "FrameSourceBehavior.h"

class FrameSourcePlugin;

class EIRFRAMESOURCESHARED_EXPORT LiveDirBehavior
        : public FrameSourceBehavior
{
public:
    LiveDirBehavior(void);
    FrameSourceBehavior * brother(void);
    QString name(void) const;
    QStringList schemeList(void);
    bool supports(const QUrl & url);
    bool supportsScheme(const QString & scheme);

    bool canGrab(void);
    void enterConnecting(void);

private:
    QQDir base_dir;
    QStringList filename_list;
};

#endif // LIVEDIRBEHAVIOR_H
