#include "SdkCommand.h"

#include <QtDebug>
#include <QtCore/QVariant>
#include <QtGui/QImage>

#include "../eirExe/Log.h"

int SdkCommand::sequence = 10000000;
const QStringList SdkCommand::complete_strings(QStringList() << "Done"
                                                             << "Error"
                                                             << "NotFound");

SdkCommand::SdkCommand(void)
    : VariableSet(++sequence, QString(), "SdkCommand")
{
    DEFINE_VARIABLESET_CTORS(SDKCOMMAND_VARIABLESET);
}

bool SdkCommand::isSuccess(void)
{
    return "Done" == getStatus();
}

bool SdkCommand::isComplete(void)
{
    return complete_strings.contains(getStatus(), Qt::CaseInsensitive);
}

bool SdkCommand::isUpdate(void)
{
    return getStatus().startsWith("Update ", Qt::CaseInsensitive);
}

bool SdkCommand::isComplete(const QString & status)
{
    return complete_strings.contains(status, Qt::CaseInsensitive);
}

bool SdkCommand::isSearch(void) const
{
    return "Search" == getMode();
}

bool SdkCommand::isRetrieve(void) const
{
    return "Retrieve" == getMode();
}

bool SdkCommand::isEnroll(void) const
{
    return "Enroll" == getMode();
}

bool SdkCommand::isError(void) const
{
    return "Error" == getStatus();
}

bool SdkCommand::isValid(void) const
{
    if ( ! (isSearch() || isRetrieve() || isEnroll()) )
        return false;
    if ("Done" != getStatus())
        return false;
    return true;
}

void SdkCommand::addInputImage(const QImage & img,
                               const QString & id)
{
    FUNCTION(img.size(), id);
    int n = getImagesIn();
    VariableId vid("Input");
    vid.append(QString::number(++n));
    VariableId vidId(vid);
    VariableId vidImage(vid);
    vidId.append("Id");
    vidImage.append("Image");
    QVariant var(QVariant::Image);
    var = img;
    setImagesIn(n);
    set(vidId, id);
    set(vidImage, var);
}

void SdkCommand::parseResult(void)
{

}

void SdkCommand::parseOutput(void)
{

}
