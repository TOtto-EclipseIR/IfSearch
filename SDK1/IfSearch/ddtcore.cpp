#include <DDTcore.h>
#include <QQRect.h>
#include <Return.h>

#include <QtDebug>
#include <QDomDocument>
#include <QDomDocumentType>
#include <QDomElement>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QLine>
#include <QMetaProperty>
#include <QPoint>
#include <QQueue>
#include <QRect>
#include <QStringList>
#include <QVariant>


DDTcore::DDTcore()
{
}

DDTcore::~DDTcore()
{
}

#if 0 //ndef IJM
VersionInfo DDTcore::version(VERSION_MAJOR, VERSION_MINOR, VERSION_MAINT, VERSION_BUILD,
                         VERSION_NAME, VERSION_DESCRIPTION_EN, VERSION_COPYRIGHT_EN, VERSION_DATE);
#endif



bool DDTcore::parseDomElement(QVariant * rtnVar, const QDomElement & elementVar)
{
    bool ok = true;
    QVariant var;

    QString type = elementVar.attribute("Type");
    QString text = elementVar.text();

    if ("QString" == type || "bool" == type)
        var = QVariant(text);
    else if ("int" == type)
        var = QVariant(text.toInt());
    else if ("QPoint" == type)
        var = QVariant(QPoint(elementVar.attribute("X").toInt(),
                             elementVar.attribute("Y").toInt()));
    else if ("QSize" == type)
        var = QVariant(QSize(elementVar.attribute("Width").toInt(),
                             elementVar.attribute("Height").toInt()));
    else if ("QRect" == type)
    {
        QQRect rect(QSize(elementVar.attribute("Width").toInt(),
                          elementVar.attribute("Height").toInt()),
                    QPoint(elementVar.attribute("CenterX").toInt(),
                           elementVar.attribute("CenterY").toInt()));
        var = QVariant(rect);
    }
    else if ("QLine" == type)
    {
        QLine line(QPoint(elementVar.attribute("X1").toInt(),
                          elementVar.attribute("Y2").toInt()),
                    QPoint(elementVar.attribute("X2").toInt(),
                           elementVar.attribute("Y2").toInt()));
        var = QVariant(line);
    }
    else if ("QRectF" == type)
    {
        QRectF rectf(QPointF(elementVar.attribute("Left").toDouble(),
                             elementVar.attribute("Top").toDouble()),
                     QPointF(elementVar.attribute("Right").toDouble(),
                             elementVar.attribute("Bottom").toDouble()));
        var = QVariant(rectf);
    }
    else if ("QDateTime" == type)
    {
        QDate date = QDate::fromString(elementVar.attribute("Date"), Qt::ISODate);
        QTime time = QTime::fromString(elementVar.attribute("Time"), Qt::ISODate);
        var = QVariant(QDateTime(date, time));
    }
    else
    {
        ok = false;
    }

    if (rtnVar && ok)
        *rtnVar = var;
    else
        ok = false;
    return ok;
}  // parseDomElement()

bool DDTcore::appendDomElement(QDomElement * element, const QString & name, const QVariant & var)
{
    bool ok = true;
    QString txtString;

    if (var.isNull())
        return false;

    QDomElement tag = element->ownerDocument().createElement("Variant");
    tag.setAttribute("Name", name);
    tag.setAttribute("Type", var.typeName());

    switch (var.type())
    {
    case QVariant::Size:
        tag.setAttribute("Height", var.toSize().height());
        tag.setAttribute("Width", var.toSize().width());
        break;

    case QVariant::Point:
        tag.setAttribute("X", var.toPoint().x());
        tag.setAttribute("Y", var.toPoint().y());
        break;

    case QVariant::Rect:
        tag.setAttribute("CenterX", var.toRect().center().x());
        tag.setAttribute("CenterY", var.toRect().center().y());
        tag.setAttribute("Height", var.toRect().height());
        tag.setAttribute("Width", var.toRect().width());
        break;

    case QVariant::Line:
        tag.setAttribute("X1", var.toLine().p1().x());
        tag.setAttribute("Y1", var.toLine().p1().y());
        tag.setAttribute("X2", var.toLine().p2().x());
        tag.setAttribute("Y2", var.toLine().p2().y());
        break;

    case QVariant::Int:
        txtString = QString::number(var.toInt());
        break;

    case QVariant::String:
        txtString = var.toString();
        break;

    case QVariant::DateTime:
        tag.setAttribute("Date", var.toDate().toString(Qt::ISODate));
        tag.setAttribute("Time", var.toTime().toString(Qt::ISODate));
        break;

    default:
        ok = false;
        break;
    } // switch(type)

    if (ok)
    {
        QDomText txtNode = element->ownerDocument().createTextNode(txtString);
        tag.appendChild(txtNode);
        element->appendChild(tag);
    }
    return ok;
}

bool DDTcore::appendDomElement(QDomElement * Element, const QString & Name, const QString & Var)
{
    return false;
}


bool DDTcore::parseDomElement(QString * rtnVar, const QDomElement & elementVar)
{
    return false;
}

Return DDTcore::readXmlFile(QDomDocument * doc, const QString & fileName, const QString & docType)
{
    QFile xmlFile(fileName);

    if ( ! xmlFile.open(QIODevice::ReadOnly))
        return Return::qfileError(xmlFile);

    return read(doc, &xmlFile, docType);
} // readXmlFile()

