#ifndef RESOLVER_H
#define RESOLVER_H

#include <QObject>
#include <QList>
#include "IdString.h"

class Resolver : public QObject
{
    Q_OBJECT

private:
    struct Item
    {
        IdString    Name;
        int         Weight;
        int         Score;
    };

public:
    explicit Resolver(QObject *parent = 0);
    void clearAll(void);
    void clearScores(void);
    bool add(const IdString & name, const int weight=0, const int score=-1);
    bool setWeight(const IdString & name, const int weight);
    bool ignore(const IdString & name);
    bool isFinished(void) const;
    int confidence(void) const;
    QStringList report(void) const;

private:
    int find(const IdString & name) const;

signals:
    void resolved(int confidence);

public slots:
    bool setScore(const IdString & name, const int score);

private:
    QList<Item> List;
};

#endif // RESOLVER_H
