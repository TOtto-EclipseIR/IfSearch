/** @file EigenFaceMatcherSettings.h
*
*	Public declarations for INDI project EigenFaceMatcherSettings
*/

#pragma once
#include <qglobal.h>
#ifdef INDIEF_LIB
# define INDIEF_EXPORT Q_DECL_EXPORT
#else
# define INDIEF_EXPORT Q_DECL_IMPORT
#endif

#include <QObject>
#include <DDTcore.h>
#include <math.h>
#include <QProperty.h>

class EigenFaceSearchTier;

#define EFSEARCHSETTINGS_QPROPERTIES(TND) \
    TND(bool, PersonMode, true)   \
    TND(int, PersonMethod, 0)   \
    TND(int, MaxPersonFaces, 5)   \
    TND(qreal, MaxResults, 99.0)   \
    TND(qreal, MaxFaces, 99.0)   \
/*    TND(int, DuplicateThreshold, 6) */  \
    TND(qreal, MaxDistance, 1000)   \
    TND(qreal, MinDistance, 0.0000001)   \
    TND(int, StrongConfidence, 850)   \
    TND(int, StrongPersonCount, 3)   \
    TND(int, PossibleConfidence, 700)   \
    TND(int, MinConfidence, 500) \
    TND(int, TierMethod, 0)   \

class INDIEF_EXPORT EigenFaceSearchSettings : public QObject
{
    Q_OBJECT
    DECLARE_QPROPERTIES(EFSEARCHSETTINGS_QPROPERTIES)

    enum Mode
    {
        CasualMatch = 1,
                FormalSearch,
                Verify,
                Authenticate,
    };

public:
    EigenFaceSearchSettings(const Mode mode,
                            QObject * parent=0);
    void setDefaults(const Mode mode);

    int maxFaces(const int totalFaces) const;
    int maxResults(int totalResults) const;
//    qreal fDuplicate(void) const;
  //  bool checkDuplicates(void) const;
    qreal fMinDistance(void) const;
    qreal fMaxDistance(void) const;
    int personMethod(const Mode mode,
                     const int totalFaces);
    QList<EigenFaceSearchTier>
    tiersForConfidence(
            const QList<int> & confidenceList,
            const bool needSort=false) const;
    EigenFaceSearchTier tierForConfidence(const int confidence);

public:
    static int confidence(qreal distance);
    static qreal distance(int confidence);

}; // EigenFaceMatcherSettings
