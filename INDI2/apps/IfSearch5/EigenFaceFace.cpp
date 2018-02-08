#include <EigenFaceFace.h>
#include <EigenFaceVector.h>

#include <QDomDocument>
#include <QFile>

#include <DDTcore.h>
#include <EigenFace.h>
#include <InfoMacros.h>
#include <Return.h>




EigenFaceFace::EigenFaceFace(void)
{
    clear();
    Return::add(EigenFace::ReturnStatusNull, "INDI EigenFace Null Status", Warning);
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

Return EigenFaceFace::read(const QDomDocument & doc)
{
    Return rtn;

    QDomElement root = doc.documentElement();
    QDomElement id = root.firstChildElement("Identification");
    QDomElement info = root.firstChildElement("ImageInfo");
    QDomElement de = root.firstChildElement(EigenFaceTemplate::eigenFaceTemplateTagName);

    QString sStatus = root.attribute("Status");
    if (sStatus == "Removed") sStatus = "Deleted";
    EigenFaceStatus sts = EigenFaceStatus(sStatus);
    Status = sts.isValid() ? sts : EigenFaceStatus::Error;
    Reason = root.attribute("Reason");

    PersonKey = id.attribute("PersonKey").toInt();
    FaceKey = id.attribute("FaceKey").toInt();
    PersonId = id.attribute("PersonId");
    FaceId = id.attribute("FaceId");

    ii.read(info);
    return tpl.read(de);
}

Return EigenFaceFace::write(QDomDocument * doc) const
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
    return Return();
}

Return EigenFaceFace::writeXmlFile(const QString & fileName) const
{
    QDomDocument doc;
    Return rtn = write(&doc);

    if (rtn.isError())
        return rtn;
    else
        TRACE("Face written: %1 Status=%2", fileName, Status.name());

    return DDTcore::writeXmlFile(fileName, doc);
}

Return EigenFaceFace::readXmlFile(const QString & fileName)
{
    QDomDocument doc;
    Return rtn = DDTcore::readXmlFile(&doc, fileName);
    if (rtn.isWarn())
        return rtn;

    return read(doc);
}
