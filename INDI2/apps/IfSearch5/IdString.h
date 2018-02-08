#ifndef IDSTRING_H
#define IDSTRING_H

#include <QtCore/QString>

class IdString : public QString
{
public:
    IdString(void);
    IdString(const char * s);
    IdString(const QString & id);
//    bool isNull(void) const
  //  { return Id.isEmpty(); }
//    QString operator()(void) const
  //  { return Id; }
    void set(const QString & id);

private:
//    QString Id;
};

inline bool operator < (const IdString & left, const IdString & right)
{ return QString::compare(left, right, Qt::CaseInsensitive) < 0; }

inline bool operator == (const IdString & left, const IdString & right)
{ return QString::compare(left, right, Qt::CaseInsensitive) == 0; }

typedef QList<IdString> IdStringList;

#endif // IDSTRING_H
