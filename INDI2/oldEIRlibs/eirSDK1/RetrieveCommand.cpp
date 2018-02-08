#include "RetrieveCommand.h"

#include "../eirExe/Log.h"

RetrieveCommand::RetrieveCommand(void)
{
    setName("RetrieveCommand");
    setMode("Retrieve");
    setCommand("Retrieve");
}

RetrieveCommand::RetrieveCommand(const SdkCommand & other)
    : SdkCommand(other)
{
    //? ctor()
}

void RetrieveCommand::parseResult(void)
{

}

void RetrieveCommand::parseOutput(void)
{
    FUNCTION();
    QString outDirName = getOutBaseDir();
    if (outDirName.isEmpty())   return;
    QDir outDir(outDirName);
    if ( ! outDir.exists())     return;
    TRACE("RetrieveCommand::parseOutput() from %1", outDir.absolutePath());
    VariableId vid("Output");
    VariableId vidNumResults(vid, "NumResults");

    int n = 0;
    foreach (QFileInfo fi, outDir.entryInfoList(QStringList() << "*.PNG",
                                                QDir::Files))
    {
        QString fn(fi.fileName());
        VariableId vidFace(vid);
        vidFace.append(QString::number(++n));
        VariableId vidFaceKey(vidFace, "FaceKey");
        VariableId vidFaceFileName(vidFace, "FaceFileName");
        set(vidFaceKey, fn.mid(2,6).toInt());
        set(vidFaceFileName, fi.absoluteFilePath());
    }
    set(vidNumResults, n);
}

int RetrieveCommand::resultCount(void) const
{
    return value("Output/NumResults").toInt();
}

QImage RetrieveCommand::resultFace(const int faceIndex)
{
    VariableId vid("Output");
    vid.append(QString::number(faceIndex));
    VariableId vidFaceImage(vid, "FaceImage");
    VariableId vidFaceFileName(vid, "FaceFileName");
    if (contains(vidFaceImage))
        return value(vidFaceImage).value<QImage>();
    else
    {
        QImage result(value(vidFaceFileName).toString());
        if ( ! result.isNull()) set(vidFaceImage, result);
        return result;
    }
}

int RetrieveCommand::faceKey(const int faceIndex)
{
    VariableId vid("Output");
    vid.append(QString::number(faceIndex));
    VariableId vidFaceKey(vid, "FaceKey");
    return value(vidFaceKey).toInt();
}
