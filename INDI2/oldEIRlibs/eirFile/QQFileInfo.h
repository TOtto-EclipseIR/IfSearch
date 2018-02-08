#ifndef QQFILEINFO_H
#define QQFILEINFO_H

#include <QFileInfo>

#include <QStringList>

class QQFileInfo : public QFileInfo
{
public:
    QQFileInfo(void);

    bool isWildCompleteName(void) const;
    bool isWildSuffix(void) const;
    bool isWild(void) const;
    QStringList imageReadNameFilters(void) const;
    static QStringList supportedImageReadFormats(void);

private:
    static QStringList readFormats_sl;
};

#endif // QQFILEINFO_H
