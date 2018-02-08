#ifndef MATRIX_H
#define MATRIX_H

#include <QSize>
#include <QVector>

template <class T> class Matrix
{
public:
    Matrix(void) {}
    Matrix(const QSize size) { resize(size); }
    bool isEmpty(void) const { return Data.isEmpty(); }
    QSize size(void) const { return Size; }
    int height(void) const { return Size.height(); }
    int width(void) const { return Size.width(); }
    void clear(void) { resize(Size); }
    bool isValid(const int x, const int y) const
    { return x >= 0 && x < Size.width() && y >= 0 && y < Size.height(); }
    T at(const int x, const int y) const
    { return isValid(x, y) ? Data.at(index(x, y)) : T(); }
    T at(const QPoint pt) const
    { return at(pt.x(), pt.y()); }
    void set(const int x, const int y, const T t)
    {
        if (isValid(x, y))
        {
            Data.replace(index(x, y), t);
//          qDebug("Matrix::set(%i,%i,%i)", x, y, t);
        }
        else
            qDebug("Matrix::set(%i,%i) invalid", x, y);
    }
    void resize(const QSize size)
    { fill(T(), size); }
    void fill(T t, QSize size)
    {
        if (size.isValid())
        {
            Size = size;
            Data.fill(t, Size.width() * Size.height());
        }
        else
        {
            Size = QSize();
            Data.clear();
        }
    }
    T * row(const int y)
    {
        return (y >= 0 && y < Size.height())
                ? Data.data() + y * Size.width() : 0;
    }
    const T * constRow(const int y) const
    {
        return (y >= 0 && y < Size.height())
                ? Data.data() + y * Size.width() : 0;
    }
    Matrix operator += (Matrix other)
    {
        int n = Data.size();
        for (int x = 0; x < n; ++x)
            Data[x] += other.Data[x];
        return *this;
    }

private:
    int index(const int x, const int y) const
    { return y * Size.width() + x; }

private:
    QSize Size;
    QVector<T> Data;
};

#endif // MATRIX_H
