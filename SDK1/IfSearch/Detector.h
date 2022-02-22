/** @file Detector.h
*
*	Public declarations for INDI project object detectors
*/

#pragma once
#include <qglobal.h>
#ifdef INDIFFD_LIB
# define INDIFFD_EXPORT Q_DECL_EXPORT
#else
# define INDIFFD_EXPORT Q_DECL_IMPORT
#endif

#include <QDomElement>
#include <QDir>
#include <QFile>
#include <QImage>
#include <QMultiMap>
#include <QObject>
#include <QSize>
#include <QString>
#include <QStringList>
class QSettings;
class QDomDocument;

#include "VersionInfo.h"

class ImageCache;
#include <QQRect.h>
#include <Return.h>

#include <opencv4/opencv2/opencv.hpp>
