#include "SearchCommand.h"

#include "../eirExe/Log.h"

SearchCommand::SearchCommand(void)
{
    setName("SearchCommand");
    setMode("Search");
    setCommand("Search");
    set("Settings/PersonMode", true);
}

SearchCommand::SearchCommand(const SdkCommand & other)
    : SdkCommand(other)
{
    //? ctor()
}

void SearchCommand::parseResult(void)
{

}

void SearchCommand::parseOutput(void)
{
    FUNCTION();
    QString outDirName = getOutBaseDir();
    if (outDirName.isEmpty())   return;
    QDir outDir(outDirName);
    if ( ! outDir.exists())     return;
    TRACE("SearchCommand::parseOutput() from %1", outDir.absolutePath());
    VariableId vid("Output");
    VariableId vidNumResults(vid, "NumResults");

    int nResult = 0;
    foreach (QString dirName, outDir.entryList(QDir::NoDotAndDotDot | QDir::Dirs))
    {
        outDir.cd(dirName);
        VariableId vidPerson(vid);
        vidPerson.append(QString::number(++nResult));
        VariableId vidConfidence(vidPerson, "MatchConfidence");
        VariableId vidPersonId(vidPerson, "PersonId");
        VariableId vidNumFaces(vidPerson, "NumFaces");
        set(vidConfidence, dirName.mid(4,3).toInt());
        set(vidPersonId, dirName.mid(8));

        int nFace = 0;
        foreach (QFileInfo fi, outDir.entryInfoList(QDir::Files))
        {
            QString fn(fi.fileName());
            VariableId vidFace(vidPerson);
            vidFace.append(QString::number(++nFace));
            VariableId vidRank(vidFace, "Rank");
            VariableId vidConfidence(vidFace, "MatchConfidence");
            VariableId vidFaceKey(vidFace, "FaceKey");
            VariableId vidFaceFileName(vidFace, "FaceFileName");

            set(vidRank, fn.mid(1,2).toInt());
            set(vidConfidence, dirName.mid(4,3).toInt());
            set(vidFaceKey, fn.mid(10).toInt());
            set(vidFaceFileName, fi.absoluteFilePath());
        }
        set(vidNumFaces, nFace);

        outDir.cdUp();
    }
    set(vidNumResults, nResult);
}

int SearchCommand::resultCount(void) const
{
    return value("Output/NumResults").toInt();
}

QImage SearchCommand::resultFace(const int rank,
                                 const int faceIndex)
{
    VariableId vid("Output");
    vid.append(QString::number(rank));
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

int SearchCommand::numFaces(const int rank) const
{
    VariableId vid("Output");
    vid.append(QString::number(rank));
    return value(VariableId(vid, "NumFaces")).toInt();
}

QString SearchCommand::personId(const int rank) const
{
    VariableId vid("Output");
    vid.append(QString::number(rank));
    return value(VariableId(vid, "PersonId")).toString();
}

int SearchCommand::personMatchConfidence(const int rank) const
{
    VariableId vid("Output");
    vid.append(QString::number(rank));
    return value(VariableId(vid, "MatchConfidence")).toInt();
}
