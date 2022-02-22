#include <HaarDetector.h>

#include <QtGlobal>
#include <QColor>
#include <QCoreApplication>
#include <QFileInfo>
#include <QDomDocument>
#include <QRect>
#include <QSettings>
#include <QElapsedTimer>

#include <opencv2/core/core_c.h>

#include <GreyImage.h>
#include <InfoMacros.h>
#include <ImageMarker.h>


HaarDetector::HaarDetector(const QString & classname,
                           const qreal classfactor,
                           QObject * parent)
    : QObject(parent)
    , pCascade(0)
    , ClassName(classname)
    , ClassFactor(classfactor)
    , cache(0)
    , settings(0)
{
    if ( ! docDetectors.isNull())
        deDetectors = classDetectors(docDetectors);
    if (deDetectors.isNull())
    {
        settings = new QSettings("DDzTech", "INDI", this);
        settings->beginGroup("Detectors");
        settings->beginGroup(ClassName);
        settings->sync();
    }
    MaxResults = 0;
    MinQuality = 0;
    Adjust = 0;
    Factor = 0.0;
    MaxDensity = 2.0;
    MaxDetectors = 16;
    Scale = 0.0;
    Flags = 0; // CV_HAAR_DO_CANNY_PRUNING;
    MaxAcross = 0;
    MinAcross = 0;
    MaxDimension = 0;
    MaxPixels = 0;
    MinPixels = 0;
    GroupMethod = 0;
    DefaultGroupMethod = 4; // WANTDO(zero)
    OverlapThreshold = 0;
    GroupThreshold = 0;
    NeighborThreshold = 0;
    MarkAll = false;
} // c'tor

HaarDetector::~HaarDetector()
{
#ifdef USE_OCV4
        // MUSTDO release
#endif
} // d'tor

bool HaarDetector::hasDetector(void)
{
    return ! cvCascade_.empty();
}


QDomElement HaarDetector::classDetectors(const QDomDocument & doc)
{
    QDomElement deRoot = doc.firstChildElement("HaarDetectors");
    if (deRoot.isNull())
        return QDomElement();

    QDomElement de = deRoot.firstChildElement("DetectorClass");
    while ( ! de.isNull())
    {
        if (de.attribute("Name") == ClassName)
            return de;
        de = de.nextSiblingElement("DetectorClass");
    }

    return QDomElement();
}

QStringList HaarDetector::detectorsAvailable(void)
{
    if (settings)
        return settings->childGroups();
    else if ( ! deDetectors.isNull())
    {
        QStringList qsl;
        QDomElement de = deDetectors.firstChildElement("Detector");
        while ( ! de.isNull())
        {
            qsl << de.attribute("Name");
            de = de.nextSiblingElement("Detector");
        }
        return qsl;
    }
    else
        return QStringList();
} // detectorsAvailable()

QString HaarDetector::detectorsDefault(void)
{
    if (settings)
        return settings->value("DefaultName").toString();
    else if ( ! deDetectors.isNull())
        return deDetectors.attribute("Default");
    else
        return QString();
} // detectorsAvailable()

void HaarDetector::clear(void)
{
    origScale = 0;
    imgOrig = QImage();
    scaled_size = QSize();
    allObjects.clear();
    allResults.clear();
    results.clear();
    cvMat_Grey.release();
} // clear()

void HaarDetector::setImage(QImage img)
{
        const int maxDim = maxDimension() ? maxDimension() : 1536;
        clear();
        if (QImage::Format_ARGB32_Premultiplied == imgOrig.format())
            imgOrig = img;
        else
            imgOrig = imgOrig.convertToFormat(QImage::Format_ARGB32_Premultiplied);
        origScale = 1;
        if (img.width() > maxDim || img.height() > maxDim)
            origScale = 1 + qMax(img.width(), img.height()) / maxDim;
        unsigned origWidth = imgOrig.width() & 0xFFFC, origHeight = imgOrig.height() & 0xFFFC;
        scaled_size = QSize(origWidth / origScale, origHeight / origScale);
        if (origWidth <= 0 && origHeight <= 0)
        {
            cvMat_Grey.release();
            emit error("Null image for detector");
            return;                                                     /* /====\ */
        }
        cvMat_Grey.create((origHeight / origScale) & 0xFFFC,
                          (origWidth / origScale) & 0xFFFC, CV_8U);
        DETAIL("Created Greyscale/%1 cvMat", origScale);
        Info::flush();

        if (QImage::Format_Indexed8 == imgOrig.format())
        {
            for (unsigned origRow = 0, matRow = 0;
                 origRow < origHeight;
                 origRow += origScale, ++matRow)
            {
                quint8 * inGrey = (quint8 *)imgOrig.scanLine(origRow);
                unsigned char * outGrey = (unsigned char *)cvMat_Grey.ptr(origRow);
                if (1 == origScale)
                {
                    memcpy(outGrey, inGrey, origWidth);
                }
                else
                {
                    for (unsigned origCol = 0; origCol < origWidth; origCol += origScale)
                        *outGrey++ = *(inGrey + origCol);
                }
            } // for(r)
            DETAIL("Copied Grey Image Data");
            Info::flush();
        }
        else // Format_Indexed8
        {
            for (unsigned origRow = 0; origRow < origHeight; origRow += origScale)
            {
                QRgb * inRgb = (QRgb *)imgOrig.scanLine(origRow);
                unsigned char * outGrey = (unsigned char *)(cvMat_Grey.ptr());
                for (unsigned origCol = 0; origCol < origWidth; origCol += origScale)
                    *outGrey++ = qGray(*(inRgb + origCol));
            } // for(origRow)
            DETAIL("Converted to Grey Image Data");
            Info::flush();
        }
        DETAIL("cvMat %1x%2 filled with greyscale data", cvMat_Grey.rows, cvMat_Grey.cols);
} // setImage(QImage)

