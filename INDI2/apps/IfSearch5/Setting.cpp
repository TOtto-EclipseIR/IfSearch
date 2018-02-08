/** @file Setting.cpp
*	Definition of Setting class for DDT core library
*
*	@author	Dynamic DeZigns Technology
*
*/

#include "Settings.h"

#include <QTimer>

Setting::Setting(Settings * Parent, const QString & Key, QVariant Default, Settings::Flags F)
    : QVariant(Default)
    , settings(Parent)
    , key(Key)
    , flags(F)
{
	settings->construct(this);
}

Setting::~Setting()
{
	settings->destruct(this);
}

void Setting::setValue(const QVariant & value)
{
	if (*(QVariant *)this != value)
	{
		*(QVariant *)this = value;
		flags |= Settings::Dirty | Settings::Changed;
		settings->emitValueChanged(key);
	}
} // setValue()
