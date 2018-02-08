#include <EigenFaceMaskedArray.h>
#include <EigenFace.h>
#include <EigenFaceData.h>

#include <QByteArray>
#include <QBuffer>
#include <QDomDocument>
#include <QDomElement>
#include <QFile>
#include <QIODevice>
#include <QString>
#include <QVector>

#include <Return.h>

using namespace INDI;

DDT::Return EigenFaceMaskedArray::readXmlFile(const QString & fileName, 
											   const QString & tagName)
{
	QString errMsg;
	int errLine, errCol;
	QDomDocument doc;

	QFile file(fileName);
	if ( ! file.open(QIODevice::ReadOnly))
		return DDT::Return::qfileError(file);
	if ( ! doc.setContent(&file, &errMsg, &errLine, &errCol))
		return DDT::Return(DDT::Return::ReturnXmlParse, fileName, errMsg, errLine, errCol);

	QDomElement de = doc.firstChildElement(tagName);
	if (de.isNull())
		return DDT::Return(DDT::Return::ReturnXmlParse, fileName, 
				"Null " + EigenFaceData::bitMaskTagName + "element");
	
	return read(de);
}
DDT::Return EigenFaceMaskedArray::read(const QDomElement & de)
{
	clear();
	Id = de.attribute("Id").toInt();
	int n = de.attribute("Count").toInt();
	if ( ! n) 
		return DDT::Return(DDT::Return::ReturnXmlParse, de.tagName(),
				"Missing Count Attribute");

	QString txtString = de.text().simplified();
	QStringList txtFloats = txtString.split(" ", QString::SkipEmptyParts);
	if (n > txtFloats.size())
		return DDT::Return(DDT::Return::ReturnXmlParse, de.tagName(), "Missing Data");

	data.fill(0.0, n);
	minData = INT_MAX, maxData = INT_MIN;
	int x = 0;
	while (n)
	{
		bool ok;
		QString s = txtFloats.at(x);
		qreal v = s.toDouble(&ok);
		if ( ! ok)
		{
			clear();
			return DDT::Return(DDT::Return::ReturnXmlParse, de.tagName(), "Invalid floating point data");
		}
		data[x] = v;
		if (v < minData)	minData = v;
		if (v > maxData)	maxData = v;
		--n, ++x;
	}

	if ( ! isValid())
		return DDT::Return(EigenFace::ReturnInvalidStructure, de.tagName());

	return DDT::Return();
} // read(de)
				
DDT::Return EigenFaceMaskedArray::write(QByteArray * ba) const
{
	DDT::Return rtn;

	if ( ! ba)
		return DDT::Return(EigenFace::ReturnStatusNull);
	if ( ! isValid())
		return DDT::Return(EigenFace::ReturnInvalidStructure, "MaskedArray");

	QBuffer buf;
	rtn = write(&buf);
	if (rtn.isError())
		return rtn;

	*ba = buf.data();
	return DDT::Return();
} // write(ba)
		
DDT::Return EigenFaceMaskedArray::write(QIODevice * io) const
{
	if ( ! isValid())
		return DDT::Return(EigenFace::ReturnInvalidStructure, "MaskedArray");

	// write stream id
	int i = StreamId;
	io->write((char *)&i, sizeof(i));

	// write basic data
	io->write((char *)&Id, sizeof(Id));
	io->write((char *)&VectorKey, sizeof(VectorKey));
	io->write((char *)&Residual, sizeof(Residual));

	i = data.size();
	io->write((char *)&i, sizeof(i));
	if (i * sizeof(qreal) != io->write((char *)data.data(), i * sizeof(qreal)))
		return DDT::Return::errorString(io->errorString());

	return DDT::Return();
} // write(io)

DDT::Return EigenFaceMaskedArray::write(QDomElement * de) const
{
	if ( ! isValid())
		return DDT::Return(EigenFace::ReturnInvalidStructure, "MaskedArray");

	QString txtString;
	int n = efData->mask().count();

	de->setAttribute("Id", Id);
	de->setAttribute("MinData", minData);
	de->setAttribute("MaxData", maxData);
	de->setAttribute("Count", n);

	txtString.reserve(n * 16);
	for (int x = 0; x < n; ++x)
	{
		if (0 == x % 8)
			txtString.append("\n");
		txtString.append(QString::number(data.at(x), 'f', 12) + " ");
	}

	QDomText txtNode = de->ownerDocument().createTextNode(txtString);
	de->appendChild(txtNode);

	return DDT::Return();
} // write(de)

		
DDT::Return EigenFaceMaskedArray::write(QDomDocument * doc, const QString & tagName) const
{
	if ( ! isValid())
		return DDT::Return(EigenFace::ReturnInvalidStructure, tagName);

	QDomElement de = doc->firstChildElement(tagName);
	while ( ! de.isNull())
	{
		doc->removeChild(de);
		de = de.nextSiblingElement(tagName);
	}

	QDomElement tag = doc->createElement(tagName);
	DDT::Return rtn = write(&tag);
	if ( ! rtn.isError())
		doc->appendChild(tag);

	return rtn;
} // write(doc)

	

DDT::Return EigenFaceMaskedArray::read(const QByteArray & ba)
{
	QBuffer buf;
	buf.setData(ba);
	return read(&buf);
}

DDT::Return EigenFaceMaskedArray::read(QIODevice * io)
{
	int i;

	clear();

	// Check Stream Id
	io->read((char *)&i, sizeof(i));
	if (StreamId != i)
		return DDT::Return(EigenFace::ReturnInvalidStructure, "EFMA:StreamId");

	// Read basic data
	io->read((char *)&Id, sizeof(Id));
	io->read((char *)&VectorKey, sizeof(VectorKey));
	io->read((char *)&Residual, sizeof(Residual));

	// Read data vector
	io->read((char *)&i, sizeof(i));
	data.fill(i, 0.0);
	if (i * sizeof(qreal) != io->read((char *)data.data(), i * sizeof(qreal)))
		return DDT::Return::errorString(io->errorString());

	if (data.isEmpty())
		minData = maxData = 0.0;
	else
	{
		minData = INT_MAX, maxData = INT_MIN;
		foreach(qreal v, data)
		{
			if (v < minData)	minData = v;
			if (v > maxData)	maxData = v;
		}
	}

	if (isValid())
		return DDT::Return();
	else
		return DDT::Return(EigenFace::ReturnInvalidStructure, "MaskedArray");
}