QImage HaarDetector::image(void) const
{
    QImage result;
    if (0 == cvMat_Grey.total())
        return result;                                                  /* /====\ */
    result = QImage(QSize(cvMat_Grey.cols, cvMat_Grey.rows), QImage::Format_Indexed8);
    result.setColorTable(GreyImage::greyColorTable());
    if (signed(cvMat_Grey.step) == cvMat_Grey.cols)
    {
        memcpy(result.bits(),
               cvMat_Grey.ptr<unsigned char>(0),
               result.width() * result.height());
    }
    else
        for (int r = 0; r < cvMat_Grey.cols; ++r)
        {
            unsigned char * pInput = (unsigned char *)(cvMat_Grey.ptr(r));
            unsigned char * pOutput = result.scanLine(r);
            memcpy(pOutput, pInput, result.width());
        }
    return result;
}


void HaarDetector::selectDetector(const QString & name)
{
    DetectorName = name;
    DetectorDescription.clear();
    DetectorFileName.clear();
    DetectorSize = QSize();

    if (settings)
    {
        DetectorDescription = settings->value(name + "/Description", QString()).toString();
        DetectorFileName = settings->value(name + "/XmlFile", QString()).toString();
        DetectorSize = settings->value(name + "/Size", QSize()).toSize();
    }
    else if ( ! deDetectors.isNull())
    {
        QDomElement deDetector;
        QDomElement de = deDetectors.firstChildElement("Detector");
        while ( ! de.isNull())
        {
            if (de.attribute("Name") == DetectorName)
            {
                deDetector = de;
                break;
            }
            de = de.nextSiblingElement("Detector");
        }
        if ( ! deDetector.isNull())
        {
            QString sHeight = deDetector.attribute("Height");
            QString sWidth = deDetector.attribute("Width");
            QDomElement deDesc = deDetector.firstChildElement("Description");
            DetectorDescription = deDesc.text().simplified();
            QDomElement deFile = deDetector.firstChildElement("XmlFile");
            DetectorFileName = dirDetectors.filePath(deFile.text().simplified());
            DetectorSize = QSize(sWidth.toInt(), sHeight.toInt());
        }
    }
    loadXmlCascade(DetectorFileName);
} // selectDetector()

void HaarDetector::selectXml(const QString & fName)
{
    QFileInfo fi(fName);
    DetectorName = fi.completeBaseName();
    DetectorDescription = fName;
    DetectorFileName = fName;
    DetectorSize = sizeFromXml(fName);
    loadXmlCascade(DetectorFileName);
} // selectXml()

bool HaarDetector::loadXmlCascade(const QString & xmlFilename)
{
    DetectorFileName = xmlFilename;
    cvCascade_.load(DetectorFileName.toStdString());
    if ( ! hasDetector())
    {
        ERRMSG("Unable to load cascade at %1", DetectorFileName);
        emit error("Unable to load cascade at " + DetectorFileName);
        DetectorName.clear();
        DetectorDescription.clear();
        DetectorSize = QSize();
    }
    return hasDetector();
}

