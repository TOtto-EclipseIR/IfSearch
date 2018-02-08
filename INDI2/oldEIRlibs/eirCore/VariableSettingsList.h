#ifndef VariableSettingsLIST_H
#define VariableSettingsLIST_H
#include "eirCore.h"

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QSet>
#include <QtCore/QSettings>

class QTimer;

#include "VariableSet.h"
#include "VariableSettings.h"

class EIRCORESHARED_EXPORT VariableSettingsList : public QObject
{
    Q_OBJECT

public:
    explicit VariableSettingsList(QObject * parent=0);
    ~VariableSettingsList();
    void add(VariableSettings * vsi);
    void add(QString string);
    void addTrigger(const VariableId & key);
    void setUpdateMsec(const int msec);
    void setUpdateRead(const bool readOnUpdate);
    bool contains(const VariableId & vid) const;
    VariableSettings * settings(const int index) const;
    void read(void);
    QVariant read(const VariableId & key,
                  const QVariant & defaultValue=QVariant());
    void write(void) const;
    void write(const VariableId & key,
               const QVariant & newValue);
    void blog(void) const;
    void set(const Variable & vbl);
    void set(const VariableId & vid,
             const QVariant & value);
    void setDefault(const Variable & vbl);
    void setDefault(const VariableId & vid,
             const QVariant & value);
    Variable at(const VariableId & vid) const;
    QVariant value(const VariableId & vid) const;
    VariableSet exportSection(const VariableId & sectionId) const;

signals:
    void started(void);
    void stopped(void);
    void triggered(QString key, QVariant value);

private slots:
    void update(void);

public slots:
    void start(void);
    void stop(void);

private:
    VariableSettings * default_vsi;
    QList<VariableSettings *> vsi_list;
    QSet<VariableId> trigger_set;
    int update_msec;
    bool updateRead_b;
    QTimer * watch_timer;
};

#endif // VariableSettingsLIST_H
