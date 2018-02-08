#ifndef SPARSEMATRIX_H
#define SPARSEMATRIX_H

#include <QMap>
#include <QPoint>

template <class T> class SparseMatrix
{

private:
    QSize   used_sz;
    QMap<QPoint, T> _pt_t_map;
};

#endif // SPARSEMATRIX_H
