#include "RetrieveAllCommand.h"

#include "../eirExe/Log.h"

RetrieveAllCommand::RetrieveAllCommand(void)
{
    setName("RetrieveAllCommand");
    setMode("RetrieveAll");
    setCommand("RetrieveAll");
}

RetrieveAllCommand::RetrieveAllCommand(const SdkCommand & other)
    : SdkCommand(other)
{
    //? ctor()
}

void RetrieveAllCommand::parseResult(void)
{

}

void RetrieveAllCommand::parseOutput(void)
{
    FUNCTION();
    QString outDirName = getOutBaseDir();
    if (outDirName.isEmpty())   return;
    QDir outDir(outDirName);
    if ( ! outDir.exists())     return;
    TRACE("RetrieveAllCommand::parseOutput() from %1", outDir.absolutePath());
    VariableId vid("Output");
    VariableId vidNumPersons(vid, "NumPersons");

    int nPerson = 0;
    QStringList dirNames = outDir.entryList(QStringList() << "PK*");
    foreach (QString dirName, dirNames)
    {
        if ( ! dirName.startsWith("PK")) continue;
        QDir personDir(outDir);
        EXPECT(personDir.cd(dirName));

        VariableId vidPerson(vid);
        vidPerson.append(QString::number(++nPerson));
        VariableId vidNumFace(vidPerson, "NumFace");
        VariableId vidPersonId(vidPerson, "PersonId");

        set(vidPersonId, dirName.mid(9));
        int nFace = 0;
        foreach (QFileInfo fi, personDir.entryInfoList(QStringList() << "*.PNG",
                                                       QDir::Files))
        {
            QString fn(fi.fileName());
            VariableId vidFace(vidPerson);
            vidFace.append(QString::number(++nFace));
            VariableId vidFaceKey(vidFace, "FaceKey");
            VariableId vidFaceFileName(vidFace, "FaceFileName");
            set(vidFaceKey, fn.mid(2, 6).toInt());
            set(vidFaceFileName, fi.absoluteFilePath());
            set(vidNumFace, nFace);
        }
        set(vidNumPersons, nPerson);
    }

}

int RetrieveAllCommand::personCount(void) const
{
    return value("Output/NumPersons").toInt();
}

QString RetrieveAllCommand::personId(const int personIndex) const
{
    VariableId vid("Output");
    vid.append(QString::number(personIndex));
    vid.append("PersonId");
    return value(vid).toString();
}

int RetrieveAllCommand::faceCount(const int personIndex) const
{
    VariableId vid("Output");
    vid.append(QString::number(personIndex));
    vid.append("NumFace");
    return value(vid).toInt();
}

QImage RetrieveAllCommand::faceImage(const int personIndex,
                                      const int faceIndex)
{
    VariableId vid("Output");
    vid.append(QString::number(personIndex));
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

int RetrieveAllCommand::faceKey(const int personIndex,
                                const int faceIndex)
{
    VariableId vid("Output");
    vid.append(QString::number(personIndex));
    vid.append(QString::number(faceIndex));
    VariableId vidFaceKey(vid, "FaceKey");
    return value(vidFaceKey).toInt();
}
