#include <FaceBase.h>

#include <QDomDocument>
#include <QString>
#include <QTime>

#include <math.h>

#include <Return.h>


#include <EigenFaceFace.h>
#include <EigenFaceTemplate.h>
#include <fbPerson.h>
#include <InfoMacros.h>	
#include "../../INDI2/oldEIRlibs/eirFile/QQDir.h"

Return FaceBase::status(void) const
{
    return Return(ReturnStatus,
                  people.size(),
                  peopleFaces[0].size(),
                  faceKeys.size(),
                  matcher ? matcher->size() : -1);
}


QString FaceBase::personId(const int key) const
{
    return people.at(key);
}

int FaceBase::personKey(const QString & id) const
{
    return people.at(id);
}

QString FaceBase::faceId(const int faceKey, const int personKey) const
{
    if ( ! peopleFaces.contains(personKey))
        return 0;
    else
        return peopleFaces[personKey].at(faceKey);
}

int FaceBase::faceKey(const QString & id,  const int personKey) const
{
    if ( ! peopleFaces.contains(personKey))
        return 0;
    else
        return peopleFaces[personKey].at(id);
}

EigenFaceFace FaceBase::face(const int faceKey) const
{
    EigenFaceFace rtn;
    rtn.readXmlFile(templateFilePath(faceKey));
    return rtn;
} // face()

bool FaceBase::getFace(EigenFaceFace * ptr, const int faceKey) const
{
    Return rtn = ptr->readXmlFile(templateFilePath(faceKey));
    return ! rtn.isError();
} // getFace()

int FaceBase::primaryFaceKey(const int personKey) const
{ 
    if ( ! mapPersonToFaces.contains(personKey))
        return 0;

    // TODO: Get person's primary face key else the following

    if (matcher)
        foreach (int fk, mapPersonToFaces[personKey])
            if (matcher->contains(fk))
                return fk;

    return mapPersonToFaces[personKey][0];
}


int FaceBase::newPersonKey(const QString & id)
{
    int rtn;
    int mod = (int)pow((qreal)10.0, personKeyDigits);
    do rtn = ((qrand() << 13) ^ qrand()) % mod;
    while (people.contains(rtn) || 0 == rtn);

    people.insertUnique(rtn, id);

    return rtn;
} // newPersonKey()

int FaceBase::newFaceKey(const QString & id, const int personKey)
{
    int rtn;
    int mod = (int)pow((qreal)10.0, faceKeyDigits);
    do rtn = ((qrand() << 13) ^ qrand()) % mod;
    while (faceKeys.contains(rtn) || 0 == rtn);

    peopleFaces[personKey].insertUnique(rtn, id);
    mapPersonToFaces[personKey].append(rtn);
    mapFaceToPerson.insert(rtn, personKey);
    faceKeys.insert(rtn);

    return rtn;
} // newFaceKey()


Return FaceBase::add(fbPerson * person)
{
    int key = person->key();
    if ( ! key)
    {
        if (person->id().isEmpty())
            return Return(ReturnMissingId, "Person");
        key = newPersonKey(person->id());
        person->setKey(key);
    }
    return person->writeXmlFile(personFilePath(key));
} // add(person)

Return FaceBase::add(EigenFaceFace * face, const QImage & image)
{	
    Return rtn;

    EigenFaceTemplate tpl = face->templat();

    int faceKey = face->faceKey();
    if ( ! faceKey)
    {
        if (face->faceId().isEmpty())
            return Return(ReturnMissingId, "Face");
        faceKey = newFaceKey(face->faceId(), face->personKey());
        face->setFaceKey(faceKey);
    }

    if (matcher)
        if (face->status().value() > 0)
            matcher->enroll(face->templat(), faceKey, face->personKey());

    rtn = face->writeXmlFile(templateFilePath(faceKey));
    RETURN(rtn);
    TRACE("FaceBase::add() normImage=%1,%2", image.width(), image.height());
    if ( ! rtn.isError() && ! image.isNull())
    {
        QString fileName = imageFilePath(faceKey);
        if (image.save(fileName, qPrintable(imageFileSuffix), 90))
            TRACE("Face Image written to %1", fileName);
        else
            ERRMSG("Error writing %1", fileName);
    }

    return rtn;
} // add(face)

