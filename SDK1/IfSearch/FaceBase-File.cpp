#include <FaceBase.h>

#include <QCoreApplication>
#include <QDomDocument>
#include <QString>
#include <QTime>
#include <QWriteLocker>

#include <math.h>

#include <InfoMacros.h>
#include <Return.h>


#include <EigenFaceFace.h>
#include <EigenFaceTemplate.h>
#include <fbPerson.h>
//#include <ObjectDetector.h>
//#include <ObjectDetectorData.h>

const QString FaceBase::templateFilePrefix = tr("FT");
const QString FaceBase::templateFileSuffix("xml");
const QString FaceBase::imageFilePrefix("FI");
const QString FaceBase::imageFileSuffix("PNG");
const QString FaceBase::personFilePrefix("PR");
const QString FaceBase::personFileSuffix("xml");


#if 0
Return FaceBase::read(ObjectDetectorData * odd, const ObjectDetectorClass cls, QString name)
{
    if ( ! odd)
        return Return(ReturnStatusNull);

    QWriteLocker lock(odd->lock());
    odd->clear();

    if (QDir::root() != dataDir)
        return readFile(odd, cls, name);
    else
        return Return(ReturnStatusNull);
} // read(odd)

Return FaceBase::readFile(ObjectDetectorData * odd, const ObjectDetectorClass cls, QString name)
{
    QDomDocument doc;
    Return rtn;

    /*--- Open Detectors.xml description file ---*/
    QDir dir = dataDir;
    if ( ! dir.cd("Detectors"))
        return Return(ReturnChangeDir, "Detectors");
    QString detectorFileName = dir.filePath("Detectors.xml");
    QFile detectorFile(detectorFileName);
    rtn = DDTcore::readXmlFile(&doc, detectorFileName, "DDT-HaarDetectors");
    if (rtn.isError())
        return rtn;

    /*--- Find Document Element for specified DetectorClass ---*/
    QDomElement deRoot = doc.firstChildElement("HaarDetectors");
    if (deRoot.isNull())
        return Return(ReturnMissingXml, "HaarDetectors");
    QDomElement deClass = deRoot.firstChildElement("DetectorClass");
    while ( ! deClass.isNull())
    {
        if (deClass.attribute("Name") == cls.name())
            break;
        deClass = deClass.nextSiblingElement("DetectorClass");
    }
    if (deClass.isNull())
        return Return(ReturnMissingXml, "DetectorClass");

    /*--- Find specified (or default) detector ---*/
    if (name.isEmpty())
        name = deClass.attribute("Default");

    QDomElement deDetector = deClass.firstChildElement("Detector");
    while ( ! deDetector.isNull())
    {
        if (deDetector.attribute("Name") == name || name.isEmpty())
            break;
        deDetector = deDetector.nextSiblingElement("Detector");
    }
    if (deDetector.isNull())
        return Return(ReturnMissingXml, "Detector");

    QString sHeight = deDetector.attribute("Height");
    QString sWidth = deDetector.attribute("Width");
    QDomElement deDesc = deDetector.firstChildElement("Description");
    QDomElement deFile = deDetector.firstChildElement("XmlFile");
    QString fileName = dir.filePath(deFile.text().simplified());

    /*--- Set ObjectDetectorData info & read from xml file ---*/
    odd->setClassName(cls.name());
    odd->setDetectorName(deDetector.attribute("Name"));
    odd->setDetectorDescription(deDesc.text().simplified());
    odd->setDetectorSize(QSize(sWidth.toInt(), sHeight.toInt()));
    return odd->loadXmlFile(fileName);
} // readFile(odd)

Return FaceBase::readSql(ObjectDetectorData * odd, const ObjectDetectorClass cls, const QString name)
{
    return Return(Return::NotImplemented, "FaceBase::readSql(ObjectDetectorData)");
} // readSql(odd)
#endif	

