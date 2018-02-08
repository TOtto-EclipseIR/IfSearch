#ifndef SDKRESULT_H
#define SDKRESULT_H
#include "eirSDK1.h"

#include <QtCore/QString>
#include <QtGui/QImage>

#include "../eirVariable/VariableGroup.h"
class VariableTable;
class SdkCommand;

#define SDKRESULT_GROUPVARIABLES(ITD) \
    ITD(Sequence, int, 0) \
    ITD(Mode, QString, QString()) \
    ITD(Command, QString, QString()) \
    ITD(Pause, bool, true) \
    ITD(Status, QString, QString()) \
    ITD(Reason, QString, QString()) \
    ITD(Results, QStringList, QStringList()) \
    ITD(PersonCount, int, 0) \

class EIRSDK1SHARED_EXPORT SdkResult : public VariableGroup
{
public:
    DECLARE_GROUPVARIABLES(SDKRESULT_GROUPVARIABLES);
    explicit SdkResult(void);
    bool from(SdkCommand * command);
    bool isSearch(void) const;
    bool isRetrieve(void) const;
    bool isEnroll(void) const;
    bool isValid(void) const;
    bool isError(void) const;

    QString personId(const int rank) { return get("PersonId", rank).toString(); }
    QString personDescription(const int rank) { return get("PersonDescription", rank).toString(); }
    int personKey(const int rank) { return get("PersonKey", rank).toInt(); }
    int personConfidence(const int rank) { return get("MatchConfidence", rank).toInt(); }
    int faceKey(const int rank, const int fx) { return get("FaceKey", rank, fx).toInt(); }
    QImage faceImage(const int rank, const int fx) { return get("FaceImage", rank, fx).value<QImage>(); }
    int retrieveFaceKey(const int rank) { return get("FaceKey", rank).toInt(); }
    QImage retrieveFaceImage(const int rank) { return get("FaceImage", rank).value<QImage>(); }

private:
    QVariant get(const QString & name,
                 const int rank,
                 const int fx=0);
};

#endif // SDKRESULT_H
