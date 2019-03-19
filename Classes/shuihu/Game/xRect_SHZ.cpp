#include "xRect_SHZ.h"

///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////
SHZxRect::SHZxRect()
:top_(0),
bottom_(0),
left_(0),
right_(0)
{
}

SHZxRect::SHZxRect(const SHZxPoint &point, const SHZxSize &size)
:top_(point.y_),
bottom_(point.y_ + size.height_),
left_(point.x_),
right_(point.x_ + size.width_)
{
}

SHZxRect::SHZxRect(float left, float top, float right, float bottom)
:top_(top),
bottom_(bottom),
left_(left),
right_(right)
{
}

SHZxRect::SHZxRect(const SHZxRect& rect)
:top_(rect.top_),
bottom_(rect.bottom_),
left_(rect.left_),
right_(rect.right_)
{
}

void SHZxRect::set_position(const SHZxPoint& pt)
{
	SHZxSize sz(get_size());

	left_ = pt.x_;
    top_  = pt.y_;

	set_size(sz);
}

void SHZxRect::set_rect(float left, float top, float right, float bottom)
{
    top_ = top;
    bottom_ = bottom;
    left_ = left;
    right_ = right;
} 

void SHZxRect::set_rect(const SHZxPoint &point, const SHZxSize &size)
{
    top_ = point.y_;
    bottom_ = point.y_ + size.height_;
    left_ = point.x_;
    right_ = point.x_ + size.width_;
} 

void SHZxRect::offset_rect(float x, float y)
{
    top_ += y;
    bottom_ += y;
    left_ += x;
    right_ += x;
}

void SHZxRect::offset_rect(const SHZxSize &size)
{
    top_ += size.height_;
    bottom_ += size.height_;
    left_ += size.width_ ;
    right_ += size.width_ ;
}

bool SHZxRect::pt_in_rect(const SHZxPoint& point) const
{
	if ((left_ > point.x_) ||
		(right_ <= point.x_) ||
		(top_ > point.y_) ||
		(bottom_ <= point.y_))
	{
		return false;
	}

	return true;
}

SHZxRect SHZxRect::intersection_rect(const SHZxRect& rect) const
{
	if ((right_ > rect.left_) &&
		(left_ < rect.right_) &&
		(bottom_ > rect.top_) &&
		(top_ < rect.bottom_))
	{
		SHZxRect temp;

		temp.left_ = (left_ > rect.left_) ? left_ : rect.left_;
		temp.right_ = (right_ < rect.right_) ? right_ : rect.right_;
		temp.top_ = (top_ > rect.top_) ? top_ : rect.top_;
		temp.bottom_ = (bottom_ < rect.bottom_) ? bottom_ : rect.bottom_;

		return temp;
	}
	else
	{
		return SHZxRect(0.0f, 0.0f, 0.0f, 0.0f);
	}

}

//xRect xRect::union_rect(const xRect& rect) const 
//{
//}
//
//xRect xRect::subtract_rect(const xRect& rect) const 
//{
//}




	

///////////////////////////////////////////////////////////////////////////////////////////////
