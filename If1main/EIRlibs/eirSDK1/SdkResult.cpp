#include "SdkResult.h"

#include <QtGui/QImage>

#include "../eirCore/eirCore.h"
#include "../eirVariable/CsvReader.h"
#include "../eirVariable/VariableTable.h"
#include "PersonReportRow.h"
#include "SdkCommand.h"

SdkResult::SdkResult(void)
    : VariableGroup(VariableId())
{
    DEFINE_GROUPVARIABLES_CTORS(SDKRESULT_GROUPVARIABLES);
}


bool SdkResult::from(SdkCommand * command)
{
    qDebug() << "SearchResult::from()" << command->getSequence()
             << command->getMode() << command->getCommand()
             << command->getStatus() << command->getReason();
    clear();
    QString mode(command->getMode());
    setSequence(command->getSequence()),
    setMode(mode),
    setCommand(command->getCommand()),
    setStatus(command->getStatus()),
    setReason(command->getReason()),
    setResults(command->getResults());
    VariableGroup settings;
    settings.implant("Settings", command);
    add(&settings, "Settings");

    if ( ! isValid())
    {
        qDebug() << "Not a valid/sucessful result";
        return false;
    }

    if ("Search" == mode)
    {
        QStringList personSections(command->sectionIds(VariableId("OutputImages")));
        //qDebug() << "personSections" << personSections;

        int rank = 0;
        foreach (QString s, personSections)
        {
            VariableId personSection("OutputImages");
            personSection.append(s);
            VariableId personRank(QString("R%1").arg(++rank, 2, 10, QChar('0')));
            QStringList personFaceIds = command->sectionIds(personSection);
            // expect personRank == s.left(3)
            int personMatchConfidence = s.mid(4,3).toInt();
            QString personId(s.mid(8));
            //qDebug() << "personFaceIds" << personFaceIds;

            int faceIndex = 0;
            foreach (QString s, personFaceIds)
            {
                VariableId matchFaceId(personSection);
                matchFaceId.append(s);
                VariableId faceRankIndex(personRank);
                faceRankIndex.append(QString("FX%2").arg(++faceIndex, 2, 10, QChar('0')));
                int faceKey = s.mid(10).toInt();
                VariableId faceKeyId(faceRankIndex);
                VariableId faceImageId(faceRankIndex);
                faceKeyId.append("FaceKey");
                faceImageId.append("FaceImage");
                set(faceKeyId, faceKey);
                set(faceImageId, command->value(matchFaceId));
                //qDebug() << s << faceKey << command->value(matchFaceId).value<QImage>().size();
            }

            VariableId matchId(personRank);
            VariableId personIdId(personRank);
            matchId.append("MatchConfidence");
            personIdId.append("PersonId");
            set(matchId, personMatchConfidence);
            set(personIdId, personId);
        }
        setPersonCount(rank);
    }
    else if ("Retrieve" == mode && "RetrieveAll" == command->getCommand())
    {
        QStringList personSections(command->sectionIds(VariableId("OutputImages")));
        qDebug() << "personSections" << personSections;

        int rank = 0;
        foreach (QString s, personSections)
        {
            VariableId personSection("OutputImages");
            personSection.append(s);
            VariableId personRank(QString("R%1").arg(++rank, 2, 10, QChar('0')));
            QStringList personFaceIds = command->sectionIds(personSection);
            // expect personRank == s.left(3)
            int personKey = s.mid(2,6).toInt();
            QString personId(s.mid(9));
            //qDebug() << "personFaceIds" << personFaceIds;

            int faceIndex = 0;
            foreach (QString s, personFaceIds)
            {
                VariableId matchFaceId(personSection);
                matchFaceId.append(s);
                VariableId faceRankIndex(personRank);
                faceRankIndex.append(QString("FX%2").arg(++faceIndex, 2, 10, QChar('0')));
                int faceKey = s.mid(2).toInt();
                VariableId faceKeyId(faceRankIndex);
                VariableId faceImageId(faceRankIndex);
                faceKeyId.append("FaceKey");
                faceImageId.append("FaceImage");
                set(faceKeyId, faceKey);
                set(faceImageId, command->value(matchFaceId));
                //qDebug() << s << faceKey << command->value(matchFaceId).value<QImage>().size();
            }

            VariableId keyId(personRank);
            VariableId personIdId(personRank);
            keyId.append("PersonKey");
            personIdId.append("PersonId");
            set(keyId, personKey);
            set(personIdId, personId);
        }
        setPersonCount(rank);
    }
    else if ("Retrieve" == mode && "Retrieve" == command->getCommand())
    {
        QStringList faceFiles(command->sectionIds(VariableId("OutputImages")));
        int rank = 0;
        foreach (QString s, faceFiles)
        {
            VariableId imageSection("OutputImages");
            VariableId faceRank(QString("R%1").arg(++rank, 2, 10, QChar('0')));
            int faceKey = s.mid(2).toInt();
            VariableId faceId(imageSection);
            faceId.append(s);
            VariableId faceKeyId(faceRank);
            faceKeyId.append("FaceKey");
            VariableId faceImageId(faceRank);
            faceImageId.append("FaceImage");
            set(faceKeyId, faceKey);
            set(faceImageId, command->value(faceId));
        }
    }
    return true;
}

bool SdkResult::isSearch(void) const
{
    return "Search" == getMode();
}

bool SdkResult::isRetrieve(void) const
{
    return "Retrieve" == getMode();
}

bool SdkResult::isEnroll(void) const
{
    return "Enroll" == getMode();
}

bool SdkResult::isError(void) const
{
    return "Error" == getStatus();
}

bool SdkResult::isValid(void) const
{
    if ( ! (isSearch() || isRetrieve() || isEnroll()) )
        return false;
    if ("Done" != getStatus())
        return false;
    return true;
}

QVariant SdkResult::get(const QString & name,
                           const int rank,
                           const int fx)
{
    VariableId vid(QString("R%1").arg(rank, 2, 10, QChar('0')));
    if (fx) vid.append(QString("FX%1").arg(fx, 2, 10, QChar('0')));
    vid.append(name);
    return VariableGroup::value(vid);
}

