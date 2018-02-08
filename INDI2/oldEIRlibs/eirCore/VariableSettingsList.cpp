#include "VariableSettingsList.h"

#include <QtCore/QTimer>
#include <QtCore/QVariant>

#include "../eirBase/BaseLog.h"
#include "IsTrigger.h"
#include "VariableIdList.h"

VariableSettingsList::VariableSettingsList(QObject * parent)
    : QObject(parent)
    , default_vsi(new VariableSettings(this))
    , update_msec(0)
    , updateRead_b(false)
    , watch_timer(0)
{
    default_vsi->setName("default");
    vsi_list.append(default_vsi);
}

VariableSettingsList::~VariableSettingsList()
{
    stop();
}

void VariableSettingsList::add(VariableSettings * vsi)
{
    vsi_list.prepend(vsi);
}

void VariableSettingsList::add(QString string)
{
    VariableSettings * vsi = new VariableSettings(string, this);
    if (QSettings::NoError == vsi->status())
    {
        vsi->setName(string);
        vsi->read();
        if ( ! vsi->isEmpty())
            vsi_list.prepend(vsi);
        else
            delete vsi;
    }
}

void VariableSettingsList::addTrigger(const VariableId & vid)
{
    trigger_set.insert(vid);
    set(vid, QString());
}

void VariableSettingsList::setUpdateMsec(const int msec)
{
    update_msec = msec;
    // if started, restart
    if (watch_timer) start();
}

void VariableSettingsList::setUpdateRead(const bool readOnUpdate)
{
    updateRead_b = readOnUpdate;
}

bool VariableSettingsList::contains(const VariableId & vid) const
{
    (void)vid;
    BTODO("VariableSettingsList::contains(const VariableId & vid)");
    return false;
}

VariableSettings * VariableSettingsList::settings(const int index) const
{
    return (index < 0 || index >= vsi_list.size())
            ? 0 : vsi_list.at(index);
}

void VariableSettingsList::read(void)
{
    foreach (VariableSettings * vsi, vsi_list)
        vsi->read(); // if flags?
}

QVariant VariableSettingsList::read(const VariableId & vid,
                                    const QVariant & defaultValue)
{
    foreach (VariableSettings * vsi, vsi_list)
        if (vsi->contains(vid))
            return vsi->read(vid);
    return defaultValue;
}

void VariableSettingsList::write(void) const
{
    foreach (VariableSettings * vsi, vsi_list)
        vsi->write();
}

void VariableSettingsList::write(const VariableId & vid,
                                 const QVariant & newValue)
{
    foreach (VariableSettings * vsi, vsi_list)
        if (vsi->contains(vid)) // && isWritable()??
        {
            vsi->write(vid, newValue);
            return;
        }
    vsi_list.first()->write(vid, newValue);
}

void VariableSettingsList::blog(void) const
{
    foreach (VariableSettings * vsi, vsi_list)
    {
        vsi->blog();
    }
}

void VariableSettingsList::set(const Variable & vbl)
{
    VariableId vid(vbl.id());
    foreach (VariableSettings * vsi, vsi_list)
        if (vsi->contains(vid))
        {
            vsi->set(vbl);
            return;
        }
    vsi_list.first()->set(vbl);
}

void VariableSettingsList::set(const VariableId & vid,
                               const QVariant & value)
{
    foreach (VariableSettings * vsi, vsi_list)
        if (vsi->contains(vid))
        {
            vsi->write(vid, value);
            return;
        }
    vsi_list.first()->write(vid, value);
}

void VariableSettingsList::setDefault(const Variable & vbl)
{
    default_vsi->set(vbl);
}

void VariableSettingsList::setDefault(const VariableId & vid,
                                      const QVariant & value)
{
    default_vsi->set(vid, value);
}

Variable VariableSettingsList::at(const VariableId & vid) const
{
    foreach (VariableSettings * vsi, vsi_list)
        if (vsi->contains(vid))
            return vsi->at(vid);
    return Variable();
}

QVariant VariableSettingsList::value(const VariableId & vid) const
{
    foreach (VariableSettings * vsi, vsi_list)
        if (vsi->contains(vid))
            return vsi->value(vid);
    return QVariant();
}

VariableSet VariableSettingsList::exportSection(const VariableId & sectionId) const
{
    VariableSet result("VariableSettingsList:" + sectionId);
    int n = sectionId.sectionCount();
    foreach (VariableSettings * vsi, vsi_list)
    {
        foreach (VariableId vid, vsi->ids(sectionId))
        {
            Variable vbl(vsi->at(vid));
            VariableId newId(vid.sections(n));
            if ( ! result.contains(newId))
            {
                result.set(newId, vbl.var());
            }
        }
    }
    result.blog();
    return result;
}

void VariableSettingsList::start(void)
{
    stop();

    foreach (VariableId vid, trigger_set)
        set(vid, QString());

    watch_timer = new QTimer(this);
    BOBJPOINTER(QTimer, watch_timer);
    BCONNECT(watch_timer, SIGNAL(timeout()),
            this, SLOT(update()));
    watch_timer->setInterval(update_msec
                             ? update_msec
                             : 20000);
    watch_timer->start();
    BEMIT(started());
}

void VariableSettingsList::stop(void)
{
    if (watch_timer)
    {
        BOBJPOINTER(QTimer, watch_timer);
        watch_timer->deleteLater();
        watch_timer = 0;
    }
    BEMIT(stopped());
}

void VariableSettingsList::update(void)
{
    if (updateRead_b)
    {
        read();
        foreach (VariableId vid, trigger_set)
        {
            QVariant var = value(vid);
            if (IsTrigger(var))
            {
                BEMIT(triggered(vid, var));
                set(vid, QString());
            }
        }
        write();
    }
    else
    {
        foreach (VariableId vid, trigger_set)
        {
            QVariant var = read(vid);
            if (IsTrigger(var))
            {
                BEMIT(triggered(vid, var));
                write(vid, QString());
            }
        }
    }
}
/*

void OptionSet::checkWatch(void)
{
    int updateMsec = value("Options/UpdateMsec").toInt();
    if (updateMsec && updateMsec != watch_timer->interval())
        watch_timer->setInterval(updateMsec);
    if ( ! updateMsec)  return;

    read();
    foreach (VariableId id, watch_list)
    {
        QVariant var = value(id);
        switch (var.type())
        {
        case QVariant::Invalid:     continue;

        default:
            if (var.canConvert(QVariant::String))
                if (var.toString().isEmpty())
                    continue;
        }
        TRACE("%1 = %2", id, var);
        EMIT(watched(id, var));
        set(id, QString());
    }

}
*/