qreal HaarDetector::scaleFactor(void) const
{
    if (scale() > 1.0)
        return scale();

    qreal result = 1.1;
    qreal minWidth = minObjectSize().width();
    qreal maxWidth = maxObjectSize().width();
    if ( ! qIsNull(factor()))
        result = 1.0 + factor() / 100.0;
    if ( ! qIsNull(maxDensity()))
        result = qMax(result, 1.0 + (maxDensity() / minWidth));
    if (maxDetectors() > 3)
        result = qMax(result, exp(log(maxWidth / minWidth) / (qreal)maxDetectors()));
    return result;
} // scaleFactor()

int HaarDetector::groupFactor(void) const
{
    qreal result_f = scaleFactor() - 1.0;
    int result_i = qRound(20.0 * sqrt(result_f));
    return qMax(1, result_i);
} // groupFactor()

bool HaarDetector::process(bool returnAll)
{
    DETAIL("Detecting Objects");
    Info::flush();
    return processCascadeClassifier(returnAll);
}

bool HaarDetector::processCascadeClassifier(bool returnAll)
{
    Q_UNUSED(returnAll); // NEEDDO(use)?
    QElapsedTimer * timer = new QElapsedTimer; NULLPTR(timer);

    allObjects.clear();
    allResults.clear();
    results.clear();
    msecProcess = 0;

    if ( ! hasDetector())
    {
        emit error(tr("No QFF classifier"));
        return false;                                                   /* /====\ */
    }
    if (0 == cvMat_Grey.total())
    {
        emit error(tr("No QFF image"));
        return false;                                                   /* /====\ */
    }
    timer->start();

    // Detected Faces
    DETAIL("Start ClassifierCascade interface");
    Info::flush();
    QSize sz = minObjectSize();
    QSize max = maxObjectSize();
    std::vector<cv::Rect> cvRects;
    cvCascade_.detectMultiScale(cvMat_Grey,
                                cvRects,
                                scaleFactor(),
                                GroupThreshold,
                                flags(),
                                cvSize(sz.width(),  sz.height()),
                                cvSize(max.width(), max.height()));
    DETAIL("%1 raw objects detected", unsigned(cvRects.size()));

    QRect imageRect = imgOrig.rect();
    for (unsigned i = 0; i < cvRects.size(); i++)
    {
        cv::Rect cvr = cvRects[i];
        QRect r(QPoint(origScale * cvr.x, origScale * cvr.y),
                QSize(origScale * cvr.width, origScale * cvr.height));
        if ( ! r.isValid()) // TODO: Why 0xBAADF00D?
            continue;
        if ( ! imageRect.contains(r))
            continue;

        if (GroupInternal == GroupMethod
                || GroupInternalAllObjects == GroupMethod)
        {
            DetectorResult ri(r);
            allResults.insert(2.0 * groupFactor()
                              * (double)GroupThreshold, ri);
        }
        else
        {
            allObjects << r;
        }
    }
    DETAIL("%1 objects returned", allObjects.size());
    msecProcess = timer->elapsed();
    delete timer;
    timer = 0;
    if (GroupInternalAllObjects == GroupMethod)
    {
        DETAIL("Detecting additional raw detectors");
        cvCascade_.detectMultiScale(cvMat_Grey,
                                    cvRects,
                                    scaleFactor(),
                                    1,
                                    flags(),
                                    cvSize(sz.width(),  sz.height()),
                                    cvSize(max.width(), max.height()));
        for (unsigned i = 0; i < cvRects.size(); i++)
        {
            cv::Rect cvr = cvRects[i];
            QRect r(QPoint(origScale * cvr.x, origScale * cvr.y),
                    QSize(origScale * cvr.width, origScale * cvr.height));
            if (r.isValid()) // TODO: Why 0xBAADF00D?
                allObjects << r;
        }
    }

    return true;
} // process()

QSize HaarDetector::sizeFromXml(const QString & fileName)
{
    QSize sz;
    QString errMsg;
    int errLine, errCol;
    QDomDocument	xml;
    QFile f(fileName);
    f.open(QIODevice::ReadOnly);
    if ( ! xml.setContent(&f, &errMsg, &errLine, &errCol))
    {
        return QSize();
    }

    QDomElement doc = xml.documentElement();
    QDomElement outer = doc.firstChildElement();
    QDomElement domSz = outer.firstChildElement("size");
    QString strSz = domSz.text();
    strSz = strSz.simplified();
    QStringList qsl = strSz.split(" ");
    sz.setWidth(qsl[0].toInt());
    sz.setHeight(qsl[1].toInt());

    return sz;
} // sizeFromXml()

QSize HaarDetector::minObjectSize(void) const
{
    QSize sz = DetectorSize;
    if (MaxAcross)
        sz *= (qreal)scaled_size.width()
                / (qreal)MaxAcross
                / (qreal)sz.width()
                * ClassFactor;
    if (MinPixels)
        if (sz.width() > MinPixels && DetectorSize.width() < MinPixels)
            sz = DetectorSize * ((qreal)MinPixels / (qreal)DetectorSize.width());
    return sz;
}

