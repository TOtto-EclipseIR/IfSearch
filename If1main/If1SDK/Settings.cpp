/** @file Settings.cpp
*	Definition of Settings class for DDT core library
*
*	@author	Dynamic DeZigns Technology
*
*/

#include "iSettings.h"

#include <QApplication>
#include <QMutableMapIterator>
#include <QTimer>

using namespace DDT;

Settings::Settings(const QString & organization,
                   const QString & application,
                   QObject * parent)
    : QSettings(organization, application, parent)
    , tree(0)
    , scanner(0)
    , timerUpdate(0)
    , timerScan(0)
    , infoOutputSetting(0)
{
    PollCount = 0;
    UpdateMsec = 0;
    WriteBack = false;
    AdvancedMode = false;
}

Settings::Settings(const QString & iniFilename,
                    QObject * parent)
    : QSettings(iniFilename, QSettings::IniFormat, parent)
    , tree(0)
    , scanner(0)
    , timerUpdate(0)
    , timerScan(0)
    , infoOutputSetting(0)
{
    PollCount = 0;
    UpdateMsec = 0;
    WriteBack = false;
    AdvancedMode = false;
}


Settings::~Settings()
{
    delete timerScan;
    delete timerUpdate;

    if (scanner)
    {
        scanner->done = true;
        scanner->wait(11000);
        if ( ! scanner->isFinished())
            scanner->terminate();
    }
}

Settings * Settings::newSettings(QString organization,
            QString application,
            QObject * parent)
{
    Settings * instance = 0;

    // Scan Applications argument list
    QStringList argList = qApp->arguments();
    QString arg0 = argList.takeFirst();
    QStringList args;
    QString iniFilename;
    QMap<QString,QString> opts;

    foreach (QString arg, argList)
    {
        if (arg.startsWith(QChar('/')))
        {
            int x = arg.indexOf(QChar('='));
            if (x < 0)
                opts.insert(arg.mid(1).toLower(), QString());
            else
            {
                QString opt = arg.mid(1, x-1);
                QString val = arg.mid(x+1);
                opts.insert(opt.toLower(), val);
            }
        }
        else if (arg.startsWith(QChar('%')))
        {
            int x = arg.indexOf(QChar('/'));
            if (x < 0)
                application = arg.mid(1);
            else
            {
                organization = arg.mid(1, x-1);
                application = arg.mid(x+1);
            }
        }
        else if (arg.startsWith('@'))
        {
            iniFilename = arg.mid(1);
        }
        else
        {
            args.append(arg);
        }
    } // foreach(arg)

    if (iniFilename.isEmpty())
        instance = new Settings(organization, application, parent);
    else
        instance = new Settings(iniFilename);
    if (instance)
    {
        instance->arg0 = arg0;
        instance->args = args;
        instance->opts = opts;
    }
    return instance;
}

void Settings::startTimers(void)
{
    if (timerUpdate)
    {
        delete timerUpdate;
        timerUpdate = 0;
    }
    if (timerScan)
    {
        delete timerScan;
        timerScan = 0;
    }

    timerUpdate = new QTimer(this);
    timerScan = new QTimer(this);

    timerUpdate->setInterval(20000);
    connect(timerUpdate, SIGNAL(timeout()), this, SLOT(scanForUpdate()));

    timerScan->setInterval(UpdateMsec);
    if (UpdateMsec)
    {
        connect(timerScan, SIGNAL(timeout()), this, SLOT(scan()));
        timerScan->start();
    }

    timerUpdate->start();
} // startTimers()

void Settings::setUpdateMsec(int ms)
{
    if (timerScan)
    {
        disconnect(timerScan, SIGNAL(timeout()), this, SLOT(scan()));
        timerScan->stop();

        timerScan->setInterval(ms);

        if (ms)
        {
            connect(timerScan, SIGNAL(timeout()), this, SLOT(scan()));
            timerScan->start();
        }
    }
    UpdateMsec = ms;
}

void Settings::setPollCountKey(const QString & key, int count)
{

    if (key.isEmpty())
        disconnect(this, SIGNAL(scanFinish()), this, SLOT(updatePollCount()));
    else
        connect(this, SIGNAL(scanFinish()), this, SLOT(updatePollCount()));
    keyForPollCount = key;
    PollCount = count;
    updatePollCount();
}

void Settings::construct(Setting * child)
{
    vars[child->key.toLower()] = child;
    QVariant def = *child;
    QVariant var = value(child->key, def);
    *(QVariant *)child = var;
    QString key = child->key.toLower();
    if (opts.contains(key))
    {
        *(QVariant *)child = opts.value(key);
        opts.remove(key);
        child->flags |= Settings::Dirty | Settings::Changed;
    }
    if (tree)
        addToTree(child);
} // construct()

