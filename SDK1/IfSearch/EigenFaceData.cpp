#include <EigenFace.h>
#include <EigenFaceData.h>

#include <InfoMacros.h>

#include <QCoreApplication>
#include <QByteArray>
#include <QDir>
#include <QDomDocument>
#include <QDomElement>
#include <QFile>
#include <QString>

//#define WRITE_XML
//#define SPLIT_XMLS



//Return EigenFaceData::staticReturn;
const QString EigenFaceData::bitMaskTagName("INDI-EigenFaceGenerator-BitMask");
const QString EigenFaceData::weightMaskTagName("INDI-EigenFaceGenerator-WeightMask");
const QString EigenFaceData::meanFaceTagName("INDI-EigenFaceGenerator-MeanFace");
const QString EigenFaceData::outputMaskTagName("INDI-EigenFaceGenerator-OutputMask");
const QString EigenFaceData::eigenFaceLayerTagName("INDI-EigenFaceGenerator-EigenFaceLayer");

EigenFaceData::EigenFaceData(const QString & eigenClass)
    : EigenClass(eigenClass)
    , Weights(this)
{
    clear();
    Return::add(EigenFace::ReturnStatusNull, "INDI EigenFace Null Status", Warning);
    Return::add(EigenFace::ReturnNoData, "INDI EigenFace No Data", Error);
    Return::add(EigenFace::ReturnBadFltFile, "Invalid FLT File Data in %1: %2", Error);
    Return::add(EigenFace::ReturnInvalidStructure, "Invalid %1 structure", Error);
    Return::add(EigenFace::ReturnConflict, "Conflicting Data for %1: %2 and %2", Error);
} // c'tor

EigenFaceData::~EigenFaceData()
{
} // d'tor

void EigenFaceData::clear(void)
{
    ActivePixels = 0;
    ActiveLayers = 0;
    EigenSize = QSize();
    EigenEyeLine = Eyes();
    Mask.clear();
    OutputMask.clear();
    Weights.clear();
    layers.fill(EigenFaceMaskedArray(this), MAX_LAYERS);
    meanFaces.clear();
} // clear()

bool EigenFaceData::isValid(void) const
{
    if (EigenClass.isEmpty())
        return false;
    if (EigenSize.isNull() || EigenEyeLine.isNull())
        return false;
    if ( ! Mask.isValid())
        return false;
    // TODO More
    return true;
} // isValid()

Return EigenFaceData::checkRootAttributes(const QDomElement & de)
{
    QString Class	= de.attribute("Class");
    QString Name	= de.attribute("Name");
    QString Desc	= de.attribute("Description");
    int Pixels		= de.attribute("Pixels").toInt();
    int Layers		= de.attribute("Layers").toInt();
    int Width		= de.attribute("Width").toInt();
    int Height		= de.attribute("Height").toInt();
    int LeftEyeX	= de.attribute("LeftEyeX").toInt();
    int LeftEyeY	= de.attribute("LeftEyeY").toInt();
    int RightEyeX	= de.attribute("RightEyeX").toInt();
    int RightEyeY	= de.attribute("RightEyeY").toInt();
    QSize newSize = QSize(Width, Height);
    Eyes newLine = Eyes(QPoint(LeftEyeX,  LeftEyeY),
                        QPoint(RightEyeX, RightEyeY));

    if ( ! Class.isEmpty() && ! EigenClass.isEmpty() && Class != EigenClass)
        return Return(EigenFace::ReturnConflict, "Class", Class, EigenClass);
    if (Pixels && ActivePixels && Pixels != ActivePixels)
        return Return(EigenFace::ReturnConflict, "ActivePixels", ActivePixels, Pixels);
    if (Layers && ActiveLayers && Layers != ActiveLayers)
        return Return(EigenFace::ReturnConflict, "ActiveLayers", ActiveLayers, Layers);
    if ( ! newSize.isEmpty() && ! EigenSize.isEmpty() && newSize != EigenSize)
        return Return(EigenFace::ReturnConflict, "EigenSize", EigenSize, newSize);
    if ( ! newLine.isNull() && ! EigenEyeLine.isNull() && newLine != EigenEyeLine)
        return Return(EigenFace::ReturnConflict, "EigenEyeLine", EigenEyeLine, newLine);

    if (EigenClass.isEmpty())
        EigenClass = Class;
    if ( ! ActivePixels)
        ActivePixels = Pixels;
    if ( ! ActiveLayers)
        ActiveLayers = Layers;
    if (EigenSize.isEmpty())
        EigenSize = newSize;
    if (EigenEyeLine.isNull())
        EigenEyeLine = newLine;

    return Return();
} // checkRootAttributes()