QSize HaarDetector::maxObjectSize(void) const
{
    QSize sz = DetectorSize;
    qreal s1 = qMin((qreal)scaled_size.height() / (qreal)sz.height() / ClassFactor,
                    (qreal)scaled_size.width()  / (qreal)sz.width()  / ClassFactor);
    qreal s2 = s1;

    if (MinAcross)
        s1 = qMax(1.0, (qreal)scaled_size.width() / (qreal)MinAcross / (qreal)sz.width() * ClassFactor);
    if (MaxPixels)
        s2 = qMax(1.0, (qreal)MaxPixels / (qreal)sz.width());
    sz *= qMin(s1, s2);
    return sz.boundedTo(scaled_size);
}

qreal HaarDetector::minAspect(void) const
{
    qreal a = DetectorSize.height() / DetectorSize.width();
    return a * 4.0 / 5.0;
}

qreal HaarDetector::maxAspect(void) const
{
    qreal a = DetectorSize.height() / DetectorSize.width();
    return a * 5.0 / 4.0;
}

QList<QSize> HaarDetector::detectorSizes(void) const
{
    QList<QSize> rtn;
    QSize szMax = maxObjectSize();
    QSize szMin = minObjectSize();
    qreal f = scaleFactor();
    qreal w = DetectorSize.width();
    qreal h = DetectorSize.height();
    if (szMax.isEmpty() || szMax.isEmpty() || DetectorSize.isEmpty() || f < 1.0001)
        return rtn;

    while (w < szMin.width() && h < szMin.height())
        w *= f, h *= f;
    while (w < szMax.width() && h < szMax.height())
    {
        rtn << QSize(w, h);
        w *= f, h *= f;
    }
    return rtn;
}

QString HaarDetector::methodString(void) const
{
    QString rtn = tr(";ScaleFactor=%1").arg(scaleFactor(), 5, 'f', 3);

    if (qIsNull(scale()))
    {
        if ( ! qIsNull(factor()))
            rtn += tr(";Factor=%1").arg(factor(), 5, 'f', 3);
        if ( ! qIsNull(maxDensity()))
            rtn += tr(";MaxDensity=%1").arg(maxDensity(), 5, 'f', 3);
        if (maxDetectors())
            rtn += tr(";MaxDetectors=%1").arg(maxDetectors());
    }
    if (MinAcross)
        rtn += tr(";MinAcross=%1").arg(MinAcross);
    if (MaxAcross)
        rtn += tr(";MaxAcross=%1").arg(MaxAcross);
    if (MinPixels)
        rtn += tr(";MinPixels=%1").arg(MinPixels);
    if (MaxPixels)
        rtn += tr(";MaxPixels=%1").arg(MaxPixels);
    if (GroupMethod ? GroupMethod : DefaultGroupMethod)
        rtn += tr(";GroupMethod=%1").arg(GroupMethod ? GroupMethod : DefaultGroupMethod);
    rtn += ";INDI-" + ClassName + "-" + this->DetectorName;
    if (GroupThreshold)
        rtn += tr(";GroupThreshold=%1").arg(GroupThreshold);
    if (OverlapThreshold)
        rtn += tr(";OverlapThreshold=%1").arg(OverlapThreshold);
    if (NeighborThreshold)
        rtn += tr(";NeighborThreshold=%1").arg(NeighborThreshold);
    if (Adjust)
        rtn += tr(";Adjust=%1").arg(Adjust);

    return rtn;
} // methodString()

QString HaarDetector::performanceString(void) const
{
    QSize sz = imgOrig.size();
    qreal MPix = (qreal)sz.width() / 1024.0 * (qreal)sz.height() / 1024.0;
    return tr("%1 %4 found; %2 msec; %3 msec/MPix", "PerformanceString")
            .arg(results.size())
            .arg(msecProcess)
            .arg((int)((qreal)msecProcess / MPix))
            .arg(ClassName);
} // performanceString()

