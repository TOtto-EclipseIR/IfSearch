#include <EigenFaceFace.h>
#include <EigenFaceVector.h>

#include <QDomDocument>
#include <QFile>

#include <DDTcore.h>
#include <EigenFace.h>
#include <InfoMacros.h>
#include <Return.h>

using namespace INDI;


EigenFaceFace::EigenFaceFace(void)
{
	clear();
	DDT::Return::add(EigenFace::ReturnStatusNull, "INDI EigenFace Null Status", DDT::Warning);
}

EigenFaceFace::~EigenFaceFace()
{
}
		
bool EigenFaceFace::isValid(void) const
{
	return true;
}
		
void EigenFaceFace::clear(void)
{
	tpl.clear();
    Status = EigenFaceStatus();
	Reason.clear();
	FaceKey = 0;
	FaceId.clear();
	PersonKey = 0;
	PersonId.clear();
	ii.clear();
}
	
void EigenFaceFace::remove(void)
{
	tpl.clear();
    Status = EigenFaceStatus::Deleted;
}

DDT::Return EigenFaceFace::read(const QDomDocument & doc)
{
	DDT::Return rtn;

//	clear();
	QDomElement root = doc.documentElement();
	QDomElement id = root.firstChildElement("Identification");
	QDomElement info = root.firstChildElement("ImageInfo");
	QDomElement de = root.firstChildElement(EigenFaceTemplate::eigenFaceTemplateTagName);

	QString sStatus = root.attribute("Status");
    if (sStatus == "Removed") sStatus = "Deleted";
	EigenFaceStatus sts = EigenFaceStatus::fromName(sStatus);
    Status = sts.isValid() ? sts : EigenFaceStatus::Error;
	Reason = root.attribute("Reason");

	PersonKey = id.attribute("PersonKey").toInt();
	FaceKey = id.attribute("FaceKey").toInt();
	PersonId = id.attribute("PersonId");
	FaceId = id.attribute("FaceId");

	ii.read(info);
	return tpl.read(de);
}

DDT::Return EigenFaceFace::write(QDomDocument * doc) const
{
	doc->clear();
	QDomElement root = doc->createElement("INDIfaceFace");
	if ( ! Status.isNull())
		root.setAttribute("Status", Status.name());
	if ( ! Reason.isEmpty())
		root.setAttribute("Reason", Reason);
	
	QDomElement id = doc->createElement("Identification");
	if (FaceKey)	
		id.setAttribute("FaceKey", FaceKey);
	if ( ! FaceId.isEmpty())
		id.setAttribute("FaceId", FaceId);
	if (PersonKey)	
		id.setAttribute("PersonKey", PersonKey);
	if ( ! PersonId.isEmpty())	
		id.setAttribute("PersonId",	PersonId);
	root.appendChild(id);

	QDomElement info = doc->createElement("ImageInfo");
	ii.writeDomElement(&info);
	root.appendChild(info);

	QDomElement deTpl = doc->createElement(EigenFaceTemplate::eigenFaceTemplateTagName);
	tpl.write(&deTpl);
	root.appendChild(deTpl);

	doc->appendChild(root);
	return DDT::Return();
}

DDT::Return EigenFaceFace::writeXmlFile(const QString & fileName) const
{
	QDomDocument doc;
	DDT::Return rtn = write(&doc);
	if (rtn.isError())
		return rtn;
	else
		TRACE("Face written: %1", fileName);

	return DDTcore::writeXmlFile(fileName, doc);
}

DDT::Return EigenFaceFace::readXmlFile(const QString & fileName)
{
	QDomDocument doc;
	DDT::Return rtn = DDTcore::readXmlFile(&doc, fileName);
	if (rtn.isWarn())
		return rtn;

	return read(doc);
}
