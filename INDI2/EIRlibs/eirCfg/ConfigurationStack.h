#ifndef CONFIGURATIONSTACK_H
#define CONFIGURATIONSTACK_H

#include <QObject>
#include <QStack>
#include "Configuration.h"

class EIRCFGSHARED_EXPORT ConfigurationStack
        : public QObject, QStack<Configuration>
{
    Q_OBJECT
public:
    explicit ConfigurationStack(QObject * parent=0);

signals:

public slots:

};

#endif // CONFIGURATIONSTACK_H
