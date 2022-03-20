/** @file EigenFaceMatcherSettings.h
*
*	Public declarations for INDI project EigenFaceMatcherSettings
*/

#pragma once
#include <qglobal.h>

#include <QObject>
#include <DDTcore.h>
#include <math.h>

class EigenFaceSearchTier;

class EigenFaceSearchSettings : public QObject
{
    Q_OBJECT
public:
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

    bool getPersonMode() const;
    int getMaxPersonFaces() const;
    int getPersonMethod() const { return PersonMethod; }
    qreal getMaxResults() const { return MaxResults; }
    qreal getMaxFaces() const { return MaxFaces; }
    qreal getMaxDistance() const;
    qreal getMinDistance() const;
    int getStrongConfidence() const;
    int getStrongPersonCount() const;
    int getPossibleConfidence() const;
    int getMinConfidence() const;
    int getTierMethod() const;

public slots:
    void setPersonMode(bool newPersonMode);
    void setPersonMethod(int newPersonMethod);
    void setMaxPersonFaces(int newMaxPersonFaces);
    void setMaxResults(qreal newMaxResults);
    void setMaxFaces(qreal newMaxFaces);
    void setMaxDistance(qreal newMaxDistance);
    void setMinDistance(qreal newMinDistance);
    void setStrongConfidence(int newStrongConfidence);
    void setStrongPersonCount(int newStrongPersonCount);
    void setPossibleConfidence(int newPossibleConfidence);
    void setMinConfidence(int newMinConfidence);
    void setTierMethod(int newTierMethod);

signals:

private:
    bool PersonMode=true;
    int PersonMethod=0;
    int MaxPersonFaces=5;
    qreal MaxResults=99.0;
    qreal MaxFaces=99.0;
    qreal MaxDistance=1000;
    qreal MinDistance=0.0000001;
    int StrongConfidence=850;
    int StrongPersonCount=3;
    int PossibleConfidence=700;
    int MinConfidence=500;
    int TierMethod=0;
}; // EigenFaceMatcherSettings
