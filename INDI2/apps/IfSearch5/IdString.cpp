#include "IdString.h"

IdString::IdString(void)
{
    clear();
}

IdString::IdString(const char * s)
{
    set(QString(s));
}

IdString::IdString(const QString & id)
{
    set(id);
}

void IdString::set(const QString & id)
{
    QString t = id.simplified();
    t.remove(QChar('<'));
    t.remove(QChar('>'));
    t.remove(QChar('&'));
    t.remove(QChar('"'));
    clear();
    append(t);
}