Return FaceBase::initialize(const QString & initString, int maxLoad)
{
    Return rtn;

    clear();
    if (matcher) matcher->clear();
    baseDir = QDir(qApp->applicationDirPath());
    if ( ! baseDir.exists(initString))
    {
        if ( ! baseDir.mkpath(initString))
            return Return(ReturnCreateDir, initString);
        if ( ! baseDir.cd(initString))
            return Return(ReturnChangeDir, initString);

        // TODO: Other preparation work in a new base directory

        return Return(ReturnNewBaseDir, baseDir.absolutePath());
    }

    if ( ! baseDir.cd(initString))
        return Return(ReturnChangeDir, initString);
    int nFaces = 0, nDuplicate = 0, nNonFace = 0, nError = 0;
    QStringList dirFilter;
    dirFilter << templateFilePrefix + "???";
    QStringList tplDirList = baseDir.entryList(dirFilter,
                                               QDir::Dirs
                                               | QDir::NoDotAndDotDot,
                                               QDir::Name);
    int nTopDirs = tplDirList.size();
    int nDoneDirs = 0;
    // emit initProgress(nDoneDirs, nTopDirs);
    foreach (QString tplDirName, tplDirList)
    {
        QDir tplDir(baseDir);
        if ( ! tplDir.cd(tplDirName))
            return Return(ReturnChangeDir, tplDirName);

        QStringList nameFilter;
        nameFilter << tplDirName + "???." + templateFileSuffix;
        QStringList tplFileList = tplDir.entryList(nameFilter, QDir::Files, QDir::Name);
        foreach (QString tplFileName, tplFileList)
        {
            rtn = initFrom(tplDir.filePath(tplFileName));
            if (rtn.isWarn())
            {
                RETURN(rtn);
                ++nError;
            }
            else if (rtn.isFalse())
            {
                ++nNonFace;
            }
            else
            {
                ++nFaces;
            }
        }

        if (maxLoad && nFaces >= maxLoad)
        {
            //emit initProgress(nTopDirs, nTopDirs);
            break;
        }
        else
        {
            //emit initProgress(++nDoneDirs, nTopDirs);
        }
    }

    if (nError)
        return Return(ReturnInitWarning, people.size(), nFaces, nNonFace, nError);
    else
        return Return(ReturnInitialized, people.size(), nFaces, nNonFace);
}


Return FaceBase::initFrom(const QString & xmlFile)
{
    Return rtn;
    QDomDocument doc;

    EigenFaceFace face;
    rtn = face.readXmlFile(xmlFile);
    if (rtn.isWarn())
        return rtn;

    int personKey       = face.personKey();
    int faceKey		= face.faceKey();
    QString personId	= face.personId();
    QString faceId	= face.faceId();
    bool nonFace        = face.templat().isEmpty();

    if (faceKey)
    {
        peopleFaces[personKey].insertUnique(faceKey, faceId);
        mapPersonToFaces[personKey].append(faceKey);
        mapFaceToPerson.insert(faceKey, personKey);
        faceKeys.insert(faceKey);
        if (personKey && ! personId.isEmpty())
            people.insertUnique(personKey, personId);
        if (matcher && ! nonFace)
            matcher->enroll(face.templat(), faceKey, personKey);
        if (nonFace)    rtn = Return::False;
    }

    return rtn;
}

QString FaceBase::filePath(const QString & prefix, const int key, const int digits, const QString & suffix) const
{
    int div = (int)pow((qreal)10.0, digits/2);
    QDir dir(baseDir);
    QString dirName = QString("%1%2").arg(prefix).arg(key/div, digits/2, 10, QChar('0'));
    dir.mkpath(dirName);
    dir.cd(dirName);
    QString fileName = tr("%1%2.%3").arg(prefix).arg(key, digits, 10, QChar('0')).arg(suffix);
    return dir.exists() ? dir.filePath(fileName) : QString();
}
