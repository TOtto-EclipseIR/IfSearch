#include "EigenFaceSearchSettings.h"


#include <QtCore/QDynamicPropertyChangeEvent>
#include <QtCore/QEvent>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QMapIterator>
#include <QtCore/QVector>

#include "EigenFaceSearchTier.h"
#include "InfoMacros.h"

EigenFaceSearchSettings::EigenFaceSearchSettings(const Mode mode, QObject * parent)
    : QObject(parent)
{
    setObjectName("EigenFaceMatcherSettings");
    setDefaults(mode);
}

void EigenFaceSearchSettings::setMaxResults(qreal newMaxResults)
{
    MaxResults = newMaxResults;
}

void EigenFaceSearchSettings::setMaxFaces(qreal newMaxFaces)
{
    MaxFaces = newMaxFaces;
}

qreal EigenFaceSearchSettings::getMaxDistance() const
{
    return MaxDistance;
}

void EigenFaceSearchSettings::setMaxDistance(qreal newMaxDistance)
{
    MaxDistance = newMaxDistance;
}

qreal EigenFaceSearchSettings::getMinDistance() const
{
    return MinDistance;
}

void EigenFaceSearchSettings::setMinDistance(qreal newMinDistance)
{
    MinDistance = newMinDistance;
}

int EigenFaceSearchSettings::getStrongConfidence() const
{
    return StrongConfidence;
}

void EigenFaceSearchSettings::setStrongConfidence(int newStrongConfidence)
{
    StrongConfidence = newStrongConfidence;
}

int EigenFaceSearchSettings::getStrongPersonCount() const
{
    return StrongPersonCount;
}

void EigenFaceSearchSettings::setStrongPersonCount(int newStrongPersonCount)
{
    StrongPersonCount = newStrongPersonCount;
}

int EigenFaceSearchSettings::getPossibleConfidence() const
{
    return PossibleConfidence;
}

void EigenFaceSearchSettings::setPossibleConfidence(int newPossibleConfidence)
{
    PossibleConfidence = newPossibleConfidence;
}

int EigenFaceSearchSettings::getMinConfidence() const
{
    return MinConfidence;
}

void EigenFaceSearchSettings::setMinConfidence(int newMinConfidence)
{
    MinConfidence = newMinConfidence;
}

int EigenFaceSearchSettings::getTierMethod() const
{
    return TierMethod;
}

void EigenFaceSearchSettings::setTierMethod(int newTierMethod)
{
    TierMethod = newTierMethod;
}


void EigenFaceSearchSettings::setDefaults(const Mode mode)
{
    switch (mode)
    {
    case FormalSearch:
    case Verify:
    case Authenticate:
        setMinConfidence(500);
        setPossibleConfidence(750);
        setStrongConfidence(900);
        break;

    case CasualMatch:
        setMinConfidence(500);
        setPossibleConfidence(650);
        setStrongConfidence(850);
        break;
    }
}

#if 0
qreal EigenFaceSearchSettings::fDuplicate(void) const
{
    return getDuplicateThreshold()
            ? pow(0.1, getDuplicateThreshold())
            : 0.0;
}

bool EigenFaceSearchSettings::checkDuplicates(void) const
{
    return !! getDuplicateThreshold();
}
#endif

qreal EigenFaceSearchSettings::fMinDistance(void) const
{
    qreal f = getMinDistance();
    if (f > 1.0) f /= 1000.0;
    if (qIsNull(f)) f = 0.0000001;
    return f;
}

qreal EigenFaceSearchSettings::fMaxDistance(void) const
{
    qreal f = getMaxDistance();
    if (f > 100.0) f /= 1000.0;
    if (qIsNull(f)) f = 1.0;
    return f;
}

int EigenFaceSearchSettings::maxFaces(const int totalFaces) const
{
    qreal f = getPersonMode() ? getMaxFaces() : getMaxResults();
    int n = totalFaces;
    if (f > 1.0)
        n = (int)f;
    else if ( ! qIsNull(f))
        n = qRound(f * (qreal)totalFaces);
    DETAIL("maxFaces = %1", n);
    return n;
}

int EigenFaceSearchSettings::maxResults(int totalResults) const
{
    qreal f = getMaxResults();
    int n = totalResults;
    if (f >= 1.0)
        n = (int)f;
    else if ( ! qIsNull(f))
        n = qRound(f * (qreal)totalResults);
    else
        n = 0;
    DETAIL("maxResults = %1", n);
    return n;
}

