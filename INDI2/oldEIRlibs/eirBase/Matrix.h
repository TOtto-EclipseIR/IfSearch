#ifndef MATRIX_H
#define MATRIX_H

#include <QtDebug>
#include <QtCore/QVector>

#include "../eirTypes/QQSizeF.h"

template <typename T> class Matrix : protected QVector<T>
{
public:
    Matrix(void);
    Matrix(const QSize size,
           bool fixedSize=true);
    Matrix(const T & fill,
           const QSize size,
           bool fixedSize=true);
    QSize size(void) const;
    bool hasRow(const int row);
    void resize(const QSize size);
    void reserve(const QSize size);
    void fill(const T & t);
    T get(const int x, const int y);
    void set(const int x, const int y, const T t);
    void appendRow(const QList<T> & row);

private:
    int index(const int x, const int y);

private:
    bool fixedSize_bool;
    QQSizeF allocated_size;
    QQSizeF used_size;
};

template <typename T> Matrix<T>::Matrix(void)
    : fixedSize_bool(false)
{
    used_size = QSizeF(0, 0);
}

template <typename T> Matrix<T>::Matrix(const QSize size,
                                        bool fixedSize)
    : fixedSize_bool(fixedSize)
{
    reserve(size);
}

template <typename T> Matrix<T>::Matrix(const T & t,
                                        const QSize size,
                                        bool fixedSize)
    : fixedSize_bool(fixedSize)
{
    reserve(size);
    fill(t);
}
template <typename T> QSize Matrix<T>::size(void) const
{
    return used_size.toSize();
}

template <typename T> bool Matrix<T>::hasRow(const int row)
{
    return row < used_size.height();
}

template <typename T> void Matrix<T>::resize(const QSize size)
{
    //qDebug() << "Matrix<T>::resize()" << size;
    if (used_size.isValid() && fixedSize_bool)  return;
    if  ( ! allocated_size.contains(size))
    {
        QVector<T> old(*this);
        allocated_size = used_size.powerOfTwo();
        QVector<T>::resize(allocated_size.area());
        qDebug() << "allocated_size" << allocated_size;
        for (int y = 0; y < used_size.height(); ++y)
            for (int x = 0; x < used_size.width(); ++x)
                set(x, y, old.value(used_size.width() * y + x));
    }
    used_size = QSizeF(size);
}

template <typename T> void Matrix<T>::reserve(const QSize size)
{
    allocated_size = used_size.united(size);
    QVector<T>::resize(allocated_size.area());
}

template <typename T> void Matrix<T>::fill(const T & t)
{
    QVector<T>::fill(t);
}

template <typename T> T Matrix<T>::get(const int x, const int y)
{
    //qDebug() << "Matrix<T>::get()" << x << y
      //       << QVector<T>::value(index(x, y));
    return QVector<T>::value(index(x, y));
}

template <typename T> void Matrix<T>::set(const int x, const int y, const T t)
{
    //qDebug() << "Matrix<T>::set()" << x << y << t;
    if ( ! used_size.contains(x, y))
    {
        if (fixedSize_bool)
            return;
        else
            resize(used_size.united(x, y).toSize());
    }
    QVector<T>::operator[](index(x, y)) = t;
}

template <typename T> int Matrix<T>::index(const int x, const int y)
{
    return allocated_size.width() * y + x;
}

template <typename T> void Matrix<T>::appendRow(const QList<T> & row)
{
    qDebug() << "Matrix<T>::appendRow()" << row;
    int y = used_size.height();
    int n = row.size();
    for (int x = 0; x < n; ++x)
        set(x, y, row.at(x));
}

#endif // MATRIX_H
