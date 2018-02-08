#include <EigenFaceGenerator.h>
#include <EigenFace.h>
#include <EigenFaceData.h>
#include <EigenFaceParameters.h>
#include <EigenFaceTemplate.h>

#include <Detector.h>
#include <ImageCache.h>

using namespace INDI;

EigenFaceGenerator::EigenFaceGenerator(QObject * parent, EigenFaceData * data, 
                                       EigenFaceParameters * parms, int instance)
                                           : QObject(parent), data(data), parms(parms), instanceId(instance)
                                           , ffd(0), leyed(0), reyed(0)
{
    DDT::Return::add(EigenFace::ReturnStatusNull, "INDI EigenFace Null Status", DDT::Warning);
    DDT::Return::add(EigenFace::ReturnNoData, "INDI EigenFace No Data", DDT::Error);
    DDT::Return::add(EigenFace::ReturnNoTemplate, "INDI EigenFace No Template", DDT::Error);
    DDT::Return::add(EigenFace::ReturnNoParameters, "INDI EigenFace No Parameters", DDT::Error);
    DDT::Return::add(EigenFace::ReturnNoDetector, "INDI EigenFace No %1 Detector", DDT::Error);
    DDT::Return::add(EigenFace::ReturnDetectorError, "INDI EigenFace Detector Error: %1", DDT::Fatal);
    DDT::Return::add(EigenFace::ReturnException, "INDI EigenFace Detector Exception: %1", DDT::Error);
    DDT::Return::add(EigenFace::ReturnBadFltFile, "Invalid FLT File Data in %1: %2", DDT::Error);
    DDT::Return::add(EigenFace::ReturnConflict, "Conflicting Data for %1: %2 and %2", DDT::Error);
    DDT::Return::add(EigenFace::ReturnInvalidStructure, "Invalid %1 structure", DDT::Error);
    DDT::Return::add(EigenFace::ReturnNullImage, "Null %1 image", DDT::Error);
    DDT::Return::add(EigenFace::ReturnNoHead, "No head location specified", DDT::Error);
    DDT::Return::add(EigenFace::ReturnNoEyes, "No eye points specified", DDT::Error);
    DDT::Return::add(EigenFace::ReturnLowConsistency, "Low consistency for EigenFace vector", DDT::Error);

    clear(ClearAll);
    EyeScale = RoiScale = MinConsistency = 0;
    Factor = 1.0;

    // Setup Head Detector
    ffd = new INDI::FrontalFaceDetector(0, this);
    ffd->selectDetector(ffd->detectorsDefault());
    ffd->setGroupMethod(INDI::HaarDetector::GroupByCenters);
    ffd->setMinQuality(300);
    ffd->setMaxResults(1);
    ffd->setMinAcross(0);
    ffd->setMaxAcross(3);
    ffd->setFactor(5);
    if (data)
        ffd->setMinPixels(2 * data->eigenEyes().distance());

    // Setup Eye Detectors
    leyed = new INDI::EyeDetector(leftInterface(), this);
    leyed->selectDetector(LeftDetector.isEmpty()
                            ? leyed->detectorsDefault()
                            : LeftDetector);
    leyed->setMaxResults(1);
    leyed->setGroupMethod(3);

    reyed = new INDI::EyeDetector(rightInterface(), this);
    reyed->selectDetector(RightDetector.isEmpty()
                            ? reyed->detectorsDefault()
                            : RightDetector);
    reyed->selectDetector(reyed->detectorsDefault());
    reyed->setMaxResults(1);
    reyed->setGroupMethod(3);
}

EigenFaceGenerator::~EigenFaceGenerator()
{
}

bool EigenFaceGenerator::isValid(void) const 
{ 
    return data && data->isValid();
}

void EigenFaceGenerator::clear(enum Clear what)
{
    switch (what)
    {
    case ClearAll:
    case ClearImage:
        originalImage = QImage();
        fSet.clear();
        eyeRoiMethod = NotSpecified;
    case ClearHead:
        head = QRect();
        HeadScale = 1.0;
        MsecGenerate = 0;
    case ClearEyes:
        ExpectedEyes = origEyes = Eyes();
        LeftEyeRoi = RightEyeRoi = QRect();
        LeftEyePerformance.clear();
        RightEyePerformance.clear();
    case ClearAdjusted:
        adjEyes = Eyes();
        Consistency = 0;
        eigenImage = QImage();
        normalImage = QImage();
        differences.clear();
        tpl.clear();
        MsecTemplate = 0;
        break;
    }
} // clear(what) private

