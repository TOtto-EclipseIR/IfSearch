#ifndef MATRIX_H
#define MATRIX_H

#include <QSize>
#include <QVector>

typedef <class T> class Matrix
{

private:
    QSize       alloc_sz;
    QSize       used_sz;
    QVector<T>  _vector;
};

#endif // MATRIX_H