QString FaceBase::enrolledImageName(const int faceKey) const
{
    return imageFilePath(faceKey);
} // enrolledImageName()

QImage FaceBase::enrolledImage(const int faceKey) const
{
    return QImage(enrolledImageName(faceKey));
} // enrolledImage()

EigenFaceStatus FaceBase::status(const int faceKey) const
{
    EigenFaceFace face;
    Return rtn = face.readXmlFile(templateFilePath(faceKey));
    return rtn.isError() ? EigenFaceStatus::Error : face.status();
}

Return FaceBase::deleteFace(const int faceKey)
{
    Return rtn;
    QDomDocument doc;

    EigenFaceFace face;
    rtn = face.readXmlFile(templateFilePath(faceKey));
    if (rtn.isError())
        return rtn;

    face.remove();
    face.setReason(QString());
    if (matcher)
        matcher->deleteFace(faceKey);

    rtn = face.writeXmlFile(templateFilePath(faceKey));
    if (rtn.isError())
        return rtn;

} // deleteFace()

Return FaceBase::removePerson(const int personKey)
{
    Return rtn;
    fbPerson person;

    rtn = person.readXmlFile(personFilePath(personKey));
    if (rtn.isError())
        return rtn;

    QList<int> keys = personFaceKeys(personKey);
    foreach (int faceKey, keys)
        deleteFace(faceKey);

    person.remove();
    rtn = person.writeXmlFile(personFilePath(personKey));
    if (rtn.isError())
        return rtn;

    return rtn;
} // removePerson()

bool FaceBase::personActive(const int personKey)
{
    Return rtn;
    fbPerson person;

    rtn = person.readXmlFile(personFilePath(personKey));
    if (rtn.isError())
        return false;
    return person.isActive();
}

Return FaceBase::removeAll(void)
{
    Return rtn;

    foreach (int personKey, people.all(0))
    {
        rtn = removePerson(personKey);
        if (rtn.isError())
            return rtn;
    }

    foreach (int faceKey, peopleFaces[0].all(0))
    {
        rtn = deleteFace(faceKey);
        if (rtn.isError())
            return rtn;
    }

    return rtn;
}

Return FaceBase::destroy(void)
{
    Return rtn;
    if (baseDir == QDir::root())
        rtn = Return::errorString("FaceBase/BaseDir invalid");
    else
    {
        QQDir::removeAll(baseDir);
        QQDir::removeAll(baseDir);
        rtn = Return::True;
    }
    clear();
    return rtn;
}

Return FaceBase::renamePerson(const int personKey, const QString & newId)
{
    Return rtn;
    fbPerson person;
    QString oldId = personId(personKey);

    if (oldId.isEmpty())
        return Return::False;

    rtn = person.readXmlFile(personFilePath(personKey));
    if (rtn.isError())
        return rtn;

    people.remove(oldId);
    people.insertUnique(personKey, newId);

    person.setId(newId);
    rtn = person.writeXmlFile(personFilePath(personKey));
    if (rtn.isError())
        return rtn;

    return rtn;
} // renamePerson()

Return FaceBase::setPersonDescription(const int personKey, const QString & description)
{
    Return rtn;
    fbPerson person;

    rtn = person.readXmlFile(personFilePath(personKey));
    if (rtn.isError())
        return rtn;

    person.setDescription(description);
    rtn = person.writeXmlFile(personFilePath(personKey));
    if (rtn.isError())
        return rtn;

    return rtn;
} // setPersonDescription()

QString FaceBase::personDescription(const int personKey)
{
    fbPerson person;

    Return rtn = person.readXmlFile(personFilePath(personKey));
    if (rtn.isError())
        return QString();
    else
        return person.description();
} // personDescription()
