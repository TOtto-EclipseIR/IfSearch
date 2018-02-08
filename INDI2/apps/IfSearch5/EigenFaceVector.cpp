#include <EigenFaceVector.h>
#include <EigenFace.h>

#include <QBuffer>
#include <QDomElement>
#include <QIODevice>
#include <QStringList>

#include <Return.h>

#include <math.h>



/*
		EigenFaceVector	VectorId;
		int PersonKey, FaceKey;
		qreal Residual;
		QVector<qreal> coefficients;
*/
		
EigenFaceVector::EigenFaceVector(void) 
{ 
	clear(); 
}

EigenFaceVector::EigenFaceVector(const QByteArray & ba)
{
	clear();
	EigenFaceVector efv;
	efv.read(ba);
	if (efv.isValid())
		*this = efv;
} // c'tor
		
void EigenFaceVector::clear(void)
{
	VectorKey = 0;
	PersonKey = FaceKey = 0;
	Residual = 0.0;
	coefficients.clear();
}
				
qreal EigenFaceVector::distance(const EigenFaceVector & other, 
								 const int max, 
								 QVector<qreal> weights)
{
	if ( ! isValid() || ! other.isValid())
		return -1.0;
	
	// TODO: Should this be calculated outside?
	int n = qMin(coefficients.size(), other.coefficients.size());
	if (max && max < n) n = max;
	if ( ! weights.isEmpty())
		n = qMin(n, weights.size());
	else
		weights.fill(1.0, n);

	qreal sumSquare = 0.0;
	for (int i = 0; i < n; ++i)
	{
		qreal d = coefficients[i] - other.coefficients[i];
		sumSquare += weights[i] * d * d;
	}
	return sqrt(sumSquare);
} // distance()

Return EigenFaceVector::read(const QDomElement & de)
{
	Return rtn;

	clear();

	int n = de.attribute("Count").toInt();
	if (n <= 0)
		return Return(EigenFace::ReturnNoData);

	VectorKey = de.attribute("VectorId").toInt();
	PersonKey = de.attribute("PersonKey").toInt();
	FaceKey = de.attribute("FaceKey").toInt();
	Residual = de.attribute("Residual").toDouble();

	QString txtString = de.text();
	QStringList coefs = txtString.simplified().split(" ", QString::SkipEmptyParts);
	if (n != coefs.size())
		return Return(EigenFace::ReturnInvalidStructure, "Bad Count");
	coefficients.fill(0.0, n);
	for (int i = 0; i < n; ++i)
		coefficients[i] = coefs[i].toDouble();

	return rtn;
} // read()

Return EigenFaceVector::write(QDomElement * de) const
{
	if ( ! isValid())
		return Return(EigenFace::ReturnInvalidStructure, "Vector");

	int n = coefficients.size();
	de->setAttribute("Count", n);
	de->setAttribute("VectorId", VectorKey);
	de->setAttribute("PersonKey", PersonKey);
	de->setAttribute("FaceKey", FaceKey);
	de->setAttribute("Residual", Residual);

	QString txtString;
	txtString.reserve(n * 16);
	for (int x = 0; x < n; ++x)
	{
		if (0 == x % 8)
			txtString.append("\n");
		txtString.append(QString::number(coefficients.at(x), 'f', 12) + " ");
	}

	QDomText txtNode = de->ownerDocument().createTextNode(txtString);
	de->appendChild(txtNode);

	return Return();
} // write(de)
		
Return EigenFaceVector::write(QByteArray * ba) const
{
	Return rtn;

	if ( ! ba)
		return Return(EigenFace::ReturnStatusNull);
	if ( ! isValid())
		return Return(EigenFace::ReturnInvalidStructure, "Vector");

	QBuffer buf;
	rtn = write(&buf);
	if (rtn.isError())
		return rtn;

	*ba = buf.data();
	return Return();
} // write(ba)

Return EigenFaceVector::write(QIODevice * io) const
{
	if ( ! isValid())
		return Return(EigenFace::ReturnInvalidStructure, "Vector");

	// write stream id
	int i = StreamId;
	io->write((char *)&i, sizeof(i));

	// write basic data
	io->write((char *)&VectorKey, sizeof(VectorKey));
	io->write((char *)&PersonKey, sizeof(PersonKey));
	io->write((char *)&FaceKey, sizeof(FaceKey));
	io->write((char *)&Residual, sizeof(Residual));

	i = coefficients.size();
	io->write((char *)&i, sizeof(i));
	if (i * sizeof(qreal) != io->write((char *)coefficients.data(), i * sizeof(qreal)))
		return Return::errorString(io->errorString());

	return Return();
} // write(io)
		
Return EigenFaceVector::read(const QByteArray & ba)
{
	QBuffer buf;
	buf.setData(ba);
	return read(&buf);
}

Return EigenFaceVector::read(QIODevice * io)
{
	int i;

	clear();

	// Check Stream Id
	io->read((char *)&i, sizeof(i));
	if (StreamId != i)
		return Return(EigenFace::ReturnInvalidStructure, "EFV:StreamId");

	// Read basic data
	io->read((char *)&VectorKey, sizeof(VectorKey));
	io->read((char *)&PersonKey, sizeof(PersonKey));
	io->read((char *)&FaceKey, sizeof(FaceKey));
	io->read((char *)&Residual, sizeof(Residual));

	// Read data vector
	io->read((char *)&i, sizeof(i));
	coefficients.fill(i, 0.0);
	if (i * sizeof(qreal) != io->read((char *)coefficients.data(), i * sizeof(qreal)))
		return Return::errorString(io->errorString());

	if (isValid())
		return Return();
	else
		return Return(EigenFace::ReturnInvalidStructure, "MaskedArray");
} // read(io)

