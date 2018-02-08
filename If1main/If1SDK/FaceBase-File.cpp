#include <FaceBase.h>

#include <QApplication>
#include <QDomDocument>
#include <QString>
#include <QTime>
#include <QWriteLocker>

#include <math.h>

#include <Return.h>

using namespace INDI;
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
DDT::Return FaceBase::read(ObjectDetectorData * odd, const ObjectDetectorClass cls, QString name)
{
    if ( ! odd)
        return DDT::Return(ReturnStatusNull);

    QWriteLocker lock(odd->lock());
    odd->clear();

    if (QDir::root() != dataDir)
        return readFile(odd, cls, name);
    else
        return DDT::Return(ReturnStatusNull);
} // read(odd)

DDT::Return FaceBase::readFile(ObjectDetectorData * odd, const ObjectDetectorClass cls, QString name)
{
    QDomDocument doc;
    DDT::Return rtn;

    /*--- Open Detectors.xml description file ---*/
    QDir dir = dataDir;
    if ( ! dir.cd("Detectors"))
        return DDT::Return(ReturnChangeDir, "Detectors");
    QString detectorFileName = dir.filePath("Detectors.xml");
    QFile detectorFile(detectorFileName);
    rtn = DDTcore::readXmlFile(&doc, detectorFileName, "DDT-HaarDetectors");
    if (rtn.isError())
        return rtn;

    /*--- Find Document Element for specified DetectorClass ---*/
    QDomElement deRoot = doc.firstChildElement("HaarDetectors");
    if (deRoot.isNull())
        return DDT::Return(ReturnMissingXml, "HaarDetectors");
    QDomElement deClass = deRoot.firstChildElement("DetectorClass");
    while ( ! deClass.isNull())
    {
        if (deClass.attribute("Name") == cls.name())
            break;
        deClass = deClass.nextSiblingElement("DetectorClass");
    }
    if (deClass.isNull())
        return DDT::Return(ReturnMissingXml, "DetectorClass");

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
        return DDT::Return(ReturnMissingXml, "Detector");

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

DDT::Return FaceBase::readSql(ObjectDetectorData * odd, const ObjectDetectorClass cls, const QString name)
{
    return DDT::Return(DDT::Return::NotImplemented, "FaceBase::readSql(ObjectDetectorData)");
} // readSql(odd)
#endif	

DDT::Return FaceBase::initialize(const QString & initString, int maxLoad)
{
    DDT::Return rtn;

    clear();
    baseDir = QDir(qApp->applicationDirPath());
    if ( ! baseDir.exists(initString))
    {
        if ( ! baseDir.mkpath(initString))
            return DDT::Return(ReturnCreateDir, initString);
        if ( ! baseDir.cd(initString))
            return DDT::Return(ReturnChangeDir, initString);

        // TODO: Other preparation work in a new base directory

        return DDT::Return(ReturnNewBaseDir, baseDir.absolutePath());
    }

    if ( ! baseDir.cd(initString))
        return DDT::Return(ReturnChangeDir, initString);
    int nFaces = 0, nDuplicate = 0, nNonFace = 0, nError = 0;
    QStringList dirFilter;
    dirFilter << templateFilePrefix + "???";
    QStringList tplDirList = baseDir.entryList(dirFilter, QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name);
    int nTopDirs = tplDirList.size();
    int nDoneDirs = 0;
    emit initProgress(nDoneDirs, nTopDirs);
    foreach (QString tplDirName, tplDirList)
    {
        QDir tplDir(baseDir);
        if ( ! tplDir.cd(tplDirName))
            return DDT::Return(ReturnChangeDir, tplDirName);

        QStringList nameFilter;
        nameFilter << tplDirName + "???." + templateFileSuffix;
        QStringList tplFileList = tplDir.entryList(nameFilter, QDir::Files, QDir::Name);
        foreach (QString tplFileName, tplFileList)
        {
            rtn = initFrom(tplDir.filePath(tplFileName));
            if (rtn.isNull())
                ++nNonFace;
            else if (rtn.is(EigenFace::ReturnAlreadyEnrolled))
                ++nDuplicate;
            else if (rtn.is(EigenFace::ReturnEnrolled))
                ++nFaces;
            else if (rtn.isWarn())
                ++nError;
        }

        if (maxLoad && nFaces >= maxLoad)
        {
            emit initProgress(nTopDirs, nTopDirs);
            break;
        }
        else
            emit initProgress(++nDoneDirs, nTopDirs);
    }

    if (nError)
        return DDT::Return(ReturnInitWarning, people.size(), nFaces, nNonFace, nError);
    else
        return DDT::Return(ReturnInitialized, people.size(), nFaces, nNonFace);
}


DDT::Return FaceBase::initFrom(const QString & xmlFile)
{
    DDT::Return rtn;
    QDomDocument doc;

    INDI::EigenFaceFace face;
    rtn = face.readXmlFile(xmlFile);
    if (rtn.isWarn())
        return rtn;

    int personKey		= face.personKey();
    int faceKey			= face.faceKey();
    QString personId	= face.personId();
    QString faceId		= face.faceId();

    if (faceKey)
    {
        peopleFaces[personKey].insertUnique(faceKey, faceId);
        mapPersonToFaces[personKey].append(faceKey);
        mapFaceToPerson.insert(faceKey, personKey);
        faceKeys.insert(faceKey);
        if (personKey && ! personId.isEmpty())
            people.insertUnique(personKey, personId);
        if (matcher && ! face.templat().isEmpty())
        {
             QString enrollError = matcher->enroll(face.templat(), faceKey, personKey);
             if ( ! enrollError.isEmpty())
                 rtn = DDT::Return::errorString(enrollError);
        }
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
