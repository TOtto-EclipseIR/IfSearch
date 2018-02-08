/** @file Setting.cpp
*	Definition of Setting class for DDT core library
*
*	@author	Dynamic DeZigns Technology
*
*/

#include "iSettings.h"
using namespace DDT;

#include <QTimer>

Setting::Setting(Settings * Parent, const QString & Key, QVariant Default, Settings::Flags F)
	: QVariant(Default), settings(Parent), key(Key), flags(F), item(0)
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
		if (item)
			item->showChanged();
		flags |= Settings::Dirty | Settings::Changed;
		settings->emitValueChanged(key);
	}
} // setValue()
