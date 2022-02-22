#include <fbValue.h>

#include <QDomElement>



DualMap<int, QPair<QString, QString> >	fbValue::valueMap;
bool fbValue::dirty = false;
QChar fbValue::PairDelimiter(';');
QChar fbValue::SubDelimiter('~');

int fbValue::find(const QString & v, const QString & sv)
{
	QPair<QString, QString> vsv(v, sv);
	if (valueMap.contains(vsv))
		return valueMap.at(vsv);
	else
		return 0;
} // find()

int fbValue::insert(const QString & v, const QString & sv)
{
	QPair<QString, QString> vsv(v, sv);
	if (valueMap.contains(vsv))
		return valueMap.at(vsv);
	else
	{
		int rtn;
        do rtn = ((rand() << 13) ^ rand()) % 1000000;
		while (valueMap.contains(rtn) || 0 == rtn);
		valueMap.insertUnique(rtn, vsv);
		return rtn;
	}
} // insert()
		
QString fbValue::value(int x)
{
	QPair<QString, QString> vsv = valueMap.at(x);
	return vsv.first;
} // value(x)

QString fbValue::subvalue(int x)
{
	QPair<QString, QString> vsv = valueMap.at(x);
	return vsv.second;
} // subvalue(x)
