#ifndef SHZ__RECT_H__
#define SHZ__RECT_H__

///////////////////////////////////////////////////////////////////////////////////////////////
#include "xPoint_SHZ.h"
#include "xSize_SHZ.h"

///////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////
class  SHZxRect 
{
public:
    SHZxRect();
    SHZxRect(const SHZxPoint &point, const SHZxSize &size);
    SHZxRect(float left, float top, float right, float bottom);
    SHZxRect(const SHZxRect& rect);

    SHZxPoint get_position() const { return SHZxPoint(left_, top_); }
    void set_position(const SHZxPoint& pt);

    float get_width() const { return right_ - left_; }
    void set_width(float width) { right_ = left_ + width; }

    float get_height() const {return bottom_ - top_; }
    void set_height(float height) { bottom_ = top_ + height;}

    SHZxSize get_size() const { return SHZxSize(get_width(), get_height());}
    void set_size(const SHZxSize& size)	{ set_width(size.width_); set_height(size.height_); }

    void set_rect(float left, float top, float right, float bottom); 
    void set_rect(const SHZxPoint &point, const SHZxSize &size); 

    void offset_rect(float x, float y);
    void offset_rect(const SHZxSize &size);

    bool pt_in_rect(const SHZxPoint& point) const;
    bool is_empty() const { return (get_width() == 0.0f && get_height() == 0.0f); }

    SHZxRect intersection_rect(const SHZxRect& rect) const;
    //xRect union_rect(const xRect& rect) const ;
    //xRect subtract_rect(const xRect& rect) const ;

    SHZxRect& operator = (const SHZxRect& rhs) 
    {
	    left_ = rhs.left_;
	    top_ = rhs.top_;
	    right_ = rhs.right_;
	    bottom_ = rhs.bottom_;

	    return *this;
    }

    bool operator == (const SHZxRect& rhs) const 	
    {
		return ((left_ == rhs.left_) && (right_ == rhs.right_) && (top_ == rhs.top_) && (bottom_ == rhs.bottom_));
	}
    bool operator != (const SHZxRect& rhs) const { return !operator==(rhs); }

public:
    float left_, top_, right_, bottom_;
};

///////////////////////////////////////////////////////////////////////////////////////////////

#endif