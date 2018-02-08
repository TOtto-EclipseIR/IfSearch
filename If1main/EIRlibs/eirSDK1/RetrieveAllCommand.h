#ifndef RetrieveAllCOMMAND_H
#define RetrieveAllCOMMAND_H
#include "eirSDK1.h"

#include "SdkCommand.h"

#include <QtCore/QString>
#include <QtGui/QImage>

class EIRSDK1SHARED_EXPORT RetrieveAllCommand : public SdkCommand
{
public:
    RetrieveAllCommand(void);
    RetrieveAllCommand(const SdkCommand & other);
    void parseResult(void);
    void parseOutput(void);
    int personCount(void) const;
    QString personId(const int personIndex) const;
    int faceCount(const int personIndex) const;
    QImage faceImage(const int personIndex,
                      const int faceIndex);
    int faceKey(const int personIndex,
                const int faceIndex);
};

#endif // RetrieveAllCOMMAND_H
