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
public:
    QRect             DetectRect;
    QLineF            EyeLine;
    int               Quality        ;
    int               Consistency    ;
    int               Confidence     ;
    int               Rank           ;
    QChar             Tier           =      QChar('?');
    int               FrameNumber    ;
    int               BestQuality    ;
    int               FaceNumber     ;
    MillisecondDelta  FrameMsd       =     0;
    MillisecondTime   ModifiedMst    =     0;
    BasicId           FrameId        ;
    QString           FileName       ;
    int               FaceKey        =        0;
    int               PersonKey      =        0;
    QString           PersonId           ;

};

#endif // IdGenerator_H
