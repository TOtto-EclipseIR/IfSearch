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
    TIVD(MillisecondDelta,  FrameMsd,        qint64,     0) \
    TIVD(MillisecondTime,   ModifiedMst,    qint64,     0) \
    TIVD(BasicId,           FrameId,        QString,    QString()) \
    TIVD(QString,           FileName,       QString,    QString()) \
    TIVD(int,               FaceKey,        int,        0) \
    TIVD(int,               PersonKey,      int,        0) \
    TIVD(QString,           PersonId,       QString,    QString()) \

class EIRCORESHARED_EXPORT IdGenerator : public VariableSet
{
public:
    DECLARE_VARIABLESET(IDGENERATOR_VARIABLESET);
    IdGenerator(void);
    void setFaceFormat(const QString & formatStrings);
    void setFrameFormat(const QString & formatStrings);
    QString faceFormat(void) const;
    QString frameFormat(void) const;
    BasicId face(const QString & className) const;
    BasicId frame(const QString & className) const;

private:
    void setFormat(const QString & className,
                   const QString & formatString);
    QString getFormat(const QString & className) const;
    BasicId generate(const QString & className,
                     const QString & outputClass) const;
    qint64 numericValue(const QChar c) const;
    QString stringValue(const QChar c,
                        const QString & className) const;

private:
    QMap<QString, QString> sortable_format_map;
    static QChar trigger;
    static QChar pathSub;
};

#endif // IdGenerator_H
