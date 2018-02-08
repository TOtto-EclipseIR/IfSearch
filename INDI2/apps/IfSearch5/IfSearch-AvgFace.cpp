#include "IfSearch.h"

#include <DDTcore.h>
#include <EigenFace.h>
#include <EigenFaceData.h>
#include <EigenFaceParameters.h>
#include <EigenFaceMaskedArray.h>
#include <FileWriteProfile.h>
#include <InfoMacros.h>
#include <Settings.h>

#include <Key.h>

#ifdef ENABLE_AVGFACE
#include <AverageFace.h>

bool IfSearch::avgFaceInit(void)
{
    NULLPTR(eigenParms);
    NULLPTR(fwpAvgFace);

    avgFace = new AverageFace(eigenParms->normalizedSize(),
                              eigenParms->normalEyes());
    NULLPTR(avgFace);

    int min = appSettings->value(tr("AvgFace/MinKey", "config"), 100001).toInt();
    int max = appSettings->value(tr("AvgFace/MaxKey", "config"), 199999).toInt();
    AverageFaceKeyTraits::setRange(min, max);

    QDir dir = fwpAvgFace->dir();
    QStringList nameFilters;
    nameFilters << "*." + fwpAvgFace->format();
    QStringList files = dir.entryList(nameFilters);
    PROGRESS("Reading %1 previous average faces", files.size());
    Info::flush();

    foreach (QString fileName, files)
    {
        if (fileName.startsWith("Avg"))
            continue;
        QImage image(dir.absoluteFilePath(fileName));
        avgFace->add(image, eigenParms->normalEyes());
        AverageFaceKey key(fileName.toInt());
        AverageFaceKey::take(key);
    }
    return true;
} // avgFaceInit()

bool IfSearch::avgFaceProcess(QImage normImage, Eyes normEyes)
{
    NULLPTR(avgFace);
    NULLPTR(fwpAvgFace);

    AverageFaceKey key = AverageFaceKey::newKey();
    avgFace->add(normImage, normEyes);
    fwpAvgFace->write(normImage, QString::number(key));
    return true;
} // avgFaceProcess()

bool IfSearch::avgFaceFinish(void)
{
    NULLPTR(avgFace);
    NULLPTR(fwpAvgFace);

    QImage avgImage = avgFace->average();
    fwpAvgFace->write(avgImage, "AvgFace");
    QImage eigenFace = EigenFace::normalize(avgImage,
                                            eigenParms->normalEyes(),
                                            eigenData->eigenSize(),
                                            eigenData->eigenEyes(),
                                            QImage::Format_Indexed8);
    EigenFaceMaskedArray avgArray(eigenData);
    avgArray.fromImage(eigenFace);
    QString xmlFileName = fwpAvgFace->dir().filePath("AvgFace.xml");
    QDomDocument doc;
    QDomElement root = doc.createElement("INDIfaceData");
    QDomElement de = doc.createElement(EigenFaceData::meanFaceTagName);
    avgArray.write(&de);
    root.appendChild(de);
    doc.appendChild(root);
    DDTcore::writeXmlFile(xmlFileName, doc);
    QImage avgEigen = avgArray.toImage();
    fwpAvgFace->write(avgEigen, "AvgEigen");
    return true;
} // avgFaceFinish()

#endif
