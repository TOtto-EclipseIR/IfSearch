#include <FaceBase.h>

#include <QDomDocument>
#include <QString>
#include <QTime>
#include <QUrl>

#include <math.h>

#include <Return.h>


#include <EigenFaceFace.h>
#include <EigenFaceTemplate.h>
#include <fbPerson.h>


const int FaceBase::faceKeyDigits = 6;
const int FaceBase::personKeyDigits = 6;


FaceBase::FaceBase(QObject * parent, EigenFaceSearcher * efm)
    : QObject(parent), matcher(efm)
{
    srand(QTime::currentTime().msec());

    baseDir = QDir::root();
    dataDir = QDir::root();

    Return::add(FaceBase::ReturnStatusNull, "INDI FaceBase Null Status", Warning);
    Return::add(FaceBase::ReturnMissingXml, "INDI FaceBase: Missing %1 xml element", Error);
    Return::add(FaceBase::ReturnWriteFile, "INDI FaceBase: Can't write file: %1", Error);
    Return::add(FaceBase::ReturnCreateDir, "INDI FaceBase: Can't create %1 directory", Error);
    Return::add(FaceBase::ReturnChangeDir, "INDI FaceBase: Can't change to %1 directory", Error);
    Return::add(FaceBase::ReturnNewBaseDir, "INDI FaceBase: New base directory at %1", Info);
    Return::add(FaceBase::ReturnBadKey, "INDI FaceBase: Bad %1 key for %2", Error);
    Return::add(FaceBase::ReturnMissingId, "INDI FaceBase: Missing %1 Id", Error);
    Return::add(FaceBase::ReturnInitWarning, "INDI FaceBase Initialized: %1 People, %2 Faces, %3 Non-searchable, %4 Errors", Warning);
    Return::add(FaceBase::ReturnInitialized, "INDI FaceBase Initialized: %1 People, %2 Faces, %3 Non-searchable", Info);
    Return::add(FaceBase::ReturnStatus, "People: %1 identified, %2 unidenfied; Faces: %3 Total, %4 Searchable", Info);
} // c'tor

FaceBase::~FaceBase()
{
}

void FaceBase::clear(void)
{
    people.clear();
    peopleFaces.clear();
    mapPersonToFaces.clear();
    mapFaceToPerson.clear();
    faceKeys.clear();
}

Return FaceBase::setDataUrl(const QUrl url)
{
    if ("dir" == url.scheme().toLower())
    {
        dataDir = QDir();
        if (dataDir.exists(url.path().mid(1)))
        {
            dataDir.cd(url.path().mid(1));
            return Return();
        }
        else
        {
            dataDir = QDir::root();
            return Return(ReturnChangeDir, url.path().mid(1));
        }
    }
    return Return(Return::NotImplemented, "setDataUrl not dir");
}
