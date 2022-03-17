/** @file InfoOutputSetting.h
*	Internal declaration of InfoOutputSetting class for DDT core library
*
*	@author	Dynamic DeZigns Technology
*/
#pragma once
#include <qglobal.h>

#include <InfoItem.h>
#include <InfoOutputBase.h>

//#include <QIcon>
#include <QQueue>

	class Setting;

    class InfoOutputSetting : public InfoOutputBase
	{
	public:
//		enum IconState { IconNormal, IconNotify, IconWarning, IconError, IconFatal };

	public:
		InfoOutputSetting(const QString & key);
		~InfoOutputSetting() {}
		QString key(void) const { return settingKey_; }
		bool isEmpty(void) const { return queue_.isEmpty(); }
		InfoItem peek(void) const { return queue_.head(); }
//		void setAppIcon(IconState state, const QIcon & icon)
  //		{ icons_.replace(state, icon); }
		QString pump(QString oldMessage);

	protected:
		virtual void write(const InfoItem & item);
		virtual void clear(void);
		virtual void flush(void);
		virtual bool isValid(void) const;
		
	private:
		QString settingKey_;
		InfoItem current_;
		QQueue<InfoItem> queue_;
//		QVector<QIcon> icons_;
//		QIcon originalIcon_;
	}; // class InfoOutputSetting
