/*! \file Rectangle.hh Declarations for TRectangle template
  *
  */

template <class SIZE, class POINT, class RECT, class VALUE> class TRectangle
{
public:
    TRectangle() : size_(), center_() {}
    TRectangle(SIZE size, POINT center) :
            size_(size), center_(center) {}
    TRectangle(RECT rect) :
            size_(rect.size()), center_(rect.center()) {}
    bool isValid(void) const
    { return size_.isValid(); }
    SIZE size(void) const
    { return size_; }
    VALUE top(void) const
    { return center_.y() - size_.height() / 2; }
    VALUE left(void) const
    { return center_.x() - size_.width() / 2; }
    VALUE height(void) const
    { return size_.height(); }
    VALUE width(void) const
    { return size_.width(); }
    void scale(const qreal scale)
    { size_ *= scale, center_ *= scale; }
    TRectangle scaled(const qreal scale) const
    { TRectangle result(*this); result.scale(scale); return result; }
    void expand(const qreal scale)
    { size_ *= scale; }
    void translate(const POINT offset)
    { center_ += offset; }
    operator RECT (void) const
    { return RECT(POINT(left(), top()), size_); }

private:
    SIZE    size_;
    POINT   center_;
}; // template class TRectangle
