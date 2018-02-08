#include "EigenFaceSearchSettings.h"


#include <QtCore/QDynamicPropertyChangeEvent>
#include <QtCore/QEvent>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QMapIterator>
#include <QtCore/QVector>

#include "EigenFaceSearchTier.h"
#include "InfoMacros.h"

EigenFaceSearchSettings::EigenFaceSearchSettings(const Mode mode,
                         QObject * parent=0)
    : QObject(parent)
{
    DEFINE_QPROPERTIES_CTORS(EFSEARCHSETTINGS_QPROPERTIES);
    setObjectName("EigenFaceMatcherSettings");
    setDefaults(mode);
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

qreal EigenFaceSearchSettings::fMinDistance(void) const
{
    return (qreal)getMinDistance() / 1000.0;
}

qreal EigenFaceSearchSettings::fMaxDistance(void) const
{
    return getMaxDistance()
            ? ((qreal)getMaxDistance() / 1000.0)
            : 1.0;
}

int EigenFaceSearchSettings::maxFaces(const int totalFaces) const
{
    qreal f = getPersonMode() ? getMaxFaces() : getMaxResults();
    int n = totalFaces;
    if (f > 1.0)
        n = (int)f;
    else if ( ! qIsNull(f))
        n = qRound(f * (qreal)totalFaces);
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
    return n;
}

int EigenFaceSearchSettings::personMethod(const Mode mode,
                                            const int totalFaces)
{
    int pm = getPersonMethod();
    if (pm) return pm;

    switch (mode)
    {
    case FormalSearch:
    case CasualMatch:
        return (totalFaces < 50) ? 3 : 2;

    case Authenticate:
    case Verify:
        return 4;
    }
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
    int n = confidenceList.size();
    QVector<EigenFaceSearchTier> result(n);

    /* Sort Descending */
    QList<int> sortedList(confidenceList);
    if (needSort)
        qSort(sortedList.begin(), sortedList.end(), qGreater<int>());

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
