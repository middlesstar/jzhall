#ifndef SHZ__SpriteHelper_H_
#define SHZ__SpriteHelper_H_
#include "cocos2d.h"


class SHZSpriteHelper
{
public:
	static bool cacheAnimations();
	static bool cacheAnimation(const char* plist);
	static void removeAnimation(const char* plist);
	static cocos2d::CCAnimate* createAnimate(const char* name);
	static cocos2d::CCAnimate* createBirdAnimate(int type);
	static cocos2d::CCAnimate* createBirdDeadAnimate(int type);

}; // SpriteHelper
#endif // _SpriteHelper_H_