void Settings::objectProperty(QObject * Object, const QString & BaseKey, const QString & PropertyName, Flags F)
{
    QString key = BaseKey + (BaseKey.isEmpty() ? "" : "/") + PropertyName;
    objectProperty(key, Object, PropertyName, F);
} // objectProperty(basekey)

void Settings::objectProperty(const QString & Key, QObject * Object, const QString & PropertyName, Flags F)
{
    SettingProperty * child = new SettingProperty(this, Object, Key, PropertyName, F);
    properties.insert(Key.toLower(), child);
    QVariant def = Object->property(qPrintable(PropertyName));
    QVariant var = value(Key, def);
    child->value = var;
    if (var != def)
        Object->setProperty(qPrintable(PropertyName), var);
    QString mapKey = Key.toLower();
    if (opts.contains(mapKey))
    {
        Object->setProperty(qPrintable(PropertyName), opts.value(mapKey));
        opts.remove(mapKey);
        child->flags |= Settings::Dirty | Settings::Changed;
    }
    if (tree)
        addToTree(child);
    if (this == Object && 0 == PropertyName.compare(tr("UpdateMsec", "config"), Qt::CaseInsensitive))
        keyForUpdateMsec = Key;
} // objectProperty(key)

void Settings::destruct(Setting * child)
{
    if (WriteBack
        && ! child->flags.testFlag(ReadOnly)
        && child->flags.testFlag(Dirty))
    {
        QVariant var = *child;
        setValue(child->key, var.toString());
    }
    vars.remove(child->key.toLower());
} // destruct()

void Settings::objectDestroyed(QObject * Object)
{
    QMutableMapIterator<QString, SettingProperty *> it(properties);
    while (it.hasNext())
    {
        it.next();
        SettingProperty * child = it.value();
        if (child->object == Object)
        {
            if (WriteBack
                && ! (child->flags & ReadOnly)
                && (child->flags & Dirty))
            {
                QVariant var = child->object->property(qPrintable(child->propertyName));
                setValue(child->key, var.toString());
            }
            delete child;
            it.remove();
        }
    }
} // objectDestroyed()

void Settings::setTreeWidget(QTreeWidget * wgt)
{
    if ( ! wgt)
        return;

    tree = wgt;
    tree->clear();
    tree->setColumnCount(2);
    tree->setColumnWidth(0, 200);
    QTreeWidgetItem * hdr = new QTreeWidgetItem;
    hdr->setData(0, 0, tr("Name", "ConfigView Header"));
    hdr->setData(1, 0, tr("Value", "ConfigView Header"));
    tree->setHeaderItem(hdr);

    foreach(Setting * var, vars)
        addToTree(var);
    foreach(SettingProperty * prop, properties)
        addToTree(prop);

    connect(tree, SIGNAL(itemChanged(QTreeWidgetItem*,int)),
            this, SLOT(itemChanged(QTreeWidgetItem*,int)));
} // setTreeWidget()

void Settings::itemChanged(QTreeWidgetItem * item, int column)
{
    if (1 != column)
        return;

    QString key;
    foreach(Setting * var, vars)
    {
        if (var->item && var->item->item == item)
        {
            key = var->key;
            break;
        }
    }
    if ( ! key.isEmpty())
    {
        foreach(Setting * var, vars)
            if (var->key == key)
                var->setValue(item->data(1, 0));
        return;
    }

    foreach(SettingProperty * prop, properties)
    {
        if (prop->item && prop->item->item == item)
        {
            key = prop->key;
            break;
        }
    }
    if ( ! key.isEmpty())
    {
        foreach(SettingProperty * prop, properties)
            if (prop->key == key)
                prop->setValue(item->data(1, 0));
    }
} // itemChanged()


QVariant Settings::valueOf(const QString & key) const
{
    QVariant rtn(QVariant::String);
    QString mapKey = key.toLower();

    if (vars.contains(mapKey))
    {
        Setting * var = vars[mapKey];
        rtn = *(QVariant *)var;
    }
    else if (properties.contains(mapKey))
    {
        SettingProperty * prop = properties.value(mapKey);
        rtn = prop->value;
    }
    else if (opts.contains(mapKey))
    {
        rtn = opts.value(mapKey);
    }

    return rtn;
}


QVariant Settings::value(const QString & key, const QVariant defaultValue) const
{
    QVariant result;
    QString mapKey = key.toLower();

    if (opts.contains(mapKey))
        result = opts.value(mapKey);
    else
        result = QSettings::value(key, defaultValue);
    return result;
} // value() override