void EigenFaceGenerator::setHeadBox(QRect r) 
{ 
    clear(ClearHead);
    head = r;
}

void EigenFaceGenerator::setEyePosition(Eyes eyeLine)
{
    clear(ClearEyes);
    origEyes = eyeLine;
}

Eyes EigenFaceGenerator::normalEyes(void) const
{ 
    return parms ? parms->normalEyes() : Eyes();
}

Eyes EigenFaceGenerator::eigenEyes(void) const
{ 
    return data ? data->eigenEyes() : Eyes();
}


DDT::Return EigenFaceGenerator::setImage(const QString & fileName)
{
    QImage image(fileName);
    return setImage(image, DDT::ImageInfo(image), fileName);
} // setImage(file)

DDT::Return EigenFaceGenerator::setImage(DDT::ImageCache * cache, const QString & imageId)
{
    if (cache)
    {
        QImage image = cache->getImage(imageId);
        DDT::ImageInfo info = cache->getInfo(imageId);
        QString imageSource = cache->getImageFileName(imageId);
        if (imageSource.isEmpty())
            imageSource = imageId;
        return setImage(image, info, imageSource);
    }
    else
        return DDT::Return(EigenFace::ReturnStatusNull);
} // setImage(cache)

DDT::Return EigenFaceGenerator::setImage(const QImage & image, 
                                         const DDT::ImageInfo & info,
                                         const QString & imageSource)
{
    clear(ClearImage);
    // validate image
    originalImage = image;
    DDT::ImageInfo ii(image);
    fSet = ii.face(0);
    fSet.merge(info.face(0));
    if (fSet.get(DDT::Feature::ImageSource).isNull() && ! imageSource.isEmpty())
        fSet.set(DDT::Feature::ImageSource, imageSource);

    head = fSet.get(DDT::Feature::HeadBox).toRect();
    origEyes = fSet.get(DDT::Feature::EyeLine).toLine();
    QString method = fSet.get(DDT::Feature::HeadMethod).toString();
    if (method.contains("Adjust=1") && method.contains("GroupMethod=3"))
        eyeRoiMethod = AdjustedNeighbors;
    else if (method.contains("GroupMethod=3"))
        eyeRoiMethod = Neighbors;
    else if ( ! method.isEmpty())
        eyeRoiMethod = Other;

    return DDT::Return();
} // setImage()

QImage EigenFaceGenerator::getEigenImage(void)
{
    if (eigenImage.isNull())
    {
        DDT::Return rtn = adjustEyes();
        if (rtn.isError())
            emit error(rtn.toString());
    }
    return eigenImage;
} // getEigenImage()


QImage EigenFaceGenerator::getNormalImage(void)
{
    if (normalImage.isNull())
    {
        DDT::Return rtn = generateNormalized();
        if (rtn.isError())
            emit error(rtn.toString());
    }
    return normalImage;
} // getNormalImage()

QImage EigenFaceGenerator::getReconImage(EigenFaceTemplate * tpl, int vector, int layers)
{
    EigenFaceMaskedArray recon(data);
    foreach(EigenFaceVector vec, tpl->distance_efVector_mmap)
        if (vec.vectorKey() == vector && vec.isValid())
        {
            QImage reconImage;
            recon.reconstruct(vec, layers);
            reconImage = recon.toImage(QImage::Format_Indexed8, false);
            return reconImage;
        }
} // getReconImage()

QImage EigenFaceGenerator::graphVector(EigenFaceTemplate * tpl, int vector, QSize size, qreal scale)
{
    QImage graphImage;
    EigenFaceMaskedArray recon(data);
    foreach(EigenFaceVector vec, tpl->distance_efVector_mmap)
        if (vec.vectorKey() == vector && vec.isValid())
        {
        if (size.isEmpty())
            size = QSize(vec.count()+2, 101);
        if (qIsNull(scale))
            scale = size.height()-1;
        QImage graphImage(size, QImage::Format_Mono);
        graphImage.fill(Qt::color0);
        int x, y, t, b, m = size.height() / 2;
        for (y = 0; y < size.width()-2 && y < vec.count(); ++y)
        {
            qreal v = vec.coefficient(y);
            if (v < 0.0)
                t = m, b = qMin(m + (int)((-v) * scale + 0.5), size.height()-1);
            else
                b = m, t = qMax(m - (int)(v * scale + 0.5), 0);
            for (x = t; x <= b; ++x)
                graphImage.setPixel(y+1, x, Qt::color1);
        }
        return graphImage;
    }
    return QImage();
} // graphVector()
