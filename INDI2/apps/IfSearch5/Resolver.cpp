#include "Resolver.h"

#include <QStringList>

Resolver::Resolver(QObject *parent) :
    QObject(parent)
{
    clearAll();
}

void Resolver::clearAll(void)
{
    List.clear();
} // clearAll()

void Resolver::clearScores(void)
{
    int n = List.size();
    for (int x = 0; x < n; ++x)
        List[x].Score = -1;
} // clearScores()

bool Resolver::add(const IdString & name, const int weight, const int score)
{
    Item item;

    if (find(name) >= 0)
        return false;

    item.Name = name, item.Weight = weight, item.Score = score;
    List.append(item);
    return true;
} // add()

bool Resolver::setWeight(const IdString & name, const int weight)
{
    int index = find(name);
    if (index < 0)
        return false;
    List[index].Weight = weight;
    return true;
} // setWeight()

bool Resolver::ignore(const IdString & name)
{
    int index = find(name);
    if (index < 0)
        return false;
    List[index].Weight = 0;
    return true;
} // ignore()

bool Resolver::isFinished(void) const
{
    int n = List.size();
    for (int x = 0; x < n; ++x)
        if (List[x].Score < 0 && List[x].Weight != 0)
            return false;
    return true;
} // isFinished()

int Resolver::confidence(void) const
{
    int totalScore = 0, totalWeight = 0;
    int n = List.size();
    for (int x = 0; x < n; ++x)
    {
        int weight = List[x].Weight;
        int score = List[x].Score;
        if (0 != weight && score > 0)
        {
            totalScore += weight * score;
            totalWeight += qAbs(weight);
        }
    }
    return (totalWeight) ? qBound(1, qRound((qreal)totalScore
                                            / (qreal)totalWeight), 999) : 0;
} // confidence()

int Resolver::find(const IdString & name) const
{
    int n = List.size();
    for (int x = 0; x < n; ++x)
        if (List[x].Name == name)
            return x;
    return -1;
} // find()

bool Resolver::setScore(const IdString & name, const int score)
{
    int index = find(name);
    if (index < 0)
        return false;
    List[index].Score = qBound(1, score, 999);
    return true;
} // setScore()

QStringList Resolver::report(void) const
{
    QStringList result;
    int n = List.size();
    for (int x = 0; x < n; ++x)
    {
        if (0 == List[x].Weight)
            result << List[x].Name + ": Ignored, Weight=0";
        else if (List[x].Score < 0)
            result << List[x].Name + ": Ignored, No Score";
        else if (List[x].Score == 0)
            result << List[x].Name + ": Ignored, Not tested";
        else
            result << QString("%1: Score=%2 * Weight=%3")
                    .arg(List[x].Name)
                    .arg(List[x].Score)
                    .arg(List[x].Weight);
    }
    return result;
} // report()
