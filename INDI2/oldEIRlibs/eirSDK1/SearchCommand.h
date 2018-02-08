#ifndef SEARCHCOMMAND_H
#define SEARCHCOMMAND_H
#include "eirSDK1.h"

#include "SdkCommand.h"

#include <QtCore/QString>
#include <QtGui/QImage>

class EIRSDK1SHARED_EXPORT SearchCommand : public SdkCommand
{
public:
    SearchCommand(void);
    SearchCommand(const SdkCommand & other);
    void parseResult(void);
    void parseOutput(void);
    int resultCount(void) const;
    int numFaces(const int rank) const;
    QImage resultFace(const int rank,
                      const int faceIndex);
    QString personId(const int rank) const;
    int personMatchConfidence(const int rank) const;
};

#endif // SEARCHCOMMAND_H
