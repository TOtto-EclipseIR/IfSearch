/** @file SettingItem.cpp
*	Definition of SettingItem class for DDT core library
*
*	@author	Dynamic DeZigns Technology
*
*/

#include "iSettings.h"


#include <QTimer>

SettingItem::SettingItem(Settings * appSettings, QTreeWidget * Tree, Setting * Variant)
	: settings(appSettings), tree(Tree), variant(Variant), property(0), item(0)
{
	settings->treeKeys.append(variant->key);
	QTreeWidgetItem * parent = makePath(variant->key);
	QStringList names = variant->key.split("/", QString::SkipEmptyParts);
	setupItem(parent, names.takeLast(), variant->flags, variant);
	variant->item = this;
} // c'tor(Var)

SettingItem::SettingItem(Settings * appSettings, QTreeWidget * Tree, SettingProperty * Property)
	: settings(appSettings), tree(Tree), variant(0), property(Property), item(0)
{
	settings->treeKeys.append(Property->key);
	QTreeWidgetItem * parent = makePath(Property->key);
	QStringList names = Property->key.split("/", QString::SkipEmptyParts);
	QVariant var = Property->object->property(qPrintable(Property->propertyName));
	setupItem(parent, names.takeLast(), Property->flags, &var);
	Property->item = this;
} // c'tor(Prop)

SettingItem::SettingItem(QTreeWidget * Tree, const QString & Name)
	: settings(0), tree(Tree), variant(0), property(0)
{
    (void)Name;
} // c'tor(Prop)

SettingItem::~SettingItem()
{
	if (settings && variant)
		settings->treeKeys.removeAll(variant->key);
	if (settings && property)
		settings->treeKeys.removeAll(property->key);
}

QTreeWidgetItem * SettingItem::makePath(const QString & Key)
{
	QStringList names = Key.split("/", QString::SkipEmptyParts);
	if (names.isEmpty())
		return 0;

	QTreeWidgetItem * treeItem = tree->invisibleRootItem();
	
	while (names.size() > 1)
	{
		QTreeWidgetItem * childItem = 0;
		QString name = names.takeFirst();

		// try to find existing name
		for (int x = 0; x < treeItem->childCount(); x++)
		{
			childItem = treeItem->child(x);
			if (0 == name.compare(childItem->data(0,0).toString()))
			{
				treeItem = childItem;
				break;
			}
			else
				childItem = 0;
		} // for(x)

		// not found, create a new one
		if ( ! childItem)
		{
			childItem = new QTreeWidgetItem;
			childItem->setData(0, 0, name);
			treeItem->addChild(childItem);
		}

		treeItem = childItem;
	} // while(>1)

	return treeItem;
}

void SettingItem::setupItem(QTreeWidgetItem * treeItem, const QString & Name, int settingFlags, QVariant * Var)
{
	if (settingFlags & Settings::Hidden)
		return;

	item = new QTreeWidgetItem;
	item->setData(0, 0, Name);
	item->setData(1, 0, *Var);
	treeItem->addChild(item);

	if (settingFlags & Settings::ReadOnly)
		item->setBackground(1, QBrush(Qt::lightGray));
	else
	{
		Qt::ItemFlags itemFlags;
		itemFlags |= Qt::ItemIsEditable;
		itemFlags |= Qt::ItemIsEnabled;
		item->setFlags(itemFlags);
	}
	item->setHidden( ! settings->advancedMode() && (settingFlags & Settings::Advanced));
}


void SettingItem::restoreFont(void)
{
	if (item)
	{
		QFont font = item->font(1);
		font.setBold(false);
		item->setFont(1, font);
	}
}


void SettingItem::showChanged(void)
{
	QVariant var;
	if (property)
		var = property->object->property(qPrintable(property->propertyName));
	if (variant)
		var = *variant;

	if ( ! var.isNull() && item)
	{
		item->setData(1, 0, var);
		QFont font = item->font(1);
		font.setBold(true);
		item->setFont(1, font);
		QTimer::singleShot(4000, this, SLOT(restoreFont()));
	}
}

