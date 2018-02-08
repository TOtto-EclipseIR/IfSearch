/** @file SettingProperty.cpp
*	Definition of Setting class for DDT core library
*
*	@author	Dynamic DeZigns Technology
*
*/

#include <Settings.h>
#include "iSettings.h"

#include <QObject>
#include <QTimer>




SettingProperty::SettingProperty(Settings * Settings, 
								 QObject * Object, 
								 const QString & Key, 
								 const QString & PropertyName,
								 int F)
	: QObject(Settings), settings(Settings), object(Object), key(Key)
	, propertyName(PropertyName), flags(F), item(0)
{
	QObject::connect(Object, SIGNAL(destroyed(QObject *)), 
					 Settings, SLOT(objectDestroyed(QObject *)));
}

void SettingProperty::setValue(const QVariant & newValue)
{
	if (object->property(qPrintable(propertyName)) != newValue)
	{
		object->setProperty(qPrintable(propertyName), newValue);
		if (item)
			item->showChanged();
		flags |= Settings::Dirty | Settings::Changed;
		value = newValue;
		settings->emitPropertyChanged(key);
	}
}
