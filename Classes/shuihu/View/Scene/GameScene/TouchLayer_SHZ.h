#ifndef SHZ_TouchLayer_H_
#define SHZ_TouchLayer_H_

#include "cocos2d.h"

class SHZITouchSink
{
public:
	virtual ~SHZITouchSink(){};
};

class SHZTouchLayer
	: public cocos2d::CCLayer
{
public:
	static SHZTouchLayer* create(int priority = 1);   

public:
	SHZTouchLayer();
	~SHZTouchLayer();
	bool init(int priority = 1);
	void setSink(SHZITouchSink* pITouchSink);
	void setTouchEnabled(bool flag);


private:
	bool		mIsTouchEnabled;
	SHZITouchSink*	mITouchSink;
	int mPriority;

	cocos2d::EventListenerTouchOneByOne* m_pEventListener;
};

#endif // _TouchLayer_H_