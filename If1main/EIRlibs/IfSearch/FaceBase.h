/** @file FaceBase.h
*
*	Public declarations for INDI project FaceBase
*/

#pragma once
#include <qglobal.h>
#ifdef INDIFB_LIB
# define INDIFB_EXPORT Q_DECL_EXPORT
#else
# define INDIFB_EXPORT Q_DECL_IMPORT
#endif

#include <EigenFace.h>
#include <EigenFaceSearcher.h>

#include <QObject>
#include <QDir>
class QDomDocument;
class QDomElement;
#include <QImage>
class QSqlRecord;

#include <DualMap.h>
#include <Return.h>

namespace INDI
{
    class EigenFaceFace;
    class EigenFaceStatus;
    class EigenFaceTemplate;
    class fbPerson;
    class ObjectDetectorClass;
    class ObjectDetectorData;
    //	class ObjectDetectorParameters;

    class INDIFB_EXPORT FaceBase : public QObject
    {
        Q_OBJECT

    public:
        enum Returns
        {
            ReturnStatusNull = 0x85060101,
            ReturnCreateDir, ReturnChangeDir, ReturnNewBaseDir,
            ReturnBadKey, ReturnWriteFile, ReturnMissingId,
            ReturnInitialized, ReturnInitWarning, ReturnStatus,
            ReturnMissingXml,
        };

    public:
        FaceBase(QObject * parent=0, EigenFaceSearcher * efm=0);
        ~FaceBase();

        void clear(void);
        DDT::Return initialize(const QString & initString, int maxLoad=0);
        void setMatcher(EigenFaceSearcher * efm) { matcher = efm; }
        static QSqlRecord tableFields(const QString & tableName);
        static QString tableOptions(const QString & tableName);
        DDT::Return setDataUrl(const QUrl url);
        QStringList enumerate(const ObjectDetectorClass cls);
        DDT::Return read(ObjectDetectorData * odd, const ObjectDetectorClass cls, QString name=QString());
        DDT::Return write(const ObjectDetectorData & odd);
        bool isEmpty(void) { return faceKeys.isEmpty(); }
        DDT::Return status(void) const;
        QString personId(const int key) const;
        int personKey(const QString & id) const;
        int personKey(const int faceKey) const
        { return mapFaceToPerson.value(faceKey); }
        QString faceId(const int faceKey, const int personKey=0) const;
        int faceKey(const QString & id,  const int personKey=0) const;
        EigenFaceStatus status(const int faceKey) const;
        bool isActive(const int faceKey) const
        { return EigenFaceStatus::Active == status(faceKey); }
        QString statusName(const int faceKey) const
        { return status(faceKey).name(); }
        int newPersonKey(const QString & id);
        int newFaceKey(const QString & id, const int personKey);
        INDI::EigenFaceFace face(const int faceKey) const;
        bool getFace(INDI::EigenFaceFace * ptr, const int faceKey) const;
        DDT::Return add(fbPerson * person);
        DDT::Return add(EigenFaceFace * face, const QImage & image=QImage());
        QImage enrolledImage(const int faceKey) const;
        QString enrolledImageName(const int faceKey) const;
        int primaryFaceKey(const int personKey) const;
        QList<int> personFaceKeys(const int personKey) const
        { return mapPersonToFaces.contains(personKey) ? mapPersonToFaces[personKey] : QList<int>(); }
        DDT::DualMap<int, QString> * peopleMap(void)
        { return &people; }
        QList<int> personKeys(void) const
        { return people.all(1); }
        DDT::DualMap<int, QString> * anonymousMap(void)
        { return &(peopleFaces[0]); }
        DDT::Return deleteFace(const int faceKey);
        DDT::Return removeAll(void);
        DDT::Return destroy(void);
        DDT::Return removePerson(const int personKey);
        bool personActive(const int personKey);
        DDT::Return renamePerson(const int personKey, const QString & newId);
        DDT::Return setPersonDescription(const int personKey, const QString & newId);
        QString personDescription(const int personKey);
        QString templateFilePath(const int faceKey) const
        { return filePath(templateFilePrefix, faceKey, faceKeyDigits, templateFileSuffix); }
        QString imageFilePath(const int faceKey) const
        { return filePath(imageFilePrefix, faceKey, faceKeyDigits, imageFileSuffix); }
        QString personFilePath(const int personKey) const
        { return filePath(personFilePrefix, personKey, personKeyDigits, personFileSuffix); }
        bool matcherContains(const int faceKey) const
        { return matcher ? matcher->contains(faceKey) : false; }
        bool containsFace(const int faceKey) const
        { return faceKeys.contains(faceKey); }

    signals:
        void initProgress(int done, int total);

    protected:
        QString filePath(const QString & prefix, const int key,
                         const int digits, const QString & suffix) const;
        DDT::Return initFrom(const QString & xmlFile);
        DDT::Return readFile(ObjectDetectorData * odd, const ObjectDetectorClass cls, QString name);
        DDT::Return readSql(ObjectDetectorData * odd, const ObjectDetectorClass cls, QString name);

    public:
        static const QString templateFilePrefix;
        static const QString imageFilePrefix;
        static const QString personFilePrefix;
        static const QString templateFileSuffix;
        static const QString imageFileSuffix;
        static const QString personFileSuffix;
        static const QString detectorTableName;
        static const QString dataSetTableName;
        static const QString eigenDataTableName;
        static const QString parameterSetTableName;
        static const QString personTableName;
        static const QString faceImageTableName;
        static const QString faceTemplateTableName;
        static const int faceKeyDigits;
        static const int personKeyDigits;


    signals:
        void error(QString faceId, QString msg);

    private:
        QDir dataDir;
        //		DDT::SqlConnection dataSql;
        EigenFaceSearcher * matcher;
        QDir baseDir;
        DDT::DualMap<int, QString>	people;
        QMap<int, DDT::DualMap<int, QString> > peopleFaces;
        QMap<int, QList<int> > mapPersonToFaces;
        QMap<int, int> mapFaceToPerson;
        QSet<int> faceKeys;
    }; // FaceBase
} // INDI
