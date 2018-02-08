#include <EigenFaceTemplate.h>
#include <EigenFaceVector.h>

#include <QBuffer>
#include <QDomDocument>
#include <QIODevice>
#include <QFile>

#include <EigenFace.h>
#include <Return.h>

using namespace INDI;

const QString EigenFaceTemplate::eigenFaceTemplateTagName("INDI-EigenFace-Template");
const QString EigenFaceTemplate::eigenFaceVectorTagName("INDI-EigenFace-EigenFaceVector");

EigenFaceTemplate::EigenFaceTemplate(void)
{
	DDT::Return::add(EigenFace::ReturnStatusNull, "INDI EigenFace Null Status", DDT::Warning);
}
		
EigenFaceTemplate::EigenFaceTemplate(const QByteArray & ba)
{
	DDT::Return::add(EigenFace::ReturnStatusNull, "INDI EigenFace Null Status", DDT::Warning);
	clear();
	EigenFaceTemplate eft;
	eft.read(ba);
	if (eft.isValid())
		*this = eft;
} // QByteArray c'tor
		
EigenFaceTemplate::~EigenFaceTemplate()
{
}
		
bool EigenFaceTemplate::isValid(void) const
{
	if (distance_efVector_mmap.isEmpty())
		return false;
	return true;
}

int EigenFaceTemplate::consistency(void) const
{
	int rtn = 0;
	foreach (EigenFaceVector vec, distance_efVector_mmap)
		if (vec.consistency() > rtn)
			rtn = vec.consistency();
	return rtn;
}
		
void EigenFaceTemplate::add(const EigenFaceTemplate & other)
{
	distance_efVector_mmap += other.distance_efVector_mmap;
/*
	QMapIterator<qreal, EigenFaceVector> it(other.vectors);
	while (it.hasNext())
	{
		i.next();
		vectors.
*/
}

DDT::Return EigenFaceTemplate::read(const QDomElement & de)
{
	DDT::Return rtn;

	int k = 0, n = de.attribute("Vectors").toInt();

	QDomElement deVector = de.firstChildElement(EigenFaceTemplate::eigenFaceVectorTagName);
	while ( ! deVector.isNull())
	{
		EigenFaceVector vec;
		rtn = vec.read(deVector);
		if (rtn.isError())
			return rtn;
		if (vec.isValid())
			distance_efVector_mmap.insert(vec.consistency(), vec), ++k;
		deVector = deVector.nextSiblingElement();
	}

	if (k != n)
		return DDT::Return(EigenFace::ReturnConflict, "Vector Count", k, n);

	return rtn;
} // read(de)

DDT::Return EigenFaceTemplate::read(const QByteArray & ba)
{
	QBuffer buf;
	buf.setData(ba);
	return read(&buf);
} // read(ba)

DDT::Return EigenFaceTemplate::read(const QImage & image)
{
    QString xml = image.text("INDIface");
    if (xml.isEmpty())
        return DDT::Return(INDI::EigenFace::ReturnNoTemplate);

    QDomDocument doc("INDIface");
    QString errorMsg;
    int errorLine, errorColumn;
    if ( ! doc.setContent(xml, &errorMsg, &errorLine, &errorColumn))
        return DDT::Return(DDT::Return::ReturnXmlParse, "INDIface image",
                           errorMsg, errorLine, errorColumn);

    QDomElement de = doc.firstChildElement(eigenFaceTemplateTagName);
    if (de.isNull())
        return DDT::Return(DDT::Return::ReturnXmlParse, "INDIface image",
                           "Missing " + eigenFaceTemplateTagName, 0,0);

    return read(de);
} // read(image)

DDT::Return EigenFaceTemplate::read(const QString & imageFilename)
{
    QImage image(imageFilename);
    if (image.isNull())
        return DDT::Return(INDI::EigenFace::ReturnNullImage, imageFilename);
    else
        return read(image);
} // read(imageFilename)

