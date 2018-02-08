#include <fbPerson.h>

#include <FaceBase.h>
#include <Return.h>
#include <InfoMacros.h>

#include <QDomDocument>
#include <QDomElement>
#include <QFile>

using namespace INDI;

DDT::Return fbPerson::readXmlFile(const QString & fileName)
{
	DDT::Return rtn;
	QString errMsg;
	int errLine, errCol;
	QDomDocument doc;

	QFile xmlFile(fileName);
	if ( ! xmlFile.open(QIODevice::ReadOnly))
		return DDT::Return::qfileError(xmlFile);
	if ( ! doc.setContent(&xmlFile, &errMsg, &errLine, &errCol))
		return DDT::Return(DDT::Return::ReturnXmlParse, xmlFile.fileName(), errMsg, errLine, errCol);

	QDomDocumentType dt = doc.doctype();
	if ("INDIface-Person" != dt.name())
		return DDT::Return(DDT::Return::ReturnXmlParse, xmlFile.fileName(), "Invalid DocType", 0, 0);

	QDomElement root = doc.documentElement();
	if ("PersonRecord" != root.tagName())
		return DDT::Return(DDT::Return::ReturnXmlParse, xmlFile.fileName(), "Invalid root tag name", 0, 0);
	Key = root.attribute("PersonKey").toInt();
	Active = !! root.attribute("Active", "1").toInt();

	QDomElement deId = root.firstChildElement("Id");
	Id = deId.text().simplified();
	QDomElement deDesc = root.firstChildElement("Description");
	Description = deDesc.text().simplified();
	QDomElement deCategories = root.firstChildElement("Categories");
	categorySet.parse(deCategories, "Category");
	
	return DDT::Return();
} // readXmlFile()

DDT::Return fbPerson::writeXmlFile(const QString & fileName)
{
	QDomDocument doc("INDIface-Person");
	QDomElement root = doc.createElement("PersonRecord");
	doc.appendChild(root);
	if (Key)
		root.setAttribute("PersonKey", Key);
	root.setAttribute("Active", Active ? "1" : "0");

	QDomElement deId = doc.createElement("Id");
	root.appendChild(deId);
	QDomText txtId = doc.createTextNode(Id);
	deId.appendChild(txtId);

	if ( ! Description.isEmpty())
	{
		QDomElement deDesc = doc.createElement("Description");
		root.appendChild(deDesc);
		QDomText txt = doc.createTextNode(Description);
		deDesc.appendChild(txt);
	}

	if ( ! categorySet.isEmpty())
	{
		QDomElement deCategories = doc.createElement("Categories");
		if (categorySet.fill(&deCategories, "Category"))
			root.appendChild(deCategories);
	}

	QByteArray ba = doc.toByteArray(2);
	QFile xmlFile(fileName);
	if ( ! xmlFile.open(QIODevice::Truncate | QIODevice::WriteOnly))
		return DDT::Return::qfileError(xmlFile);
	if (ba.size() != xmlFile.write(ba))
		return DDT::Return::qfileError(xmlFile);
	else
		TRACE("Person Record written: %1", fileName);

	return DDT::Return();
} // writeXmlFile()