Return EigenFaceData::loadFrom(QFile & xmlFile)
{
    Return rtn;
    QString errMsg;
    int errLine, errCol;
    QDomDocument doc;

    if ( ! xmlFile.open(QIODevice::ReadOnly))
        rtn = Return::qfileError(xmlFile);
    else if ( ! doc.setContent(&xmlFile, &errMsg, &errLine, &errCol))
        rtn = Return(Return::ReturnXmlParse,
                            xmlFile.fileName(), errMsg, errLine, errCol);
    if (rtn.isWarn())
        return rtn;

    QDomDocumentType dt = doc.doctype();
    QDomElement root = doc.documentElement();
    QDomElement de = root.firstChildElement();
    rtn = checkRootAttributes(root);

    while ( ! de.isNull() && ! rtn.isError())
    {
        if (eigenFaceLayerTagName == de.tagName())
        {
            EigenFaceMaskedArray layer(this);
            rtn = layer.read(de);
            int id = layer.id();
            if ( ! rtn.isError())
            {
                layers.replace(id, layer);
                if (0 == id)
                    DETAIL("%1 from %2", eigenFaceLayerTagName,
                       xmlFile.fileName());
            }
        }
        else if (meanFaceTagName == de.tagName())
        {
            EigenFaceMaskedArray mean(this);
            rtn = mean.read(de);
            int id = mean.id();
            if ( ! rtn.isError())
            {
                meanFaces.insert(id, mean);
                DETAIL("%1 from %2 Id=%3", meanFaceTagName,
                       xmlFile.fileName(), id);
            }
        }
        else if (bitMaskTagName == de.tagName())
        {
            rtn = Mask.read(de);
            if ( ! rtn.isError())
            {
                if (EigenSize != Mask.size())
                    rtn = Return(EigenFace::ReturnConflict,
                                      "BitMask size", EigenSize, Mask.size());
                else if (ActivePixels != Mask.count())
                    rtn = Return(EigenFace::ReturnConflict,
                                      "BitMask bits", ActivePixels, Mask.count());
            }
            if ( ! rtn.isError())
                DETAIL("%1 from %2", bitMaskTagName, xmlFile.fileName());
        }
        else if (outputMaskTagName == de.tagName())
        {
            rtn = OutputMask.read(de);
            if ( ! rtn.isError())
                if (EigenSize != Mask.size())
                    rtn = Return(EigenFace::ReturnConflict,
                                      "OutputMask size", EigenSize, Mask.size());
            if ( ! rtn.isError())
                DETAIL("%1 from %2", outputMaskTagName, xmlFile.fileName());
        }
        else if (weightMaskTagName == de.tagName())
        {
            rtn = Weights.read(de);
            if ( ! rtn.isError())
                DETAIL("%1 from %2", weightMaskTagName, xmlFile.fileName());
        }
        de = de.nextSiblingElement();

        updateProgress();
    } // while(!null !error)

    return rtn;
} // loadFrom()

Return EigenFaceData::create(const QDir & initDir)
{
    Return rtn;

    layers.resize(MAX_LAYERS);

    QStringList nameFilter;
    nameFilter << "*.xml";
    QStringList xmlFileList = initDir.entryList(nameFilter, QDir::Files);
    if (xmlFileList.isEmpty())
        return Return(EigenFace::ReturnNoData);

    foreach (QString xmlFileName, xmlFileList)
    {
        QFile file(initDir.filePath(xmlFileName));
        rtn = loadFrom(file);
        if (rtn.isError())
            break;
    } // foreach()

    if (Weights.isNull())
        Weights.fill(1.0);

    if (OutputMask.isNull())
        OutputMask = Mask;

    // TODO: Scan Layers

// IEIGENFACE_PRELIMINARY
/*
    QImage bitImage = ptr->Mask.toImage(QImage::Format_Indexed8);
    bitImage.save("BitMask.PNG");
    QImage outImage = ptr->OutputMask.toImage(QImage::Format_Indexed8);
    outImage.save("OutMask.PNG");
*/

    return rtn;
} // create(dir)

void EigenFaceData::updateProgress(void)
{
    int max, val;
    max = (ActiveLayers ? ActiveLayers : MAX_LAYERS) + 4 + meanFaces.size();
    val = meanFaces.size();
    foreach (EigenFaceMaskedArray v, layers)
        if (v.isValid())
            ++val;
    if (Mask.isValid())			++val;
    if (OutputMask.isValid())	++val;
    if (Weights.isValid())		++val;

    emit initProgress(val, max);
#ifndef __linux
    qApp->processEvents();
#endif
} // updateProg()
