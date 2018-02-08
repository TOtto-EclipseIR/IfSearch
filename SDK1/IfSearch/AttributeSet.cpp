/** @file AttributeSet.cpp
*	Definition of AttributeSet class for DDT core library
*
*	@author	Dynamic DeZigns Technology
*
*/

#include <AttributeSet.h>

AttributeSet::AttributeSet(void)
{
}

AttributeSet::~AttributeSet()
{
}
		
void AttributeSet::set(const QString & section, 
					   const QString & variable, 
					   const QString & value, 
					   const QString & detail)
{
	QString key = section + "~" + variable;
	QString val = value;
	if ( ! detail.isEmpty())
		val += "~" + detail;
	map.insert(key, val);
} 
		
AttributeSet & AttributeSet::operator=(const AttributeSet & that)
{
	if (that.map.isEmpty())
		map.clear();
	else
		map = that.map;
	return *this;
}
		
bool AttributeSet::isEmpty(void)
{
	return map.isEmpty();
}

