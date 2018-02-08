/** @file ImageInfo.cpp
*	Definition of ImageInfo class for DDT image library
*
*	@author	Dynamic DeZigns Technology
*
*/

#include <QDomDocument>
#include <QImage>
#include <QtDebug>

#include <ImageInfo.h>

using namespace DDT;

ImageInfo::ImageInfo(void)
{
	aSet.clear();
	fSets.clear();
} // default c'tor
		
ImageInfo::ImageInfo(const QImage & image)
{
	QString errorMsg;
	int errorLine, errorCol;

	QString xmlInfo = image.text("INDIinfo");
	if ( ! xmlInfo.isEmpty())
	{
		QDomDocument doc;
		if (doc.setContent(xmlInfo, &errorMsg, &errorLine, &errorCol))
		{
			read(doc.documentElement());
		}
	}
} // ImageInfo c'tor

ImageInfo::~ImageInfo()
{
} // d'tor


ImageInfo & ImageInfo::operator=(const ImageInfo & that)
{
//	aSet.clear();
	this->aSet  = that.aSet;
//	fSets.clear();
	this->fSets = that.fSets;
	return *this;
}
		
bool ImageInfo::read(const QDomElement & de)
{
	QDomNodeList nodesFaces = de.elementsByTagName("Faces");
	for (int kNode = 0; kNode < nodesFaces.size(); ++kNode)
	{
		QDomNode nodeFace = nodesFaces.at(kNode);
		QDomElement elementFeatureSet = nodeFace.firstChildElement("FeatureSet");
		while ( ! elementFeatureSet.isNull())
		{
			DDT::FeatureSet fSet(elementFeatureSet);
			fSets << fSet;
			elementFeatureSet = elementFeatureSet.nextSiblingElement("FeatureSet");
		}
	}
	return true;
} // read(QDomElement)

void ImageInfo::addFace(const FeatureSet & fSet)
{
	fSets.append(fSet);
}
		
QList<QPoint> ImageInfo::eyes(void) const
{
	QList<QPoint> list;
	foreach(FeatureSet fSet, fSets)
	{
		QPoint l = fSet.get(Feature::LeftEye).toPoint();
		QPoint r = fSet.get(Feature::RightEye).toPoint();
		if ( ! l.isNull() && ! r.isNull())
			list << l << r;
	}
	return list;
}

QList<QRect> ImageInfo::heads(void) const
{
	QList<QRect> list;
	foreach(FeatureSet fSet, fSets)
	{
		QRect rect = fSet.get(Feature::HeadBox).toRect();
		if (rect.isValid())
			list << rect;
	}
	return list;
}

QList<QRect> ImageInfo::heads(QString usage) const
{
	QList<QRect> list;
	foreach(FeatureSet fSet, fSets)
	{
		QRect rect = fSet.get(Feature::HeadBox).toRect();
		QString u = fSet.get(Feature::Usage).toString();
		if (rect.isValid() && u == usage)
			list << rect;
	}
	return list;
}

bool ImageInfo::isEmpty(void)
{
	return aSet.isEmpty() && fSets.isEmpty();
//	return fSets.isEmpty();
}
		
bool ImageInfo::writeDomElement(QDomElement * de) const
{
	QDomElement faces = de->ownerDocument().createElement("Faces");
    de->appendChild(faces);
	foreach(FeatureSet fSet, fSets)
	{
		QDomElement face = de->ownerDocument().createElement("FeatureSet");
		fSet.fillDomElement(&face);
		faces.appendChild(face);
	}
	return true;
} // toDomDocument()
		
bool ImageInfo::setImageText(QImage * image, QString tag)
{
	if (isEmpty() || ! image || image->isNull())
		return false;

	QDomDocument doc(tag);
	QDomElement root = doc.createElement(tag);
	doc.appendChild(root);
	writeDomElement(&root);
	QString xml = doc.toString();
	image->setText("INDIinfo", xml);
	return true;
} // setImageText()
