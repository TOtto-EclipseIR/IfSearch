/** @file Settings.h
*	Public declaration of Settings class for DDT config library
*
*	@author	Dynamic DeZigns Technology
*/

#pragma once
#include <qglobal.h>

#include <QFlags>
#include <QList>
#include <QMap>
#include <QSettings>
#include <QString>
#include <QStringList>
#include <QVariant>
class QDomElement;
class QTimer;
class QTreeWidget;
class QTreeWidgetItem;
class QReadWriteLock;

#include <InfoMacros.h>

class Return;
class StatusHandler;
class VersionInfo;
class Settings;
class SettingItem;

class SettingProperty : public QObject
{
    Q_OBJECT
    friend class Settings;
    friend class SettingsScanner;
    friend class SettingsItem;

public:
    SettingProperty(Settings * Settings, QObject * Object,
                    const QString & Key, const QString & PropertyName,
                    int F=0);
    void setValue(const QVariant & newValue);

    //	private:
public:
    Settings *	settings;
    QObject * object;
    QString key;
    QString propertyName;
    QVariant value;
    int flags;
    SettingItem * item;
}; // class SettingProperty


class SettingsScanner : public QThread
{
    Q_OBJECT
    friend class Settings;

public:
    SettingsScanner(Settings * parent);
    ~SettingsScanner();

protected:
    virtual void run();

signals:
    void changeProperty(QString key, QVariant var);

private:
    Settings * settings;
    bool done;
}; // class SettingsScanner

class Settings : public QSettings
{
    Q_OBJECT
    Q_PROPERTY(int UpdateMsec READ updateMsec WRITE setUpdateMsec)
    Q_PROPERTY(bool WriteBack READ writeBack WRITE setWriteBack)
    Q_PROPERTY(bool AdvancedMode READ advancedMode WRITE setAdvancedMode)
    Q_PROPERTY(int PollCount READ pollCount)
    friend class Setting;
    friend class SettingItem;
    friend class SettingsScanner;
    friend class SettingProperty;

public:
    enum Flag
    {
        Volatile	= 0x00000001,
        ReadOnly	= 0x00000002,
        Advanced	= 0x00000004,
        Hidden		= 0x00000008,
        WidgetState	= 0x00000100,
        Geometry	= 0x00000200,
        Dirty		= 0x00010000,
        Changed		= 0x00020000,
    };
    Q_DECLARE_FLAGS(Flags, Flag)

private:
    Settings(QObject * parent = 0);
    Settings(const QString & organization,
             const QString & application = QString(),
             QObject * parent = 0);
    Settings(const QString & iniFilename,
             QObject * parent);

public:
    static Settings * newSettings(QObject * parent = 0);
    ~Settings();

