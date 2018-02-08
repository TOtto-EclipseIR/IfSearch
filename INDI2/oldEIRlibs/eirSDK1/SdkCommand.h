#ifndef SDKCOMMAND_H
#define SDKCOMMAND_H
#include "eirSDK1.h"

#include <QtGui/QImage>

#include "../eirCore/VariableSet.h"


#define SDKCOMMAND_VARIABLESET(TIVD) \
    TIVD(int, Sequence, int, 0) \
    TIVD(QString, Mode, QString, QString()) \
    TIVD(QString, Command, QString, QString()) \
    TIVD(bool, Pause, bool, true) \
    TIVD(QString, Status, QString, QString()) \
    TIVD(QString, Reason, QString, QString()) \
    TIVD(QStringList, Results, QStringList, QStringList()) \
    TIVD(int, TimeoutMsec, int, 2000) \
    TIVD(int, ReadMsec, int, 300) \
    TIVD(int, ImagesIn, int, 0) \
    TIVD(int, ImagesOut, int, 0) \
    TIVD(QString, OutBaseDir, QString, QString()) \


class EIRSDK1SHARED_EXPORT SdkCommand : public VariableSet
{
public:
    DECLARE_VARIABLESET(SDKCOMMAND_VARIABLESET);
    SdkCommand(void);
    bool isSuccess(void);
    bool isComplete(void);
    bool isUpdate(void);
    bool isSearch(void) const;
    bool isRetrieve(void) const;
    bool isEnroll(void) const;
    bool isValid(void) const;
    bool isError(void) const;
    static bool isComplete(const QString & status);
    void addInputImage(const QImage & img, const QString & id);
    virtual void parseResult(void);
    virtual void parseOutput(void);

private:
    static int sequence;
    static const QStringList complete_strings;
};

#endif // SDKCOMMAND_H