Return DDTcore::read(QDomDocument * doc, QFile * file, const QString & docType)
{
    QString errMsg;
    int errLine, errCol;

    if ( ! doc)
        return Return(Return::NullPointer, "DDTcore::readXmlFile(QDomDocument *)");
    if ( ! file->isReadable())
        if ( ! file->open(QIODevice::ReadOnly))
            return Return::qfileError(*file);
    if ( ! doc->setContent(file, &errMsg, &errLine, &errCol))
        return Return(Return::ReturnXmlParse, file->fileName(), errMsg, errLine, errCol);
    if ( ! docType.isEmpty() && docType != doc->doctype().name())
    {
        errMsg = QObject::tr("Invalid docType name [%1], expected [%2]")
                        .arg(doc->doctype().name()).arg(docType);
        return Return(Return::ReturnXmlParse, file->fileName(), errMsg, 0, 0);
    }

    return Return();
} // read(doc,file)

Return DDTcore::write(QFile * file, const QDomDocument & doc, const QString & docType)
{
    return Return(Return::NotImplemented, "write(file,doc)");
} // write(file,doc)

Return DDTcore::writeXmlFile(const QString & fileName, const QDomDocument & doc)
{
    QByteArray ba = doc.toByteArray(2);
    QFile xmlFile(fileName);
    if ( ! xmlFile.open(QIODevice::Truncate | QIODevice::WriteOnly))
        return Return::qfileError(xmlFile);
    if (ba.size() != xmlFile.write(ba))
        return Return::qfileError(xmlFile);

    return Return();
} // writeXmlFile()

QVector<int> DDTcore::lugNuts(const int n)
{
    QQueue< QPair<int,int> > queue;
    QVector<int> result;

    result.reserve(n);
    if (0 == n)
        return result;
    result.append(0);
    if (1 == n)
        return result;
    result.append(n-1);
    if (2 == n)
        return result;

    queue.enqueue(QPair<int,int>(1, n-2));
    while ( ! queue.isEmpty())
    {
        QPair<int,int> p = queue.dequeue();

        if (p.first == p.second)
        {
            result.append(p.first);
        }
        else if (p.first == p.second - 1)
        {
            result.append(p.first);
            queue.enqueue(QPair<int,int>(p.second, p.second));
        }
        else
        {
            int c = (p.first + p.second) / 2;
            result.append(c);
            queue.enqueue(QPair<int,int>(p.first, c-1));
            queue.enqueue(QPair<int,int>(c+1, p.second));
        }
    } // while(!empty)

    return result;
} // lugNuts(n)

QStringList DDTcore::lugNuts(const QStringList qsl)
{
    QStringList result;
    QVector<int> v = lugNuts(qsl.size());
    foreach (int x, v)
        result << qsl.at(x);
    return result;
} // lugNuts(qsl)

QStringList DDTcore::propertyNames(const QObject * obj)
{
    QStringList names;
    const QMetaObject * meta = obj->metaObject();
    int n = meta->propertyCount();
    for (int i = meta->propertyOffset(); i < n; ++i)
    {
        QByteArray propertyName = meta->property(i).name();
        names << propertyName;
    }
    return names;
}

bool DDTcore::copyProperties(QObject * dst, QObject * src)
{
    const QMetaObject * metaDst = dst->metaObject();
    const QMetaObject * metaSrc = src->metaObject();
    if (metaDst != metaSrc)
        return false;

    int n = metaSrc->propertyCount();
    int k = 0;
    for (int i = metaSrc->propertyOffset(); i < n; ++i)
    {
        QByteArray propertyName = metaSrc->property(i).name();
        if (dst->setProperty(propertyName, src->property(propertyName)))
            ++k;
    }

    return k == n;
} // copyProperties()

QList<QRect> DDTcore::simplifyRoiList(QList<QRect> inputList, QRect boundingRect, QSize minSize)
{
    QList<QRect> output;

    if (inputList.isEmpty())
        output << boundingRect;
    else
    {
        while ( ! inputList.isEmpty())
        {
            QRect r1 = inputList.takeFirst() & boundingRect;
            int x2 = 1;
            while (x2 < inputList.size())
            {
                QRect r2 = inputList[x2] & boundingRect;
                if (r1.contains(r2))
                    inputList.removeAt(x2);
                else if (r1.intersects(r2))
                {
                    r1 |= r2;
                    inputList.removeAt(x2);
                }
                else
                    ++x2;
            }
            if (r1.width() >= minSize.width() && r1.height() >= minSize.height())
                output << r1;
        } // while
    }
    return output;
} // simplifyRoiList()

Return DDTcore::writeEmptyFile(QString fileName)
{
    if (fileName.isEmpty())
        return Return::errorString("DDTcore::writeEmptyFile() no file name");

    QDir dir(QFileInfo(fileName).dir());
    if ( ! dir.exists())
    {
        if ( ! dir.mkpath("."))
            return Return::errorString(QObject::tr("DDTcore::writeEmptyFile() "
                                "unable to make path %1").arg(dir.path()));
    }

    QFile f(fileName);
    if ( ! f.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return Return::qfileError(f);
    f.close();

    return Return();
}
