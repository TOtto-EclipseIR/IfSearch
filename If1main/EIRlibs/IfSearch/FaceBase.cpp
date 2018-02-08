#include <FaceBase.h>

#include <QDomDocument>
#include <QString>
#include <QTime>
#include <QUrl>

#include <math.h>

#include <Return.h>

using namespace INDI;
#include <EigenFaceFace.h>
#include <EigenFaceTemplate.h>
#include <fbPerson.h>
		
		
const int FaceBase::faceKeyDigits = 6;
const int FaceBase::personKeyDigits = 6;

		
FaceBase::FaceBase(QObject * parent, EigenFaceSearcher * efm)
	: QObject(parent), matcher(efm)
{
	qsrand(QTime::currentTime().msec());

	baseDir = QDir::root();
	dataDir = QDir::root();

	DDT::Return::add(FaceBase::ReturnStatusNull, "INDI FaceBase Null Status", DDT::Warning);
	DDT::Return::add(FaceBase::ReturnMissingXml, "INDI FaceBase: Missing %1 xml element", DDT::Error);
	DDT::Return::add(FaceBase::ReturnWriteFile, "INDI FaceBase: Can't write file: %1", DDT::Error);
	DDT::Return::add(FaceBase::ReturnCreateDir, "INDI FaceBase: Can't create %1 directory", DDT::Error);
	DDT::Return::add(FaceBase::ReturnChangeDir, "INDI FaceBase: Can't change to %1 directory", DDT::Error);
	DDT::Return::add(FaceBase::ReturnNewBaseDir, "INDI FaceBase: New base directory at %1", DDT::Info);
	DDT::Return::add(FaceBase::ReturnBadKey, "INDI FaceBase: Bad %1 key for %2", DDT::Error);
	DDT::Return::add(FaceBase::ReturnMissingId, "INDI FaceBase: Missing %1 Id", DDT::Error);
	DDT::Return::add(FaceBase::ReturnInitWarning, "INDI FaceBase Initialized: %1 People, %2 Faces, %3 Non-searchable, %4 Errors", DDT::Warning);
	DDT::Return::add(FaceBase::ReturnInitialized, "INDI FaceBase Initialized: %1 People, %2 Faces, %3 Non-searchable", DDT::Info);
	DDT::Return::add(FaceBase::ReturnStatus, "People: %1 identified, %2 unidenfied; Faces: %3 Total, %4 Searchable", DDT::Info);
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

DDT::Return FaceBase::setDataUrl(const QUrl url)
{
	if ("dir" == url.scheme().toLower())
	{
		dataDir = QDir();
		if (dataDir.exists(url.path().mid(1)))
		{
			dataDir.cd(url.path().mid(1));
			return DDT::Return();
		}
		else
		{
			dataDir = QDir::root();
			return DDT::Return(ReturnChangeDir, url.path().mid(1));
		}
	}
	return DDT::Return(DDT::Return::NotImplemented, "setDataUrl not dir");
}