    void appStart(void);
    void setPollCountKey(const QString & key, int count=0);
    void setInfoOutputSetting(InfoOutputSetting * infoOut)
    { infoOutputSetting = infoOut; setValue(infoOut->key(), QString()); }
    void objectProperties(QObject * Object,
                          const QString & BaseKey,
                          const QStringList & PropertyNames,
                          Flags F=0)
    { foreach(QString n, PropertyNames) objectProperty(Object, BaseKey, n, F); }
    void objectProperties(QObject * Object,
                          const QString & BaseKey,
                          const QList<QByteArray> & PropertyNames,
                          Flags F=0)
    { foreach(QByteArray n, PropertyNames) objectProperty(Object, BaseKey, n, F); }
    void objectProperty(QObject * Object, const QString & BaseKey, const QString & PropertyName, Flags F=0);
    void objectProperty(const QString & Key, QObject * Object, const QString & PropertyName, Flags F=0);
    void setTreeWidget(QTreeWidget * wgt);
    void startScanner(void);
    void startTimers(void);
    QVariant valueOf(const QString & key) const;
    QString programName(void) const { return arg0; }
    QStringList argList(void) const { return args; }
    int argSize(void) const { return args.size(); }
    QString argTake(void) { return args.size() ? args.takeFirst() : QString(); }
    QString arg(int x) { return (x < 0 || x > args.size()-1) ? QString() : args.at(x); }
    void dump(InfoSeverity sev=InfoSeverity::Info,
              const QString & prefix=QString());
    bool clearGroup(const QString & groupName);
    QString source(void) const;

public: // overrides
    QVariant value(const QString & key, const QVariant defaultValue=QVariant()) const;
    void setValue(const QString & key, const QVariant newValue);

private:
    void construct(Setting * child);
    void destruct(Setting * child);
    void addToTree(Setting * var);
    void addToTree(SettingProperty * prop);
    void emitValueChanged(QString key);
    void emitPropertyChanged(QString key);
    QString flagsString(int f);
    void setVersion(const VersionInfo & ver, const QString & key);

public:
    bool advancedMode(void) { return AdvancedMode; }
    bool writeBack(void) { return WriteBack; }
    int updateMsec(void) { return UpdateMsec; }
    int pollCount(void)	{ return PollCount; }
    void setWriteBack(bool b) { WriteBack = b; }
    void setUpdateMsec(int ms);
    void setAdvancedMode(bool b);

public slots:
    void scanForUpdate(void);
    void scan(void);
    void updatePollCount(void);

private slots:
    void objectDestroyed(QObject * Object);
    void itemChanged(QTreeWidgetItem * item, int column);
    void changeProperty(QString key, QVariant var);

signals:
    void valueChanged(QString key);
    void propertyChanged(QString key);
    void scanStart(void);
    void scanFinish(void);

private:
    QMap<QString, Setting *> vars;
    QMultiMap<QString, SettingProperty *> mKeyPropertyMap;
    QStringList treeKeys;
    QReadWriteLock * rwlVars;
    QReadWriteLock * rwlProps;
    bool WriteBack;
    bool AdvancedMode;
    int UpdateMsec;
    int PollCount;
    QTreeWidget * tree;
    SettingsScanner * scanner;
    QTimer * timerUpdate;
    QTimer * timerScan;
    QString keyForUpdateMsec;
    QString keyForPollCount;
    QStringList args;
    QMap<QString,QString> opts;
    QString arg0;
    InfoOutputSetting * infoOutputSetting;
}; // class Settings
Q_DECLARE_OPERATORS_FOR_FLAGS(Settings::Flags);

class  Setting : public QVariant
{
    friend class Settings;
    friend class SettingItem;
    friend class SettingsScanner;

public:
    Setting(Settings * Parent,
            const QString & Key,
            QVariant Default,
            Settings::Flags F=0);
    ~Setting();

    QString keyName(void) const { return key; }
    void setValue(const QVariant & value);

private:
    Settings *	settings;
    QString key;
    Settings::Flags flags;
    SettingItem * item;
}; // class Setting

#ifndef IJM
class  WidgetSetting : public QObject
{
    Q_OBJECT
    friend class Settings;
    friend class SettingItem;
    friend class SettingsScanner;

public:
    WidgetSetting(Settings * Parent,
                  QWidget * Widget,
                  const QString & KeyGroup,
                  Settings::Flags F=Settings::Geometry | Settings::Hidden);
    ~WidgetSetting();

private slots:
    void saveSettings(void);

private:
    Settings * parent;
    QWidget * widget;
    QString keyGroup;
    Settings::Flags flags;
    QByteArray baGeometry;
    QByteArray baState;
    SettingItem * item;
}; // class WidgetSetting

class  SettingSet : public QObject
{
    Q_OBJECT

public:
    enum Returns
    {
        ReturnStatusNull = 0x85020101,
    };

public:
    SettingSet(QObject * parent=0);
    ~SettingSet();
    void clear(void) { values.clear(); }
    Return readXmlFile(const QString & fileName);
    QVariant value(const QString & key, const QVariant & dflt=QVariant());

private:
    void readDomElement(QDomElement & de, QString prefix);

private:
    QMap<QString,QVariant> values;
}; // class SettingSet
#endif
