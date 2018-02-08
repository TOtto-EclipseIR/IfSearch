#ifndef FACEID_H
#define FACEID_H
#include "eirTypes.h"

#include "../eirCore/VariableGroup.h"

#include <QtCore/QLine>
#include <QtCore/QRect>

#define FACEID_GROUPVARIABLES(ITD) \
    ITD(DetectRect, QRect, QRect()) \
    ITD(EyeLine, QLineF, QLineF()) \
    ITD(Quality, int, 0) \
    ITD(Consistency, int, 0) \
    ITD(Confidence, int, 0) \
    ITD(Rank, int, 0) \
    ITD(FrameNumber, int, 0) \
    ITD(FaceNumber, int, 0) \
    ITD(ModifiedEms, qint64, 0) \
    ITD(FrameId, QString, QString()) \
    ITD(Format, QString, QString()) \

class EIRTYPESSHARED_EXPORT FaceId : public VariableGroup
{
public:
    DECLARE_GROUPVARIABLES(FACEID_GROUPVARIABLES);
    FaceId(void);
    QString generate(const QString & outputType);

private:
    qint64 numericValue(const QChar c);
    QString stringValue(const QChar c,
                        const QString & outputType);

private:
    static QChar trigger;
};

#endif // FACEID_H
