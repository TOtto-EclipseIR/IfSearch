#ifndef FIXEDPOINT_H
#define FIXEDPOINT_H

template <typename FLOAT,
          typename INT,
          INT BASE> class FixedPoint
{
public:
    FixedPoint(void) : i(0) {}
private:
    INT i;
};

#endif // FIXEDPOINT_H
