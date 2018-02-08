/*! \file Histogram.h Declarations for Histogram class
  *
  */
#ifndef HISTOGRAM_H
#define HISTOGRAM_H


#include <QtCore/QList>
#include <QtCore/QMultiMap>
#include <QtCore/QVector>
#include <QtCore/QReadWriteLock>

/*! \class  Histogram
  * \brief  Histogram with specified number of bins
  * \tparam BIN    basic unsigned integer type for bin index (uint8 or unsigned)
  * \todo   Ensure BIN template parameter is unsigned
  */
template <class BIN> class Histogram
{
public:
    Histogram(const unsigned numBins=0, const bool update=true)
        :	numBins(numBins), updatesEnabled(update), tot(0) { clear(); }
    Histogram<BIN>(const Histogram<BIN> & other)
    {
        QWriteLocker wLock(&lock);
        {
            //QReadLocker rLock(&other.lock_); // can't lock const other???
            numBins = other.numBins,
            bins = other.bins,
            tot = other.tot;
        }
        rebuild();
    }
    ~Histogram<BIN>() {}

    void clear(void)
    { QWriteLocker wlock(&lock);	bins.fill(0, numBins), sort.clear(), tot = 0; }
    bool isEmpty(void)
    { QReadLocker rlock(&lock);	return 0 == tot; }
    void disableUpdates(void)
    { QWriteLocker wlock(&lock);	updatesEnabled = false, sort.clear(); }
    void enableUpdates(void)
    { QWriteLocker wlock(&lock);	updatesEnabled = true, rebuild(); }
    unsigned countAt(BIN bin)
    { QReadLocker rlock(&lock); return (bin < numBins) ? bins.at(bin) : 0; }
    unsigned total(void)
    { QReadLocker rlock(&lock); return tot; }

    Histogram<BIN> & operator = (const Histogram<BIN> & other)
                                {
        QWriteLocker wLock(&lock);
        {
            //QReadLocker rLock(&other.lock_); // can't lock const other???
            numBins = other.numBins,
            bins = other.bins,
            tot = other.tot;
        }
        rebuild();
        return *this;
    }

    void increment(BIN bin)
    {
        if (bin < numBins)
        {
            QWriteLocker wlock(&lock);
            if (updatesEnabled)
                sort.remove(bins[bin], bin);
            bins[bin]++;
            ++tot;
            if (updatesEnabled)
                sort.insert(bins[bin], bin);
        }
    }

    BIN sortBin(unsigned pos)
    {
        QReadLocker rlock(&lock);
        QMapIterator<unsigned, BIN> it(sort);
        it.toBack();
        BIN bin = 0;
        while (it.hasPrevious() && pos--)
        {
            it.previous();
            bin = it.value();
        }
        return bin;
    }

    void combine(BIN intoBin, BIN fromBin);

    void combine(BIN intoBin, QVector<BIN> fromBins)
    {
        QWriteLocker wlock(&lock);
        if (updatesEnabled)
        {
            foreach (BIN bin, fromBins)
                if (bin < numBins)
                    sort.remove(bins[bin], bin);
            if (intoBin < numBins)
                sort.remove(bins[intoBin], intoBin);
        }
        unsigned sum = 0;
        foreach (BIN bin, fromBins)
            if (bin < numBins)
                sum += bins[bin];
        if (intoBin >= 0 && intoBin < numBins)
        {
            bins[intoBin] += sum;
            if (updatesEnabled)
                sort.insert(bins[intoBin], intoBin);
        }
    }

private:
    void rebuild(void)
    {
        // already locked by enableUpdates()
        sort.clear();
        for (BIN bin=0; bin < numBins; ++bin)
        {
            unsigned count = bins.at(bin);
            if (count)
                sort.insert(count, bin);
        }
    }

private:
    unsigned numBins;
    QVector<unsigned> bins;
    QMultiMap<unsigned, BIN> sort;
    bool updatesEnabled;
    unsigned tot;
    QReadWriteLock lock;
};

#endif // HISTOGRAM_H
