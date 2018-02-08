#ifndef VariableSettings_H
#define VariableSettings_H

#include <QtCore/QObject>
#include <QtCore/QSettings>
#include <QtCore/QStringList>

#include "VariableSet.h"

class EIRCORESHARED_EXPORT VariableSettings : public QObject, public VariableSet
{
    Q_OBJECT

public:
    explicit VariableSettings(QObject * parent=0);
    VariableSettings(QSettings * settings,
                     const QString & group=QString(),
                     QObject * parent=0);
    VariableSettings(QString string,
                     QObject * parent=0);
    VariableSettings(const QSettings::Format format,
                     const QString & fileName,
                     QObject * parent=0);
    VariableSettings(const QString & orgName,
                     const QString & appName,
                     QObject * parent=0);

    QString fileName(void) const;
    QSettings::Format format(void) const;
    QSettings::Scope scope(void) const;
    QString orgName(void) const;
    QString appName(void) const;
    QSettings::Status status(void) const;
    QString settingName(void) const;
    QString group(void) const;

    void addNotify(const QString & key);
    void addTrigger(const QString & key);
    void setUpdateMsec(const int msec);

    void read(void);
    QVariant read(const QString & key,
                  const QVariant & defaultValue=QVariant());
    void write(void) const;
    void write(const QString & key,
               const QVariant & newValue);
    void remove(const QString & key) const;
    void removeAll(void) const;

signals:
    void notify(QString key);
    void triggered(QString key);

private slots:
    void update(void);

private:
    QSettings * _settings;
    int update_msec;
    QStringList notify_list;
    QStringList trigger_list;
};

#endif // VariableSettings_H