DDT::Return EigenFaceTemplate::read(QIODevice * io)
{
	DDT::Return rtn;
	int i;

	clear();

	// Check Stream Id
	io->read((char *)&i, sizeof(i));
	if (StreamId != i)
		return DDT::Return(EigenFace::ReturnInvalidStructure, "EFT:StreamId");

	// Read data vector
	io->read((char *)&i, sizeof(i));
	while (i--)
	{
		EigenFaceVector vec;
		rtn = vec.read(io);
		if (rtn.isError())
			return rtn;
		if (vec.isValid())
			distance_efVector_mmap.insert(vec.consistency(), vec);
	}

	if (isValid())
		return DDT::Return();
	else
		return DDT::Return(EigenFace::ReturnInvalidStructure, "Template");
} // read(io)

DDT::Return EigenFaceTemplate::write(QDomDocument * doc) const
{
    QDomElement de = doc->firstChildElement(EigenFaceTemplate::eigenFaceTemplateTagName);
    while ( ! de.isNull())
    {
        doc->removeChild(de);
        de = de.nextSiblingElement(EigenFaceTemplate::eigenFaceTemplateTagName);
    }

    QDomElement tag = doc->createElement(EigenFaceTemplate::eigenFaceTemplateTagName);
    DDT::Return rtn = write(&tag);
    if ( ! rtn.isError())
        doc->appendChild(tag);

    return rtn;
}
		
DDT::Return EigenFaceTemplate::write(QDomElement * de) const
{
	de->setTagName(EigenFaceTemplate::eigenFaceTemplateTagName);
	de->setAttribute("Vectors", distance_efVector_mmap.size());

	QDomNodeList nodes = de->childNodes();
	for (int i = 0; i < nodes.size(); ++i)
		de->removeChild(nodes.item(i));

	foreach (EigenFaceVector vector, distance_efVector_mmap)
	{
		QDomElement deVector = de->ownerDocument().createElement(EigenFaceTemplate::eigenFaceVectorTagName);
		DDT::Return rtn = vector.write(&deVector);
		if (rtn.isError())
			return rtn;
		else
			de->appendChild(deVector);
	}

	return DDT::Return();
}

DDT::Return EigenFaceTemplate::write(QByteArray * ba) const
{
	DDT::Return rtn;

	if ( ! ba)
		return DDT::Return(EigenFace::ReturnStatusNull);
	if ( ! isValid())
		return DDT::Return(EigenFace::ReturnInvalidStructure, "Template");

	QBuffer buf;
	rtn = write(&buf);
	if (rtn.isError())
		return rtn;

	*ba = buf.data();
	return DDT::Return();
}
		
DDT::Return EigenFaceTemplate::write(QIODevice * io) const
{
	if ( ! isValid())
		return DDT::Return(EigenFace::ReturnInvalidStructure, "Template");

	// write stream id
	int i = StreamId;
	io->write((char *)&i, sizeof(i));

	i = distance_efVector_mmap.size();
	io->write((char *)&i, sizeof(i));
	foreach(EigenFaceVector vec, distance_efVector_mmap)
	{
		DDT::Return rtn = vec.write(io);
		if (rtn.isError())
			return rtn;
	}

	return DDT::Return();
} // write(io)

QStringList EigenFaceTemplate::idList(void) const
{
    QStringList result;
    foreach (EigenFaceVector vec, distance_efVector_mmap.values())
        result << vec.id();
    return result;
}

void EigenFaceTemplate::setVectorId(const QString & id)
{
    QMutableMapIterator<qreal, EigenFaceVector> it(distance_efVector_mmap);
    while (it.hasNext())
    {
        it.next();
        EigenFaceVector vec = it.value();
        vec.setId(id);
        it.setValue(vec);
    }
}

void EigenFaceTemplate::removeById(const QString & id)
{
    QMutableMapIterator<qreal, EigenFaceVector> it(distance_efVector_mmap);
    while (it.hasNext())
    {
        it.next();
        if (id == it.value().id())
            it.remove();
    }
}
