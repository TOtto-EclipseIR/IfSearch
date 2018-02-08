#include "EigenFaceSearcher.h"

#include <QtCore/QList>

#include "InfoMacros.h"
#include "EigenFaceSearchResultList.h"
#include "EigenFaceSearchPerson.h"
#include "EigenFaceSearchSettings.h"
#include "EigenFaceTemplate.h"
#include "EigenFaceVector.h"
#include "Return.h"


EigenFaceSearcher::EigenFaceSearcher(void)
    : bestConfidence_i(0)
    , leastDistance_r(999.9)
{
}

void EigenFaceSearcher::clear(void)
{
    faceKey_vector_mhash.clear();
}

bool EigenFaceSearcher::contains(const int faceKey) const
{
    return faceKey_vector_mhash.contains(faceKey);
}

int EigenFaceSearcher::size(void) const
{
    return faceKey_vector_mhash.size();
}

QString EigenFaceSearcher::deleteFace(const int faceKey)
{
    if ( ! faceKey_vector_mhash.contains(faceKey))
        return QString("%1 faceKey not enrolled").arg(faceKey);
    faceKey_vector_mhash.remove(faceKey);
    return QString();
}

QString EigenFaceSearcher::enroll(const INDI::EigenFaceTemplate & tpl,
                                      const int faceKey,
                                      const int personKey)
{
    if (faceKey_vector_mhash.contains(faceKey))
        return QString("%1 faceKey already enrolled").arg(faceKey);
    foreach(INDI::EigenFaceVector vector, tpl.vectorMap().values())
    {
        vector.setKeys(faceKey, personKey);
        faceKey_vector_mhash.insert(faceKey, vector);
    }
    return QString();
}

int EigenFaceSearcher::bestConfidence(void) const
{
    return bestConfidence_i;
}

qreal EigenFaceSearcher::leastDistance(void) const
{
    return leastDistance_r;
}

/*! search()
  *
  * This function does its work by passing an EigenFaceSearchResultList
  * through several filters.
  *
  * distanceList is the computed distances for every search vector versus
  *     every (selected) enrolled vector. It contains one result per "person".
  *     It is limited by MaxDistance.
  * rankedList is sorted by distance and still contains one result per
  *     "person" but that single result has a rank assigned.
  * personList has each person's individual results combined into one entry.
  * combinedList has the distances of the individual results combined
  *     together for that person's distance and confidence score.
  *     It is limited by MinConfidence.
  * sortedList has been sorted by least distance and tiers assigned. It has been
  *     limited by maxResults.
  * tieredList has had search result tiers assigned to the sorted results.
  */
EigenFaceSearchResultList EigenFaceSearcher::
    search(const INDI::EigenFaceSearchSettings::Mode mode,
           const INDI::EigenFaceTemplate & tpl,
           INDI::EigenFaceSearchSettings * settings,
           const QList<int> & faceKeyList)
{
    int personMethod = settings->personMethod(mode, faceKey_vector_mhash.size());
    bool personMode = settings->getPersonMode();
    qreal maxDistance = settings->fMaxDistance();
    int maxFaces = settings->maxFaces(faceKey_vector_mhash.size());
    int minConfidence = settings->getMinConfidence();

    EigenFaceSearchResultList distanceList = computeDistance(faceKeyList, tpl, maxDistance);
    EigenFaceSearchResultList rankedList = rankResults(distanceList, maxFaces);
    EigenFaceSearchResultList personList = personMode
                                            ? collectPersons(rankedList)
                                            : rankedList;

    EigenFaceSearchResultList combinedList;
    if (personMode)
        switch (personMethod)
        {
        case 0:
        default:
        case 6: combinedList = combineBySearchKey(personList, settings);break;
        case 5: combinedList = combineByVector(personList, settings);   break;
        case 4: combinedList = combineAll(personList, settings);        break;
        case 3: combinedList = combineByRank(personList, settings);     break;
        case 2: combinedList = combineByEnroll(personList, settings);   break;
        case 1: combinedList = combineBest(personList, settings);       break;
        }
    else
        combinedList = personList;

    EigenFaceSearchResultList sortedList = sortResults(combinedList, settings);
    EigenFaceSearchResultList tieredList;
    switch (settings->getTierMethod())
    {
    default:
    case 0:
    case 1:
        tieredList = setFixedTiers(sortedList, settings);
        break;
    }
    tieredList.setBests();
    bestConfidence_i = tieredList.bestConfidence();
    leastDistance_r = tieredList.leastDistance();
    return tieredList;
}

