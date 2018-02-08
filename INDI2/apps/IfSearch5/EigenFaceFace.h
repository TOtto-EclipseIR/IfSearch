/** @file EigenFaceFace.h
*
*	Public declarations for INDI project EigenFaceFace record
*/

#pragma once
#include <qglobal.h>

class QDomDocument;
class QDomElement;
class QImage;

class Return;
#include <ImageInfo.h>


#include <EigenFace.h>
#include "EigenFaceStatus.h"
#include <EigenFaceTemplate.h>

class  EigenFaceFace
{
public:
    EigenFaceFace(void);
    ~EigenFaceFace();
    bool isValid(void) const;
    void clear(void);
    Return read(const QDomDocument & doc);
    Return readXmlFile(const QString & fileName);
    void remove(void);
    Return write(QDomDocument * doc) const;
    Return writeXmlFile(const QString & fileName) const;

    EigenFaceStatus status(void) const { return Status; }
    QString reason(void) const { return Reason; }
    int faceKey(void) const { return FaceKey; }
    int personKey(void) const { return PersonKey; }
    QString faceId(void) const { return FaceId; }
    QString personId(void) const { return PersonId; }
    ImageInfo info(void) const { return ii; }
    EigenFaceTemplate templat(void) const { return tpl; }
    int consistency(void) const { return tpl.consistency(); };

    void setStatus(EigenFaceStatus v) { Status = v; }
    void setReason(const QString & v) { Reason = v; }
    void setInfo(const ImageInfo & i) { ii = i; }
    void setFaceKey(int v) { FaceKey = v; }
    void setPersonKey(int v) { PersonKey = v; }
    void setPersonId(QString v) { PersonId = v; }
    void setFaceId(QString v) { FaceId = v; }
    void setTemplate(EigenFaceTemplate t) { tpl = t; }
    //		void setTemplatePtr(EigenFaceTemplate * p) { tpl = p; }

private:
    EigenFaceStatus Status;
    QString Reason;

    int FaceKey;
    QString FaceId;
    int PersonKey;
    QString PersonId;

    ImageInfo ii;
    EigenFaceTemplate tpl;
}; // EigenFaceFace