int EigenFaceSearchSettings::personMethod(const Mode mode,
                                            const int totalFaces)
{
    int pm = getPersonMethod();
    if ( ! pm)
    {
        switch (mode)
        {
        case FormalSearch:
        case CasualMatch:
            pm = (totalFaces < 50) ? 3 : 2;
            break;

        case Authenticate:
        case Verify:
            pm = 4;

        default: // do nothing
            break;
        }
    }
    DETAIL("personMethod = %1", pm);
    return pm;
}

EigenFaceSearchTier EigenFaceSearchSettings::tierForConfidence(const int confidence)
{
    EigenFaceSearchTier result;

    if (confidence >= getStrongConfidence())
        result = EigenFaceSearchTier::Strong;
    else if (confidence >= getPossibleConfidence())
        result = EigenFaceSearchTier::Possible;
    else if (confidence >= getMinConfidence())
        result = EigenFaceSearchTier::Weak;
    else
        result = EigenFaceSearchTier::NoMatch;

    return result;
}

QList<EigenFaceSearchTier>
EigenFaceSearchSettings::tiersForConfidence(
        const QList<int> & confidenceList,
        const bool needSort) const
{
    const int n = confidenceList.size();
    QVector<EigenFaceSearchTier> result(n);

    /* Sort Descending */
    QList<int> sortedList;
    if (needSort)
    {
        int ix = 0;
        QMultiMap<int, int> sortMap;
        for (int conf : confidenceList)
            sortMap.insert(conf, ix++);
        sortedList = sortMap.keys();
    }
    else
    {
        sortedList = confidenceList;
    }

    /* Iterate checking confidences */
    EigenFaceSearchTier currentTier = EigenFaceSearchTier::Strong;
    int currentConfidence = getStrongConfidence();
    int x = 0;
    foreach (int conf, sortedList)
    {
        while (conf < currentConfidence)
        {
            // down a tier
            switch (currentTier)
            {
            case EigenFaceSearchTier::Strong:
                if (1 == x)
                {
                    result[0] = EigenFaceSearchTier::Best;
                    DETAIL("Upgrade to Best");
                }
                else if (x >= getStrongPersonCount())
                {
                    // too many to be strong, re-declare them to be just Possible
                    int x2 = x;
                    DETAIL("Demote %1 to Possible", x2);
                    while (--x2 > 0)
                        result[x2] = EigenFaceSearchTier::Possible;
                }

                currentTier = EigenFaceSearchTier::Possible;
                currentConfidence = getPossibleConfidence();
                break;

            case EigenFaceSearchTier::Possible:
                currentTier = EigenFaceSearchTier::Weak;
                currentConfidence = getMinConfidence();
                break;

            case EigenFaceSearchTier::Weak:
                currentTier = EigenFaceSearchTier::NoMatch;
                currentConfidence = 0;
                break;

            case EigenFaceSearchTier::NoMatch:
            default:
                ERRMSG("Unhandled EigenFaceMatchTier: %1",
                      currentTier.name());
            }
        }
        result[x++] = currentTier;
        TRACE("Rank %1 is %2 %3", x, currentTier.name(), conf);
    }

    DETAIL("Done: x=%1 n=%2 tier=%3", x, n, currentTier.name());
    if (EigenFaceSearchTier::Strong == currentTier
            && x >= getStrongPersonCount())
    {
        DETAIL("Demote all to Weak");
        result.fill(EigenFaceSearchTier::Weak);
    }

    return result.toList();
}

int EigenFaceSearchSettings::confidence(qreal distance)
{
    return qBound(1, qRound(1000.0 / (1.0 + distance)), 999);
}

qreal EigenFaceSearchSettings::distance(int confidence)
{
    return 1000.0 / qMax(0.1, (qreal)confidence) - 1.0;
}

bool EigenFaceSearchSettings::getPersonMode() const
{
    return PersonMode;
}

void EigenFaceSearchSettings::setPersonMode(bool newPersonMode)
{
    if (PersonMode == newPersonMode)
        return;
    PersonMode = newPersonMode;
}

void EigenFaceSearchSettings::setPersonMethod(int newPersonMethod)
{
    if (PersonMethod == newPersonMethod)
        return;
    PersonMethod = newPersonMethod;
}

int EigenFaceSearchSettings::getMaxPersonFaces() const
{
    return MaxPersonFaces;
}

void EigenFaceSearchSettings::setMaxPersonFaces(int newMaxPersonFaces)
{
    if (MaxPersonFaces == newMaxPersonFaces)
        return;
    MaxPersonFaces = newMaxPersonFaces;
}