void Settings::setValue(const QString & key, const QVariant newValue) const
{
    if (vars.contains(key))
    {
        Setting * var = vars[key];
        //var->setValue(newValue);
        *(QVariant *)var = newValue;
    }
    else if (properties.contains(key))
    {
        SettingProperty * prop = properties.value(key);
        //prop->setValue(newValue);
        *(QVariant *)prop = newValue;
    }
    QSettings::setValue(key, newValue);
} // setValue() override

void Settings::addToTree(Setting * var)
{
    if (tree && ! treeKeys.contains(var->key))
        new SettingItem(this, tree, var);
} // addToTree(var)


void Settings::addToTree(SettingProperty * prop)
{
    if (tree && ! treeKeys.contains(prop->key))
        new SettingItem(this, tree, prop);
} // addToTree(prop)

void Settings::startScanner(void)
{
    if (scanner)
    {
        if ( ! scanner->isRunning())
            scanner->start(QThread::LowestPriority);
    }
    else
    {
        scanner = new SettingsScanner(this);
        qRegisterMetaType<QVariant>("QVariant");
        connect(scanner, SIGNAL(changeProperty(QString,QVariant)),
                this, SLOT(changeProperty(QString,QVariant)));
        scanner->start(QThread::LowestPriority);
    }


} // startScanner()


void Settings::changeProperty(QString key, QVariant var)
{
    QList<SettingProperty *> props = properties.values(key.toLower());
    foreach (SettingProperty * prop, props)
        prop->setValue(var);
} // changeProperty() SLOT




void Settings::emitValueChanged(QString key)
{
    emit valueChanged(key);
}

void Settings::emitPropertyChanged(QString key)
{
    emit propertyChanged(key);
}


void Settings::setAdvancedMode(bool b)
{
    AdvancedMode = b;
    foreach(Setting * var, vars)
    {
        if (var->item && var->item->item)
            var->item->item->setHidden( ! AdvancedMode && (var->flags & Advanced));
    }
    foreach(SettingProperty * prop, properties)
    {
        if (prop->item && prop->item->item)
            prop->item->item->setHidden( ! AdvancedMode && (prop->flags & Advanced));
    }
} // setAdvancedMode()


void Settings::dump(InfoSeverity sev, const QString & prefix)
{
    QString flags, value, objName;

    DUMPVAL(sev, "---Settings from ", source());
    foreach(Setting * var, vars)
    {
        if ( ! var->key.startsWith(prefix, Qt::CaseInsensitive))
            continue;
        flags = flagsString(var->flags);
        value = var->toString();
        if (value.isEmpty())
            DUMPVAL(sev, QString("   [%1] {%2}").arg(flags).arg(var->key), "empty");
        else
            DUMPVAL(sev, QString("   [%1] {%2}").arg(flags).arg(var->key), value);
    }
    foreach(SettingProperty * prop, properties)
    {
        if ( ! prop->key.startsWith(prefix, Qt::CaseInsensitive))
            continue;
        flags = flagsString(prop->flags);
        if (prop->object)
            objName = prop->objectName();
        if (objName.isEmpty())
            objName = QString::number((unsigned int)prop->object, 16);
        if (prop->value.isNull())
            DUMPVAL(sev, QString("   [%1] {%2} for %3").arg(flags).arg(prop->key).arg(objName), "empty");
        else
            DUMPVAL(sev, QString("   [%1] {%2} for %3").arg(flags).arg(prop->key).arg(objName), prop->value);
    }
} // dump(sev)

QString Settings::flagsString(int f)
{
    QString rtn;
    rtn += (f & Volatile)	? "V" : " ";
    rtn += (f & ReadOnly)	? "R" : " ";
    rtn += (f & Advanced)	? "A" : " ";
    rtn += (f & Hidden)		? "H" : " ";
    rtn += (f & WidgetState)? "W" : " ";
    rtn += (f & Geometry)	? "G" : " ";
    rtn += (f & Dirty)		? "D" : " ";
    rtn += (f & Changed)	? "C" : " ";
    return rtn;
}

bool Settings::clearGroup(const QString & groupName)
{
    if ( ! childGroups().contains(groupName))
        return false;
    beginGroup(groupName);
    remove(groupName);
    endGroup();
    return true;
}

QString Settings::source(void) const
{
    if (QSettings::fileName().isEmpty())
        return QSettings::organizationName()
                + "/" + QSettings::applicationName();
    else
        return QSettings::fileName();
}
