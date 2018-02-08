#ifndef CIRCULARQUEUE_H
#define CIRCULARQUEUE_H
#include "eirType.h"

#include <QVector>


template <class T> class CircularQueue
{
public:
    CircularQueue(void) {}

    bool isNull(void) const { return t_vec.isEmpty(); }
    void clear(void) { t_vec.clear(); first_i = last_i = read_i = write_i = 0; }
    bool isEmpty(void) const { return read_i == write_i; }
    bool isFull(void) const { return write_i >= capacity(); }
    bool isOverflow(void) const { return size() > capacity(); }
    int size(void) const { return write_i - read_i; }
    void allocate(const int size)
    {
        int n = powerOfTwo(size);
        clear();
        t_vec.fill(T(), n);
        indexMask_i = (1 << n) - 1;
    }
    void fill(const T & t, const int size=0)
    {
        int n = size ? powerOfTwo(size) : t_vec.size();
        clear();
        t_vec.fill(t, n);
        indexMask_i = (1 << n) - 1;
    }
    int capacity(void) const { return t_vec.size(); }

    int enqueue(const T t)
    {
        int result = write_i;
        t_vec[write_i & indexMask_i] = t;
        ++write_i;
        return result;
    }
    T dequeue(void)
    {
        if (isEmpty()) return T();
        T result = t_vec[read_i & indexMask_i];
        ++read_i;
        return result;
    }

private:

private:
    QVector<T> t_vec;
    int first_i;
    int last_i;
    int read_i;
    int write_i;
    int indexMask_i;
};

#endif // CIRCULARQUEUE_H
