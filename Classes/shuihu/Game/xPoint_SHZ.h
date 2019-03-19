#ifndef SHZ__POINT_H__
#define SHZ__POINT_H__

///////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////
class SHZxPoint
{
public:
    SHZxPoint():x_(0), y_(0) {}
    SHZxPoint(float x, float y):x_(x), y_(y) {}
    SHZxPoint(const SHZxPoint &point):x_(point.x_), y_(point.y_) {}
    ~SHZxPoint() {}

public:
    void offset(float x, float y) { x_+= x; y_+=y; }

    void set_point(float x, float y) { x_=x; y_=y; }

    bool operator == (const SHZxPoint &point) const { return (x_==point.x_&&y_==point.y_); }
    bool operator != (const SHZxPoint &point) const { return (x_!=point.x_||y_!=point.y_); }

    SHZxPoint &operator = (const SHZxPoint &point) { x_=point.x_; y_=point.y_; return *this; }

    void operator += (const SHZxPoint &point) { x_+=point.x_; y_+=point.y_; }
    void operator -= (const SHZxPoint &point) { x_-=point.x_; y_-=point.y_; }

    SHZxPoint operator + (const SHZxPoint &point) { return SHZxPoint(x_+point.x_, y_+point.y_); }
    SHZxPoint operator - (const SHZxPoint &point) { return SHZxPoint(x_-point.x_, y_-point.y_); }
    SHZxPoint operator - () { return SHZxPoint(-x_, -y_); }

    SHZxPoint operator * (float multip) { return SHZxPoint(x_*multip, y_*multip); }

public:
    float x_;
    float y_;
};


inline SHZxPoint operator+ (const SHZxPoint &lhs, const SHZxPoint &rhs)
{
	return SHZxPoint(lhs.x_+rhs.x_, lhs.y_+rhs.y_);
}

inline SHZxPoint operator- (const SHZxPoint &lhs, const SHZxPoint &rhs)
{
	return SHZxPoint(lhs.x_-rhs.x_, lhs.y_-rhs.y_);
}


///////////////////////////////////////////////////////////////////////////////////////////////

#endif