EigenFaceSearchResultList EigenFaceSearcher::
    computeDistance(const QList<int> & faceKeyList,
                    const INDI::EigenFaceTemplate & tpl,
                    qreal maxDistance)
{
    QList<INDI::EigenFaceVector> searchVectors = tpl.vectorMap().values();
    QList<INDI::EigenFaceVector> enrollVectors;
    EigenFaceSearchResultList distanceList;

    int n = searchVectors.size();
    for (int x = 0, sk = -1; x < n; ++x, --sk)
        searchVectors[x].setFaceKey(sk);

    if (faceKeyList.isEmpty())
        enrollVectors = faceKey_vector_mhash.values();
    else
        foreach(int faceKey, faceKeyList)
            enrollVectors << faceKey_vector_mhash.values(faceKey);

    foreach(INDI::EigenFaceVector enrollVector, enrollVectors)
        foreach(INDI::EigenFaceVector searchVector, searchVectors)
        {
            qreal distance = enrollVector.distance(searchVector);
            if (distance <= maxDistance
                    && ! qIsNull(distance))
                distanceList
                        << EigenFaceSearchPerson(enrollVector.personKey(),
                                                 enrollVector.faceKey(),
                                                 searchVector.faceKey(),
                                                 enrollVector.vectorKey(),
                                                 searchVector.vectorKey(),
                                                 distance,
                                                 INDI::EigenFaceSearchSettings::
                                                     confidence(distance));
            DETAIL("PK=%1 FK=%2 distance=%3",enrollVector.personKey(),
                   enrollVector.faceKey(), distance);
        }

    DETAIL("distanceList %1 entries", distanceList.size());
    return distanceList;
}

EigenFaceSearchResultList EigenFaceSearcher::
    rankResults(const EigenFaceSearchResultList & distanceList,
                const int maxFaces)
{
    QMultiMap<qreal, EigenFaceSearchPerson> distanceMap;
    foreach(EigenFaceSearchPerson efsp, distanceList)
        distanceMap.insert(efsp.getDistance(), efsp);

    EigenFaceSearchResultList rankedList;
    int rank = 0;
    foreach(EigenFaceSearchPerson efsp, distanceMap.values())
    {
        efsp.setFirstRank(++rank);
        rankedList << efsp;
        DETAIL("PK=%1 rank=%2 distance=%3", efsp.getPersonKey(),
               efsp.getRank(), efsp.getDistance());
        if (efsp.size() >= maxFaces)
            break;
    }

    DETAIL("rankedList %1 entries", rankedList.size());
    return rankedList;
}

EigenFaceSearchResultList EigenFaceSearcher::
    collectPersons(const EigenFaceSearchResultList & rankedList)
{
    QMultiHash<int, EigenFaceSearchPerson> personHash;
    foreach(EigenFaceSearchPerson efsp, rankedList)
    {
        int personKey = efsp.getPersonKey();
        if (personKey && personHash.contains(personKey))
        {
            EigenFaceSearchPerson hashPerson = personHash.value(personKey);
            hashPerson.append(efsp);
            personHash.replace(personKey, hashPerson);
        }
        else
            personHash.insert(personKey, efsp);
    }

    EigenFaceSearchResultList personList;
    foreach(EigenFaceSearchPerson efsp, personHash.values())
    {
        DETAIL("adding PK=%1 distance=%2",
               efsp.getPersonKey(), efsp.getDistance());
        personList << efsp;
    }


    DETAIL("personList %1 entries", personList.size());
    return personList;
}

