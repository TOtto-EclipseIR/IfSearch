#ifndef RetrieveCOMMAND_H
#define RetrieveCOMMAND_H
#include "eirSDK1.h"

#include "SdkCommand.h"

#include <QtCore/QString>
#include <QtGui/QImage>

class EIRSDK1SHARED_EXPORT RetrieveCommand : public SdkCommand
{
public:
    RetrieveCommand(void);
    RetrieveCommand(const SdkCommand & other);
    void parseResult(void);
    void parseOutput(void);
    int resultCount(void) const;
    QImage resultFace(const int faceIndex);
    int faceKey(const int faceIndex);
};

#endif // RetrieveCOMMAND_H
