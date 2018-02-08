#include <EigenFace.h>
#include "EigenFaceData.h"

#include <QBuffer>
#include <QByteArray>
#include <QColor>
#include <QDomDocument>
#include <QDomElement>
#include <QFile>
#include <QImage>
#include <QIODevice>
#include <QString>
#include <QVector>


using namespace INDI;


EigenFaceBitMask::EigenFaceBitMask(void)
{
} // c'tor

EigenFaceBitMask::EigenFaceBitMask(const QByteArray & ba)
{
	clear();
	EigenFaceBitMask efbm;
	efbm.read(ba);
	if (efbm.isValid())
		*this = efbm;
} // c'tor

EigenFaceBitMask::~EigenFaceBitMask()
{
} // d'tor
				
void EigenFaceBitMask::clear(void)
{
	maskSize = QSize();
	nBits = -1;
	data.clear();
} // clear()

bool EigenFaceBitMask::isValid(void) const
{
	if ( ! maskSize.isValid())
		return false;
	if (nBits != data.count(true))
		return false;
	if (data.size() != maskSize.width() * maskSize.height())
		return false;
	return true;
}

QImage EigenFaceBitMask::toImage(QImage::Format Format) const
{
	if ( ! isValid())
		return QImage();
	
	QImage image(maskSize, QImage::Format_Indexed8);
	image.setNumColors(2);
	image.setColor(0, qRgb( 64,  64,  64));
	image.setColor(1, qRgb(192, 192, 192));
	image.fill(0);
	int w = maskSize.width();
	for (int x = 0; x < data.size(); x++)
		if (data.testBit(x))
			image.setPixel(x % w, x / w, 1);
	image.convertToFormat(Format);
	return image;
} // toImage()
		
DDT::Return EigenFaceBitMask::readXmlFile(const QString & fileName)
{
	QString errMsg;
	int errLine, errCol;
	QDomDocument doc;

	QFile file(fileName);
	if ( ! file.open(QIODevice::ReadOnly))
		return DDT::Return::qfileError(file);
	if ( ! doc.setContent(&file, &errMsg, &errLine, &errCol))
		return DDT::Return(DDT::Return::ReturnXmlParse, fileName, errMsg, errLine, errCol);

	QDomElement de = doc.firstChildElement(EigenFaceData::bitMaskTagName);
	if (de.isNull())
		return DDT::Return(DDT::Return::ReturnXmlParse, fileName, 
				"Null " + EigenFaceData::bitMaskTagName + "element");
	
	return read(de);
} // readXmlFile()
		
DDT::Return EigenFaceBitMask::read(const QDomElement & de)
{
	int width = de.attribute("Width").toInt();
	int height = de.attribute("Height").toInt();
	int bitCount = de.attribute("BitCount").toInt();
	if ( ! width || ! height)
		return DDT::Return(DDT::Return::ReturnXmlParse, de.tagName(),
				"Missing Width or Height Attribute");

	QString txtString = de.text().simplified();
	if ((width + 1) * height - 1 > txtString.size())
		return DDT::Return(DDT::Return::ReturnXmlParse, de.tagName(),
				"Not enough text data");

	data.fill(false, width * height);
	int x = 0;
	nBits = 0;
	for (int row = 0; row < height; ++row)
	{
		for (int col = 0; col < width; ++col)
		{
			if ("+" == txtString.at(row * (width + 1) + col))
				data.setBit(x), ++nBits;
			++x;
		}
	}

	if (bitCount && nBits != bitCount)
		return DDT::Return(DDT::Return::ReturnXmlParse, de.tagName(),
				"Invalid Bit Count");

	maskSize.setWidth(width), maskSize.setHeight(height);
	if ( ! isValid())
		return DDT::Return(EigenFace::ReturnInvalidStructure, de.tagName());

	return DDT::Return();
} // read(de)

DDT::Return EigenFaceBitMask::read(const QByteArray & ba)
{
	QBuffer buf;
	buf.setData(ba);
	return read(&buf);
} // read(ba)

DDT::Return EigenFaceBitMask::read(QIODevice * io)
{
	int i;

	clear();

	// Check Stream Id
	io->read((char *)&i, sizeof(i));
	if (StreamId != i)
		return DDT::Return(EigenFace::ReturnInvalidStructure, "EFBM:StreamId");

	// Read basic data
	io->read((char *)&maskSize, sizeof(maskSize));
	io->read((char *)&nBits, sizeof(nBits));

	// Read data vector
	char c;
	int n = maskSize.width() * maskSize.height();
	i = 0;
	data.fill(false, n);
	while (i < n)
	{
		io->read(&c, sizeof(c));
		if ('-' != c)
			data.setBit(i);
		++i;
	}

	if (isValid())
		return DDT::Return();
	else
		return DDT::Return(EigenFace::ReturnInvalidStructure, "Bitmask");
} // read(io)

DDT::Return EigenFaceBitMask::write(QIODevice * io) const
{
	if ( ! isValid())
		return DDT::Return(EigenFace::ReturnInvalidStructure, "Bitmask");

	// write stream id
	int i = StreamId;
	io->write((char *)&i, sizeof(i));

	// write basic data
	io->write((char *)&maskSize, sizeof(maskSize));
	io->write((char *)&nBits, sizeof(nBits));

	// write bit data
	int n = maskSize.width() * maskSize.height();
	QByteArray ba(n, '-');
	i = 0;
	while (i < n)
	{
		if (data.at(i))
			ba[i] = '+';
		++i;
	}
	if (n != io->write(ba.constData(), n))
		return DDT::Return::errorString(io->errorString());

	return DDT::Return();
} // write(io)
		
DDT::Return EigenFaceBitMask::write(QByteArray * ba) const
{
	DDT::Return rtn;

	if ( ! ba)
		return DDT::Return(EigenFace::ReturnStatusNull);
	if ( ! isValid())
		return DDT::Return(EigenFace::ReturnInvalidStructure, "Bitmask");

	QBuffer buf;
	rtn = write(&buf);
	if (rtn.isError())
		return rtn;

	*ba = buf.data();
	return DDT::Return();
} // write(ba)

DDT::Return EigenFaceBitMask::write(QDomElement * de) const
{
	if ( ! isValid())
		return DDT::Return(EigenFace::ReturnInvalidStructure, "BitMask");

	de->setAttribute("Width", maskSize.width());
	de->setAttribute("Height", maskSize.height());
	de->setAttribute("BitCount", nBits);

	int x = 0;
	QString txtString;
	txtString.reserve((maskSize.width() + 1) * maskSize.height());
	for (int row = 0; row < maskSize.height(); ++row)
	{
		for (int col = 0; col < maskSize.width(); ++col)
			txtString.append((data.testBit(x++) ? "+" : "-"));
		txtString.append("\n");
	}
	QDomText txtNode = de->ownerDocument().createTextNode(txtString);
	de->appendChild(txtNode);

	return DDT::Return();
} // write(de)

DDT::Return EigenFaceBitMask::write(QDomDocument * doc) const
{
	if ( ! isValid())
		return DDT::Return(EigenFace::ReturnInvalidStructure, "BitMask");

	QDomElement de = doc->firstChildElement(EigenFaceData::bitMaskTagName);
	while ( ! de.isNull())
	{
		doc->removeChild(de);
		de = de.nextSiblingElement(EigenFaceData::bitMaskTagName);
	}

	QDomElement tag = doc->createElement(EigenFaceData::bitMaskTagName);
	DDT::Return rtn = write(&tag);
	if ( ! rtn.isError())
		doc->appendChild(tag);

	return rtn;
} // write(doc)

