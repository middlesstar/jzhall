#ifndef __GlobalTickObject_H__
#define __GlobalTickObject_H__

#include "cocos2d.h"
USING_NS_CC;


class GlobalTickObject : public Ref
{
public:
	GlobalTickObject();
	~GlobalTickObject();

	static GlobalTickObject* getInstance();
public:
	void gameTick(float dt);
};

#endif // __GlobalTickObject_H__
