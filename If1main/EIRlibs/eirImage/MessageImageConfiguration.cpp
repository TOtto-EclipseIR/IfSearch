#include "MessageImageConfiguration.h"

#include "../eirBase/BaseLog.h"
#include "../eirCore/VariableIdList.h"
#include "../eirExe/Log.h"

MessageImageConfiguration::MessageImageConfiguration()
    : VariableSet("MessageImageConfiguration")
{
    FUNCTION();
    DEFINE_VARIABLESET_CTORS(MI_CONFIGURATION_VARIABLESET);
}

MessageImageConfiguration::MessageImageConfiguration(const VariableSet & other)
    : VariableSet(other)
{
    FUNCTION(other.name());
}

QSize MessageImageConfiguration::frameSize(const int width) const
{
    return getFrameRect().size(width ? width : getDefaultWidth());
}

QRect MessageImageConfiguration::fieldRect(const int width) const
{
    return getFieldRect().rect(width ? width : getDefaultWidth());
}

QRect MessageImageConfiguration::iconRect(const int width) const
{
    return getIconRect().rect(width ? width : getDefaultWidth());
}

QRect MessageImageConfiguration::codeRect(const int width) const
{
    return getCodeRect().rect(width ? width : getDefaultWidth());
}

QRect MessageImageConfiguration::titleRect(const int width) const
{
    return getTitleRect().rect(width ? width : getDefaultWidth());
}

QRect MessageImageConfiguration::textRect(const int width) const
{
    FUNCTION(width);
    FNRETURN(getTextRect().rect(width ? width : getDefaultWidth()));
    return getTextRect().rect(width ? width : getDefaultWidth());
}

QRect MessageImageConfiguration::severityRect(const int width) const
{
    return getSeverityRect().rect(width ? width : getDefaultWidth());
}

QRect MessageImageConfiguration::progressRect(const int width) const
{
    return getProgressRect().rect(width ? width : getDefaultWidth());
}

QBrush MessageImageConfiguration::frameBrush(void) const
{
    return QBrush(QColor(getFrameBrush()));
}

QBrush MessageImageConfiguration::codeBrush(void) const
{
    return QBrush(QColor(getCodeBrush()));
}

QBrush MessageImageConfiguration::titleBrush(void) const
{
    return QBrush(QColor(getTitleBrush()));
}

QBrush MessageImageConfiguration::textBrush(void) const
{
    return QBrush(QColor(getTextBrush()));
}

QBrush MessageImageConfiguration::progressBrush(void) const
{
    return QBrush(QColor(getProgressBrush()));
}

QPen MessageImageConfiguration::codePen(void) const
{
    return QPen(QColor(getCodePenColor()));
}

QPen MessageImageConfiguration::severityPen(void) const
{
    return QPen(QColor(getSeverityPenColor()));
}


QPen MessageImageConfiguration::textPen(void) const
{
    return QPen(QColor(getTextPenColor()));
}


QPen MessageImageConfiguration::titlePen(void) const
{
    return QPen(QColor(getTitlePenColor()));
}


QFont MessageImageConfiguration::codeFont(void) const
{
    return QFont(getCodeFontFamily(), getCodeFontPoints());
}

QFont MessageImageConfiguration::severityFont(void) const
{
    return QFont(getSeverityFontFamily(), getSeverityFontPoints());
}


QFont MessageImageConfiguration::textFont(void) const
{
    return QFont(getTextFontFamily(), getTextFontPoints());
}


QFont MessageImageConfiguration::titleFont(void) const
{
    return QFont(getTitleFontFamily(), getTitleFontPoints());
}

QBrush MessageImageConfiguration::severityBrush(const Severity sev)
{
    return QBrush(color_sevArray.at(sev));
}

QIcon MessageImageConfiguration::severityIcon(const Severity sev)
{
    return icon_sevArray.at(sev);
}

void MessageImageConfiguration::initialize(void)
{
    FUNCTION();

    /*--- First Defaults ---*/
    color_sevArray.set(QColor("blue"), Severity::Minimum);
    color_sevArray.set(QColor("green"), Severity::Progress);
    color_sevArray.set(QColor("orange"), Severity::Warning);
    color_sevArray.set(QColor("red"), Severity::Error);
    color_sevArray.set(QColor("blue"), BaseLog::fatalLevel());

    icon_sevArray.set(QQIcon("severity-null"), Severity::Minimum);
    icon_sevArray.set(QQIcon("severity-progress"), Severity::Progress);
    icon_sevArray.set(QQIcon("severity-warning"), Severity::Warning);
    icon_sevArray.set(QQIcon("severity-error"), Severity::Error);
    icon_sevArray.set(QQIcon("severity-fatal"), BaseLog::fatalLevel());

    /*--- Then read from VariableSet ---*/
    VariableSet fieldColor_vars = VariableSet::exportSection("FieldColor");
    foreach(VariableId vid, fieldColor_vars.ids())
    {
        Severity sev(vid);
        if (sev.isValid())
            color_sevArray.set(sev, QColor(fieldColor_vars.value(vid).toString()));
    }

    VariableSet icon_vars = VariableSet::exportSection("Icon");
    foreach(VariableId vid, icon_vars.ids())
    {
        Severity sev(vid);
        QQIcon icon(icon_vars.value(vid).toString());
        if (sev.isValid() && ! icon.isNull())
            icon_sevArray.set(sev, icon);
    }
}
