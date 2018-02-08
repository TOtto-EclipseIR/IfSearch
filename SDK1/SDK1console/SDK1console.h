#ifndef SDK1CONSOLE_H
#define SDK1CONSOLE_H

#include <QtCore/QCoreApplication>

class SDK1console : public QCoreApplication
{
    Q_OBJECT
public:
    explicit SDK1console(int & argc, char ** argv);

signals:

public slots:

};

#endif // SDK1CONSOLE_H
