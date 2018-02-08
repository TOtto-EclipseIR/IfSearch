/** @file InfoOutputSetting.cpp
*	Definition of InfoOutputSetting class for DDT core library
*
*	@author	Dynamic DeZigns Technology
*
*/

#include <Info.h>
#include <Settings.h>

#include <QCoreApplication>

using namespace DDT;

InfoOutputSetting::InfoOutputSetting(const QString & key)
    : settingKey_(key)
{
    icons_.resize(IconFatal+1);
    //originalIcon_ = qApp->windowIcon();
}

QString InfoOutputSetting::pump(QString oldMessage)
{
    if (current_.severity() != InfoSeverity::Null)
    {
        if (oldMessage.isEmpty()
                || (current_.severity() < InfoSeverity::Error
                        && current_.msecSince() > 60000))
        {
            oldMessage = QString();
            current_.clear();
            QIcon icon = icons_.at(IconNormal);
            if (icon.isNull())
                icon = originalIcon_;
            //qApp->setWindowIcon(icon);
        }
    }

    if (current_.severity() == InfoSeverity::Null
            && ! queue_.isEmpty())
    {
        QIcon icon;
        current_ = queue_.dequeue();
        if (current_.severity() == InfoSeverity::Warning)
            oldMessage = "W", icon = icons_.at(IconWarning);
        else if (current_.severity() == InfoSeverity::Error)
            oldMessage = "E", icon = icons_.at(IconError);
        else if (current_.severity() == InfoSeverity::Fatal)
            oldMessage = "F", icon = icons_.at(IconFatal);
        else
            oldMessage = "?", icon = icons_.at(IconNormal);
        oldMessage += QString("%1 %2 %3").arg((unsigned)current_.returnCode(), 8, 16, QChar('0'))
                                         .arg(current_.time().toString("hh:mm:ss"))
                                         .arg(current_.stringOnly());
        if (icon.isNull())
            icon = originalIcon_;
        //qApp->setWindowIcon(icon);
    }

    return oldMessage;
}

void InfoOutputSetting::write(const InfoItem & item)
{
    QWriteLocker wLock(lock());
    if (item.severity() >= InfoSeverity::Fatal)
        queue_.clear();
    queue_.enqueue(item);
}

void InfoOutputSetting::flush(void)
{
}

void InfoOutputSetting::clear(void)
{
    QWriteLocker wLock(lock());
    current_.clear();
    queue_.clear();
}

bool InfoOutputSetting::isValid(void) const
{
    return true;
}