EigenFaceSearchResultList EigenFaceSearcher::
    combineBySearchKey(const EigenFaceSearchResultList & personList,
                       INDI::EigenFaceSearchSettings * settings)
{
    const int minConfidence = settings->getMinConfidence();
    const int maxPersonFaces = settings->getMaxPersonFaces();
    EigenFaceSearchResultList combinedList;

    foreach (EigenFaceSearchPerson efsp, personList)
    {
        QList<EigenFaceSearchResult> personResults = efsp.results();
        QSet<int> searchKeySet;
        efsp.clearResults();
        qreal distance = 1.0;
        qreal personRank = 0.0;
        foreach(EigenFaceSearchResult efsr, personResults)
        {
            int searchKey = efsr.getSearchKey();
            if ( ! searchKeySet.contains(searchKey))
            {
                searchKeySet.insert(searchKey);
                distance *= qMin(1.0, efsr.getDistance()
                                          * (1.0 + (0.5 * ++personRank)));
                efsp.append(efsr);
                DETAIL("Append FK=%1 SK=%4 distance=%2 net=%3",
                       efsr.getFaceKey(), efsr.getDistance(),
                       distance, efsr.getSearchKey());
            }
            if (efsp.size() >= maxPersonFaces)
                break;
        }
        int confidence = INDI::EigenFaceSearchSettings::confidence(distance);
        if (confidence >= minConfidence)
        {
            DETAIL("Insert PK=%1", efsp.getPersonKey());
            efsp.setDistance(distance);
            efsp.setConfidence(confidence);
            combinedList << efsp;
        }
    }
    return combinedList;
}

EigenFaceSearchResultList EigenFaceSearcher::
    combineBest(const EigenFaceSearchResultList & personList,
                INDI::EigenFaceSearchSettings * settings)
{
    int minConfidence = settings->getMinConfidence();
    return computePersonDistance(personList,
                                 minConfidence,
                                 1,
                                 0);
}

EigenFaceSearchResultList EigenFaceSearcher::
    combineByEnroll(const EigenFaceSearchResultList & personList,
                      INDI::EigenFaceSearchSettings * settings)
{
    int maxPersonFaces = settings->getMaxPersonFaces();
    int minConfidence = settings->getMinConfidence();
    return computePersonDistance(personList,
                                 minConfidence,
                                 maxPersonFaces,
                                 faceKey_vector_mhash.size());
}

EigenFaceSearchResultList EigenFaceSearcher::
    combineByVector(const EigenFaceSearchResultList & personList,
                      INDI::EigenFaceSearchSettings * settings)
{
    int maxPersonFaces = settings->getMaxPersonFaces();
    int minConfidence = settings->getMinConfidence();
    return computePersonDistance(personList,
                                 minConfidence,
                                 maxPersonFaces,
                                 personList.totalVectors());
}

EigenFaceSearchResultList EigenFaceSearcher::
    combineByRank(const EigenFaceSearchResultList & personList,
                  INDI::EigenFaceSearchSettings * settings)
{
    int maxPersonFaces = settings->getMaxPersonFaces();
    int minConfidence = settings->getMinConfidence();
    return computePersonDistance(personList,
                                 minConfidence,
                                 maxPersonFaces,
                                 0);
}

EigenFaceSearchResultList EigenFaceSearcher::
    combineAll(const EigenFaceSearchResultList & personList,
               INDI::EigenFaceSearchSettings * settings)
{
    int minConfidence = settings->getMinConfidence();
    return computePersonDistance(personList, minConfidence, 0, -1);
}

