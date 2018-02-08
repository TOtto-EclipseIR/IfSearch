/** @file iSettings.h
*	Definition of internal Settings-related classes for DDT core library
*
*	@author	Dynamic DeZigns Technology
*
*/

#pragma once
#include <Settings.h>

#include <QObject>
#include <QString>
#include <QThread>
#include <QTreeWidgetItem>

namespace DDT
{
	class SettingProperty;

	class SettingItem : public QObject
	{
		Q_OBJECT
		friend class Settings;
		friend class Setting;
		friend class SettingProperty;

	public:
		SettingItem(Settings * appSettings, QTreeWidget * Tree, Setting * Variant);
		SettingItem(Settings * appSettings, QTreeWidget * Tree, SettingProperty * Property);
		~SettingItem();

	public slots:
		void restoreFont(void);
		void showChanged(void);

	private:
		SettingItem(QTreeWidget * Tree, const QString & Name);
		QTreeWidgetItem * makePath(const QString & Key);
		void setupItem(QTreeWidgetItem * parentItem, const QString & Name, int f, QVariant * Var);

	private:
		QTreeWidgetItem * item;
		Setting * variant;
		SettingProperty * property;
		Settings * settings;
		QTreeWidget * tree;
	}; // class SettingItem



} // namespace DDT
