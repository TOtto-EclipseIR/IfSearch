#ifndef QQICON_H
#define QQICON_H
#include "eirGUI.h"

#include <QtGui/QIcon>
//#include "../eirBase/StaticInitialize.h"

#include <QtCore/QList>
#include <QtCore/QSize>
class QDir;
class QString;

class EIRGUISHARED_EXPORT QQIcon : public QIcon //, StaticInitialize
{
public:
    QQIcon(void);
    QQIcon(const QString & name);

    static void addTheme(const QDir & dir);
    static void setSizes(const QList<QSize> & sizes);
    static void addSize(const QSize size);

private:
    //bool staticInitialize(void);
    static QIcon create(const QString & name);
    static QIcon fromSvg(const QString & fileName);
    static QIcon fromImageFile(const QString & fileName);

private:
    static QList<QSize> size_list;
};

#endif // QQICON_H