EigenFaceSearchResultList EigenFaceSearcher::
    computePersonDistance(const EigenFaceSearchResultList personList,
                          const int minConfidence,
                          const int maxPersonFaces,
                          const int topRank)
{
    EigenFaceSearchResultList combinedList;
    qreal fTopRank = topRank;
    DETAIL("minConfidence=%1, maxPersonFaces=%2, topRank=%3",
           minConfidence, maxPersonFaces, topRank);

    foreach (EigenFaceSearchPerson efsp, personList)
    {
        int nResults = efsp.size();
        if (maxPersonFaces && nResults > maxPersonFaces)
            nResults = maxPersonFaces;

        QList<EigenFaceSearchResult> efsrs = efsp.results();
        EigenFaceSearchResult bestResult = efsrs.takeFirst();
        qreal distance = bestResult.getDistance();
        DETAIL("PersonKey=%2, nResults=%1, first distance=%3",
               nResults, efsp.getPersonKey(), distance);

        QSet<int> usedFaceKeys;
        usedFaceKeys.insert(bestResult.getFaceKey());
        QList<EigenFaceSearchResult> usedResults;
        usedResults << bestResult;
        efsp.setResults(usedResults);
        qreal personRank = 1;

        foreach(EigenFaceSearchResult efsr, efsrs)
        {
            if (efsp.size() >= nResults)
                break;
            int resultFaceKey = efsr.getFaceKey();

            qreal resultRank = efsr.getRank();
            qreal resultDistance = efsr.getDistance();
            qreal c = 1.0;
            if (topRank < 0)
                ; // leave at one
            else if (topRank)
                c = 1.0 + resultRank / fTopRank;
            else // topRank is zero
                //c = 2.0 - (1.0 / (1.0 + resultRank));
                c = 1.0 + (0.05 * ++personRank);
            distance *= qMin(1.0, c * resultDistance);
            DETAIL("distance=%3, rank=%4, c=%2, updated=%1",
                   distance, c, resultDistance, resultRank);
            if (usedFaceKeys.contains(resultFaceKey))
                continue;

            efsp.append(efsr);
            usedFaceKeys.insert(resultFaceKey);
        }

        int confidence = INDI::EigenFaceSearchSettings::confidence(distance);
        DETAIL("confidence=%1 minConfidence=%2", confidence, minConfidence);
        if (confidence > minConfidence)
        {
            efsp.setBestDistance(bestResult.getDistance());
            efsp.setBestFaceKey(bestResult.getFaceKey());
            efsp.setConfidence(confidence);
            efsp.setDistance(distance);
            combinedList << efsp;
        }
    }

    DETAIL("combinedList %1 entries", combinedList.size());
    return combinedList;
}

EigenFaceSearchResultList EigenFaceSearcher::
    sortResults(const EigenFaceSearchResultList & combinedList,
                INDI::EigenFaceSearchSettings * settings)
{
    int maxResults = settings->maxResults(faceKey_vector_mhash.size());

    QMultiMap<qreal, EigenFaceSearchPerson> distanceMap;
    foreach(EigenFaceSearchPerson efsp, combinedList)
        distanceMap.insert(efsp.getDistance(), efsp);

    int rank = 0;
    EigenFaceSearchResultList sortedResults;
    foreach(EigenFaceSearchPerson efsp, distanceMap.values())
    {
        efsp.setRank(++rank);
        sortedResults << efsp;
        if (maxResults && rank >= maxResults)
            break;
    }

    DETAIL("sortedResults %1 entries", sortedResults.size());
    return sortedResults;
}

EigenFaceSearchResultList EigenFaceSearcher::
    setFixedTiers(const EigenFaceSearchResultList & sortedList,
                  INDI::EigenFaceSearchSettings * settings)
{
    EigenFaceSearchResultList tieredList;

    foreach(EigenFaceSearchPerson efsp, sortedList)
    {
        int confidence = efsp.getConfidence();
        EigenFaceSearchTier tier = settings->tierForConfidence(confidence);
        efsp.setTier(tier);
        if (1 == efsp.getRank() && EigenFaceSearchTier::Strong == tier)
        {
            if (1 == sortedList.size())
                efsp.setTier(EigenFaceSearchTier::Best);
            else
            {
                int secondConfidence = sortedList.at(1).getConfidence();
                EigenFaceSearchTier secondTier
                        = settings->tierForConfidence(secondConfidence);
                if (secondTier < EigenFaceSearchTier::Strong)
                    efsp.setTier(EigenFaceSearchTier::Best);
            }
        }
        tieredList << efsp;
    }

    DETAIL("tieredList %1 entries", tieredList.size());
    return tieredList;
}
