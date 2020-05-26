#pragma once

#include <QObject>
#include <eirExe/Console.h>

class IfConsole : public Console
{
    Q_OBJECT
public:
    explicit IfConsole(QObject *parent = nullptr);

signals:

};

