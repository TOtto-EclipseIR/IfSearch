#include <Detector.h>
#include <InfoMacros.h>
#include <QQRect.h>

#include <QMapIterator>

#include <math.h>



void DetectorResult::addToAverage(QRect Rect)
{
    allRects.append(Rect);
    ptAverage.setX(ptAverage.x() + Rect.x());
    ptAverage.setY(ptAverage.y() + Rect.y());
    szAverage.setWidth(szAverage.width() + Rect.width());
    szAverage.setHeight(szAverage.height() + Rect.height());
    qreal n = allRects.size();
    rect.setX(ptAverage.x() / n);
    rect.setY(ptAverage.y() / n);
    rect.setWidth(szAverage.width() / n);
    rect.setHeight(szAverage.height() / n);
} // addToAverate()

bool HaarDetector::isStrange(QQRect rect)
{
    bool rtn = false;

    if (rect.aspect() < minAspect() || rect.aspect() > maxAspect())
        rtn = true;
    else if (rect.width() > maxObjectSize().width() * origScale
             || rect.height() > maxObjectSize().height() * origScale)
        rtn = true;
    else if (rect.center().x() < 0 || rect.center().x() > scaled_size.width() * origScale)
        rtn = true;
    else if (rect.center().y() < 0 || rect.center().y() > scaled_size.height() * origScale)
        rtn = true;
    if (rtn)
        ++strangeObjects;
    return rtn;
}

void HaarDetector::groupObjects(bool returnAll)
{
    rawObjects = allObjects.size();
    strangeObjects
            = 0;
    switch (GroupMethod ? GroupMethod : DefaultGroupMethod)
    {
    case GroupByCenters:		groupByCenters(returnAll);		break;
    case GroupByNeighbors:		groupByNeighbors(returnAll);		break;
    case GroupInternal:
    case GroupInternalAllObjects:	groupInternal(returnAll);		break;
    default:
    case GroupByOverlap:		groupByOverlap(returnAll);		break;
    }
}

void HaarDetector::groupByOverlap(bool returnAll)
{
    DETAIL("GroupOverlap pass 1");
    // 1. Initialize parameters
    QMultiMap<int, QQRect> sortByArea;
    qreal fOverlap = (OverlapThreshold < 1 || OverlapThreshold > 99) ? 0.75 : (OverlapThreshold / 100.0);

    DETAIL("GroupOverlap pass 2");
    // 2. Iterate allObjects, elimate strageness, load sort map
    foreach (QQRect rect, allObjects)
        if ( ! isStrange(rect))
            sortByArea.insert(rect.area(), rect);

    DETAIL("GroupOverlap pass 3");
    // 3. Iterate down by area
    allResults.clear();
    QMutableMapIterator<int, QQRect> it(sortByArea);
    it.toBack();
    while (it.hasPrevious())
    {
        // 3a. Take largest remaining to start next result
        it.previous();
        QQRect rect1 = it.value();
        it.remove();
        DetectorResult ri(rect1);

        // 3b. Iterate remaining
        while (it.hasPrevious())
        {
            // 3c. If next remaining meets overlap threshold
            it.previous();
            QQRect rect2 = it.value();
            if (rect2.overlapArea(ri.rect) > fOverlap * rect2.area())
            {
                it.remove();
                ri.addRect(rect2);
            }
        } // while(it)
        rect1 = ri.rect;
        rect1.matchAspect(DetectorSize);
        ri.rect = rect1;

        // 3c. Calculate score and add to results
        double score = 10.0 * sqrt(groupFactor()) * sqrt((double)ri.allRects.size());
        // TODO? divide by width?
        allResults.insert(score, ri);

        it.toBack(); // start over with largest
    } // while(it)

    handleResults(returnAll);
    DETAIL("GroupOverlap done");
} // groupByOverlap()

void HaarDetector::groupByNeighbors(bool returnAll)
{
    DETAIL("GroupNeighbors pass 1");
    // 1. Initialize parameters
    QMultiMap<int, QQRect> sortByArea;
    qreal fNeighbor = (NeighborThreshold < 1 || NeighborThreshold > 99)
            ? 0.33 : (NeighborThreshold / 100.0);

    DETAIL("GroupNeighbors pass 2");
    // 2. Iterate allObjects, elimate strageness, load sort map
    foreach (QQRect rect, allObjects)
        if ( ! isStrange(rect))
            sortByArea.insert(rect.area(), rect);

    DETAIL("GroupNeighbors pass 3");
    // 3. Iterate down by area
    allResults.clear();
    QMutableMapIterator<int, QQRect> it(sortByArea);
    it.toBack();
    while (it.hasPrevious())
    {
        // 3a. Take largest remaining to start next result
        it.previous();
        QQRect rect1 = it.value();
        it.remove();
        DetectorResult ri(rect1);

        // 3b. Iterate remaining
        while (it.hasPrevious())
        {
            // 3c. If next remaining meets overlap threshold
            it.previous();
            QQRect rect2 = it.value();
            if (QQRect(ri.rect).isNeighbor(rect2, fNeighbor))
            {
                it.remove();
                ri.addToAverage(rect2);
            }
        } // while(it)
        rect1 = ri.rect;
        rect1.matchAspect(DetectorSize);
        ri.rect = rect1;

        // 3c. Calculate score and add to results
        double score =  5.0 * groupFactor() * sqrt((double)ri.allRects.size());
        // TODO? divide by width?
        allResults.insert(score, ri);

        it.toBack(); // start over with largest
    } // while(it)

    handleResults(returnAll);
    DETAIL("GroupNeighbors done");
}

void HaarDetector::groupInternal(bool returnAll)
{
    handleResults(returnAll);
}

