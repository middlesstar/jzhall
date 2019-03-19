#ifndef SHZ__SIZE_H__
#define SHZ__SIZE_H__

/////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////
class SHZxSize 
{
public:
    SHZxSize():width_(0), height_(0)  { }
    SHZxSize(float width, float height):width_(width), height_(height) {}
    SHZxSize(const SHZxSize &size): width_(size.width_), height_(size.height_) {}
    ~SHZxSize() {}

    bool operator == (const SHZxSize &size) const { return (size.width_==width_&&size.height_==height_); }
    bool operator != (const SHZxSize &size) const { return (size.width_!=width_||size.height_!=height_); }

    SHZxSize &operator = (const SHZxSize &size) { width_=size.width_; height_=size.height_; return *this; }

    SHZxSize& operator += (const SHZxSize &size) { width_+=size.width_; height_+=size.height_; }
    SHZxSize& operator -= (const SHZxSize &size) { width_-=size.width_; height_-=size.height_; }

    void set_size(float width, float height) { width_=width; height_=height; }

    SHZxSize operator + (const SHZxSize &size) { return SHZxSize(width_+size.width_, height_+size.height_); }
    SHZxSize operator - (const SHZxSize &size) { return SHZxSize(width_-size.width_, height_-size.height_); }
    SHZxSize operator - () { return SHZxSize(-width_, -height_); }

public:
    float width_;
    float height_;
};

/////////////////////////////////////////////////////////////////////////////////////////

#endif
