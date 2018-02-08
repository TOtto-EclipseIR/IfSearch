#ifndef FIXEDPOINT_H
#define FIXEDPOINT_H

template <typename INT,
          typename FLOAT,
          INT BASE> class FixedPoint
{
public:
    FixedPoint(void) : i(0) {}
    FixedPoint(const FLOAT f) : i(f * BASE) {}
    operator FLOAT (void) { return (FLOAT)(i) / (FLOAT)BASE; }
    FixedPoint<INT, FLOAT, BASE> operator += (FixedPoint<INT, FLOAT, BASE> other) { i += other.i; return *this; }

private:
    INT i;
};

#endif // FIXEDPOINT_H