QImage HaarDetector::detectImage(void)
{
    QList<QRect> allDetectors = allObjects;
    QList<DetectorResult> allGrouped;
    if (markAll())
    {
        groupObjects(true);
        allGrouped = results;
        groupObjects(false);
    }
    else
        allGrouped = results;

    QImage resultImage(image().convertToFormat(QImage::Format_RGB32));
    QColor allColor(markExtraColor().isEmpty() ? "cyan" : markExtraColor());
    QColor scaleColor(markScaleColor().isEmpty() ? "darkred" : markScaleColor());
    QFont font("Helvetica", 10);
    allColor.setAlpha(32);
    QList<QColor> detectColorList = markColorList();
    ImageMarker marker(&resultImage);
    marker.title(methodString());
    marker.detectorScale(detectorSize(), detectorSizes(), scaleColor);

    foreach (DetectorResult res, allGrouped)
        foreach (QRect r, res.allRectangles())
            allDetectors.removeOne(r);
    if (1 == origScale)
        marker.rects(allDetectors, allColor);
    else
    {
        foreach (QRect r, allDetectors)
        {
            QRect sr(QPoint(r.left() / origScale, r.top() / origScale),
                     QSize(r.width() / origScale, r.height() / origScale));
            marker.rect(sr, allColor);
        }
    }

    int k = allGrouped.size();
    while ( ! allGrouped.isEmpty())
    {
        DetectorResult result = allGrouped.takeLast();
        QColor foundColor = detectColorList.at(--k % detectColorList.size());
        if (result.score() < minQuality())
            foundColor = foundColor.darker(200 + (minQuality() - result.score()) / 10);
        else
            foundColor = foundColor.lighter(50 + result.score() / 10);
        foundColor.setAlpha(128);
        if (1 == origScale)
            marker.rects(result.allRectangles(), foundColor, 0);
        else
        {
            foreach (QRect r, result.allRectangles())
            {
                QRect sr(QPoint(r.left() / origScale, r.top() / origScale),
                         QSize(r.width() / origScale, r.height() / origScale));
                marker.rect(sr, foundColor, 0);
            }
        }
        QRect r(result.rectangle());
        QRect sr(QPoint(r.left() / origScale, r.top() / origScale),
                 QSize(r.width() / origScale, r.height() / origScale));
        foundColor.setAlpha(192);
        marker.rect(sr, foundColor, 5);
        QString detectString = QString("Q%1 %3 W%2")
                .arg(result.score(), 3, 10, QChar('0'))
                .arg(result.rectangle().width())
                .arg(k+1, 2);
        marker.title(sr, detectString, Qt::white,
                     foundColor, font, Qt::AlignLeft);
    }

    marker.end();
    return resultImage;
} // detectImage()

QList<QColor> HaarDetector::markColorList(void) const
{
    QList<QColor> resultList;
    if (MarkColors.isEmpty())
#ifdef USE_OCV4
        resultList << QColor(0xFF00F0) << QColor(0xF000FF);
#endif
    else
        foreach (QString s, markColors().simplified().split(' '))
            resultList << QColor(s);
    return resultList;
} // markColorList()

void HaarDetector::handleResults(bool returnAll)
{
    // Deal with constraint properties
    DETAIL("Handle Detector Results");
    results.clear();
    int k = 0;
    QMultiMap<double, DetectorResult>::const_iterator citr1;
    citr1 = allResults.end();
    while (citr1 != allResults.begin())
    {
        --citr1;
        int scaled = scaleScore(citr1.key());
        DetectorResult ri = citr1.value();
        if (MinQuality && scaled < MinQuality && ! returnAll)
            break;
        if (MaxResults && results.size() >= MaxResults && ! returnAll)
            break;
        if (ri.allRectangles().size() < 2)
            continue;
        if (Adjust)
            ri.rect = doAdjust(ri.rect);
        ri.k = ++k;
        ri.sco = scaled;
        results.insert(scaled, ri);
    }
    DETAIL("%1 results handled", results.size());
} // handleResults()


/**** Statics ****/
QDomDocument HaarDetector::docDetectors;
QDir HaarDetector::dirDetectors;

VersionInfo HaarDetector::cvVersion(void)
{
    VersionInfo ver(CV_MAJOR_VERSION,
                    CV_MINOR_VERSION,
                    CV_SUBMINOR_VERSION,
                    0,
                    CV_VERSION,
                    "Copyright (c) 2000-2008, Intel Corporation. "
                    "(c) 2008-2010, Willow Garage Inc.",
                    "Intel",
                    "Computer Vision Library");
    return ver;
}

Return HaarDetector::initialize(QFile * xmlFile)
{
    QString errMsg;
    int errLine, errCol;

    if ( ! xmlFile->open(QIODevice::ReadOnly))
        return Return::qfileError(*xmlFile);
    if ( ! docDetectors.setContent(xmlFile, &errMsg, &errLine, &errCol))
        return Return(Return::ReturnXmlParse, xmlFile->fileName(), errMsg, errLine, errCol);

    QFileInfo fi(xmlFile->fileName());
    dirDetectors = fi.dir();

    return Return();
}
