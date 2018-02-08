#ifndef FRAMESOURCEENUMERATIONCACHE_H
#define FRAMESOURCEENUMERATIONCACHE_H
#include "eirFrameSource.h"

#include <QtCore/QObject>

#include <QtCore/QMap>

#include "FrameSourceEnumeration.h"

class QUrl;

class VariableSetInterface;
class ResultSet;
class FrameSourceChannelInfo;

class EIRFRAMESOURCESHARED_EXPORT FrameSourceEnumerationCache
        : public QObject
{
    Q_OBJECT

public:
    FrameSourceEnumerationCache(QObject * parent=0);
    void invalidate(void);
    void invalidate(const QUrl & url);
    void set(const QUrl & url,
             const FrameSourceEnumeration & fse);
    void set(const QUrl & url,
             const int channel,
             const FrameSourceChannelInfo & fsci);
    FrameSourceEnumeration get(const QUrl & url) const;
    FrameSourceChannelInfo get(const QUrl & url,
                               const int channel) const;
    int size(void) const;
    ResultSet read(VariableSetInterface * vsi);
    ResultSet write(VariableSetInterface * vsi);

private:
    QMap<QUrl, FrameSourceEnumeration> url_enum_map;
};

#endif // FRAMESOURCEENUMERATIONCACHE_H
