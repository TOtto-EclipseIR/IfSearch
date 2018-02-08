#ifndef IdGenerator_H
#define IdGenerator_H
#include "eirCore.h"

#include "VariableSet.h"

#include <QtCore/QLine>
#include <QtCore/QRect>

#include "../eirTypes/MillisecondDelta.h"
#include "../eirTypes/MillisecondTime.h"
#include "BasicId.h"

#define IDGENERATOR_VARIABLESET(TIVD) \
    TIVD(QRect,             DetectRect,     QRect,      QRect()) \
    TIVD(QLineF,            EyeLine,        QLineF,     QLineF()) \
    TIVD(int,               Quality,        int,        0) \
    TIVD(int,               Consistency,    int,        0) \
    TIVD(int,               Confidence,     int,        0) \
    TIVD(int,               Rank,           int,        0) \
    TIVD(QChar,             Tier,           QChar,      QChar('?')) \
    TIVD(int,               FrameNumber,    int,        0) \
    TIVD(int,               BestQuality,    int,        0) \
    TIVD(int,               FaceNumber,     int,        0) \
    TIVD(MillisecondDelta,  BaseMsd,        qint64,     0) \
    TIVD(MillisecondTime,   ModifiedMst,    qint64,     0) \
    TIVD(BasicId,           FrameId,        QString,    QString()) \
    TIVD(QString,           FileName,       QString,    QString()) \

class EIRCORESHARED_EXPORT IdGenerator : public VariableSet
{
public:
    DECLARE_VARIABLESET(IDGENERATOR_VARIABLESET);
    IdGenerator(void);
    void setFaceFormat(const QString formatString);
    void setFrameFormat(const QString formatString);
    BasicId face(const QString & outputType);
    BasicId frame(const QString & outputType);

private:
    enum indicies
    {
        Face = 0,
        Frame,
    };
    BasicId generate(const int formatIndex,
                     const QString & outputType);
    qint64 numericValue(const QChar c);
    QString stringValue(const QChar c,
                        const QString & outputType);

private:
    static QChar trigger;
    static QChar pathSub;
};

#endif // IdGenerator_H
