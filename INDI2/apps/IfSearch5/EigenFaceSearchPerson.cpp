#include "EigenFaceSearchPerson.h"

#include "InfoMacros.h"

EigenFaceSearchPerson::EigenFaceSearchPerson(void)
{
    DEFINE_PROPERTIES_CTORS(SEARCHPERSON_PROPERTIES);
}

EigenFaceSearchPerson::
    EigenFaceSearchPerson(const int personKey,
                          const int faceKey,
                          const int searchKey,
                          const int enrollVector,
                          const int searchVector,
                          const qreal distance,
                          const int confidence)
{
    DEFINE_PROPERTIES_CTORS(SEARCHPERSON_PROPERTIES);
    EigenFaceSearchResult efsr(personKey,
                               faceKey,
                               searchKey,
                               enrollVector,
                               searchVector,
                               distance,
                               confidence);
    result_list.append(efsr);
    setPersonKey(personKey);
    setDistance(distance);
    setConfidence(confidence);
}

void EigenFaceSearchPerson::append(const EigenFaceSearchPerson & other)
{
    foreach(EigenFaceSearchResult efsr, other.result_list)
        result_list.append(efsr);
}

void EigenFaceSearchPerson::append(const EigenFaceSearchResult & result)
{
    result_list.append(result);
}

void EigenFaceSearchPerson::clearResults(void)
{
    result_list.clear();
}

int EigenFaceSearchPerson::size(void) const
{
    return result_list.size();
}

bool EigenFaceSearchPerson::contains(const int faceKey) const
{
    foreach(EigenFaceSearchResult efsr, result_list)
        if (faceKey == efsr.getFaceKey())
            return true;
    return false;
}

bool EigenFaceSearchPerson::isEmpty(void) const
{
    return result_list.isEmpty();
}

void EigenFaceSearchPerson::setFirstRank(const int rank)
{
    if ( ! result_list.isEmpty())
        result_list[0].setRank(rank);
    setRank(rank);
}

int EigenFaceSearchPerson::bestFaceKey(void) const
{
    return (result_list.isEmpty())
            ? -1
            : result_list[0].getFaceKey();
}

QList<EigenFaceSearchResult> EigenFaceSearchPerson::results(void) const
{
    return result_list;
}

void EigenFaceSearchPerson::setResults(const QList<EigenFaceSearchResult> & resultList)
{
    result_list = resultList;
}
