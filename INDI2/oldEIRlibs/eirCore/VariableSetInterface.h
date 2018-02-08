#ifndef VARIABLESETINTERFACE_H
#define VARIABLESETINTERFACE_H

#include <QtCore/QObject>
#include <QtCore/QSettings>
#include <QtCore/QStringList>

#include "VariableSet.h"

class EIRCORESHARED_EXPORT VariableSetInterface : public QObject, public VariableSet
{
    Q_OBJECT
public:
    explicit VariableSetInterface(QObject * parent=0);
    explicit VariableSetInterface(QSettings * settings,
                                  const QString & group=QString(),
                                  QObject * parent=0);
    void setFileName(const QString & fileName,
                     QSettings::Format format);
    void setOrgAppName(const QString & orgName,
                      const QString & appName);
    void beginGroup(const QString & group);

    QString fileName(void) const;
    QSettings::Format format(void) const;
    QString orgName(void) const;
    QString appName(void) const;
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

public slots:
    void update(void);

private:
    QSettings * _settings;
    int update_msec;
    QStringList notify_list;
    QStringList trigger_list;
};

#endif // VARIABLESETINTERFACE_H