void HaarDetector::groupByCenters(bool returnAll)
{
    DETAIL("GroupCenters pass 1");
    // 1. Initialize parameters
    QSize szMinObj = minObjectSize();
    //	QSize szMaxObj = maxObjectSize();
    //	qreal minAsp = minAspect();
    //	qreal maxAsp = maxAspect();
    QList<QPoint> bins;
    QList<QList<QRect> > rects;
    QMultiMap<int, QPoint> sort;

    DETAIL("GroupCenters pass 2");
    // 2. Iterate allObjects
    foreach (QRect rect, allObjects)
    {
        if (isStrange(rect))
            continue;

        QPoint ptBin = QPoint(rect.center().x() / szMinObj.width(),
                              rect.center().y() / szMinObj.height());
        int x = bins.indexOf(ptBin);
        if (x < 0)
        {
            bins.append(ptBin);
            QList<QRect> l;
            l << rect;
            rects.append(l);
        }
        else
        {
            rects[x].append(rect);
        }
    }

    DETAIL("GroupCenters pass 3");
    // 3. Iterate centers
    for (int i = 0; i < bins.size(); i++)
        if ( ! rects[i].isEmpty())
            sort.insert(rects[i].size(), bins[i]);

    DETAIL("GroupCenters pass 4");
    // 4. Iterate sort
    QMapIterator<int,QPoint> itSort(sort);
    itSort.toBack();
    while (itSort.hasPrevious())
    {
        itSort.previous();
        QPoint pt = itSort.value();
        int ix = bins.indexOf(pt);
        QList<QRect> list = rects.at(ix);
        if (list.isEmpty())
            continue; // TODO: Why?

        // calculate average center
        int ctrX = 0, ctrY = 0;
        foreach(QRect r, list)
            ctrX += r.center().x(), ctrY += r.center().y();
        ctrX /= list.size(), ctrY /= list.size();

        // combine with adjacent cells
        for (int x = -1; x <= +1; x++)
            for (int y = -1; y <= +1; y++)
                if (x || y)
                {
                    QPoint pt2 = pt + QPoint(x, y);
                    int ix2 = bins.indexOf(pt2);
                    if (ix2 < 0)
                        continue;
                    QList<QRect> list2 = rects.at(ix2);
                    if (list2.size() > list.size())
                        continue;
                    foreach(QRect r, list2)
                    {
                        if (qAbs(r.center().x() - ctrX) < szMinObj.width()
                                && qAbs(r.center().y() - ctrY) < szMinObj.height())
                        {
                            rects[ix].append(r);
                            rects[ix2].removeOne(r);
                        }
                    }
                } // if, for(y), for(x)
    } // while(itSort)

    DETAIL("GroupCenters pass 5");
    // 5. Iterate centers again
    sort.clear();
    for (int i = 0; i < bins.size(); i++)
        if ( ! rects[i].isEmpty())
            sort.insert(rects[i].size(), bins[i]);

    DETAIL("GroupCenters pass 6");
    // 6. Iterate sort again
    QMapIterator<int,QPoint> itSort2(sort);
    itSort2.toBack();
    while (itSort2.hasPrevious())
    {
        itSort2.previous();
        QPoint pt = itSort2.value();
        int ix = bins.indexOf(pt);
        QList<QRect> list = rects.at(ix);
        while ( ! list.isEmpty())
        {
            QQRect rect = list.takeFirst();
            int area = rect.area();
            int k = 1;
            double score = sqrt((double)rect.width());
            DetectorResult ri;
            ri.allRects.append(rect);

            QMutableListIterator<QRect> it(list);
            while (it.hasNext())
            {
                it.next();
                QQRect qqr(it.value());
                int a = qqr.area();
                area += a, k++, score += sqrt((double)it.value().width());
                rect = rect.united(it.value());
                ri.allRects.append(it.value());
                it.remove();
            }
            rect.matchAspect(DetectorSize);
            ri.rect = rect;
            allResults.insert(score / 5.0, ri);
        } // while(!empty)
    } // while(itSort2)

    handleResults(returnAll);
} // groupByCenters()

void HaarDetector::handleResults(bool returnAll)
{
    // Deal with constraint properties
    DETAIL("Handle Detector Results");
    results.clear();
    int k = 0;
    QMapIterator<double, DetectorResult> it(allResults);
    it.toBack();
    while (it.hasPrevious())
    {
        it.previous();
        int scaled = scaleScore(it.key());
        DetectorResult ri = it.value();
        if (MinQuality && scaled < MinQuality && ! returnAll)
            break;
        if (MaxResults && results.size() >= MaxResults && ! returnAll)
            break;
        if (ri.allRectangles().size() < 2)
            continue;
        if (Adjust)
            ri.rect = doAdjust(ri.rect);
        ri.k = ++k;
        ri.sco = scaled;
        results.insert(scaled, ri);
    }
    if (ForceFind && results.isEmpty())
    {
        DETAIL("ForceFind");
        it.toBack();
        while (it.hasPrevious())
        {
            it.previous();
            int scaled = scaleScore(it.key());
            if (MaxResults && results.size() >= MaxResults && ! returnAll)
                break;
            DetectorResult ri = it.value();
            ri.k = 0;
            ri.sco = scaled;
            results.insert(scaled, ri);
        }
    }
    if (ForceFind && results.isEmpty())
    {
        DETAIL("ForceFind on steroids");
        foreach (QRect rect, allObjects)
        {
            if (MaxResults && results.size() >= MaxResults && ! returnAll)
                break;
            DetectorResult ri(rect);
            ri.k = 0;
            ri.sco = 1;
            results.insert(1, ri);
        }
    }
    DETAIL("%1 results handled", results.size());
} // handleResults()
