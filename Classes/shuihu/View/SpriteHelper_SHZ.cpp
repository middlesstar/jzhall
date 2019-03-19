#include "SpriteHelper_SHZ.h"
#include "shuihu/DictionaryAide_SHZ.h"

USING_NS_CC;

void cachAnimation_SHZ(CCDictionary* info)
{
	CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	SHZDictionaryAide aide(info);

	const char* name	= aide.stringFromKey("name");
	const char* format	= aide.stringFromKey("format");
	float delay			= aide.floatFromKey("delay");
	CCArray* frames		= aide.arrayFromKey("frames");
	int count			= frames == 0 ? 0 : frames->count();
	CCAssert(frames!=0, "frames is null");

	//CCArray* animFrames = CCArray::createWithCapacity(count);
	Vector<AnimationFrame* >animFrames(count);
	char str[80];

	ValueMap tmpMap;
	for(int k = 0; k < count; k++) 
	{
		const char* sFrame = ((CCString*)frames->objectAtIndex(k))->getCString();
		int frame = -1;
		float frameDelay = 0;
		sscanf(sFrame, "%d,%f", &frame, &frameDelay);
		sprintf(str, format, frame);
		CCSpriteFrame *sptFrame = cache->spriteFrameByName(str);
		CCAssert(sptFrame!=0, str);

		if (sptFrame)
		{
			CCAnimationFrame* aniFrame = new CCAnimationFrame();
			aniFrame->initWithSpriteFrame(sptFrame, frameDelay, tmpMap);
			animFrames.pushBack(aniFrame);
			//animFrames->addObject(aniFrame);
			aniFrame->release();
		}
	}

	CCAnimation *animation = CCAnimation::create(animFrames, delay);//需要修改
	CCAnimationCache::sharedAnimationCache()->addAnimation(animation, name);
}

cocos2d::CCAnimate* SHZSpriteHelper::createAnimate(const char* name)
{
	CCAnimation* animation	= CCAnimationCache::sharedAnimationCache()->animationByName(name);
	if (animation == 0)
		return 0;

	return CCAnimate::create(animation);
}

CCAnimate* SHZSpriteHelper::createBirdAnimate(int type)
{
	char str[80];
	sprintf(str, "bird%d_move", type);
	return createAnimate(str);
}

CCAnimate* SHZSpriteHelper::createBirdDeadAnimate(int type)
{
	char str[80];
	sprintf(str, "bird%d_dead", type);
	return createAnimate(str);
}


bool SHZSpriteHelper::cacheAnimations()
{
	CCArray* arr = CCArray::createWithContentsOfFile("animations.plist");
	if (arr == 0)
		return false;

	for (int i = 0, l = arr->count(); i < l; ++i)
	{
		const char* name = ((CCString*)arr->objectAtIndex(i))->getCString();
		char plist_name[64]={0};
		sprintf(plist_name, "%s.plist", name);
		CCArray* arr2 = CCArray::createWithContentsOfFile(plist_name);

		for (int j = 0, jl = (int)arr2->count(); j < jl; ++j)
		{
			CCDictionary* ani = (CCDictionary*)arr2->objectAtIndex(j);
			CCAssert(ani!=0, name);
			
			if (ani == 0)
				continue;

			::cachAnimation_SHZ(ani);
		}
	}
	
	return true;
}

const char* getPath_SHZ(char szPath[], int len, const char* file)
{
	const char* ppos = strrchr(file, '/');
	int index = ppos - file;
	if (index > len)
		index = len;
	memcpy(szPath, file, index);
	return szPath;
}

bool SHZSpriteHelper::cacheAnimation(const char* plist)
{
	CCDictionary* dic = CCDictionary::createWithContentsOfFile(plist);
	if (dic == 0)
		return false;

	CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	SHZDictionaryAide aide(dic);

	const char* name	= aide.stringFromKey("name");
	const char* format	= aide.stringFromKey("format");
	float delay			= aide.floatFromKey("delay");
	CCArray* frames		= aide.arrayFromKey("frames");
	int count			= frames == 0 ? 0 : frames->count();
	CCAssert(frames!=0, "frames is null");

	//CCArray* animFrames = CCArray::createWithCapacity(count);
	Vector<AnimationFrame* >animFrames(count);
	ValueMap tmpMap;
	char str[80];
	for(int k = 0; k < count; k++) 
	{
		const char* sFrame = ((CCString*)frames->objectAtIndex(k))->getCString();
		int frame = -1;
		float frameDelay = 0;
		sscanf(sFrame, "%d,%f", &frame, &frameDelay);
		sprintf(str, format, frame);
		CCSpriteFrame *sptFrame = cache->spriteFrameByName(str);
		CCAssert(sptFrame!=0, str);

		if (sptFrame)
		{
			CCAnimationFrame* aniFrame = new CCAnimationFrame();
			aniFrame->initWithSpriteFrame(sptFrame, frameDelay, tmpMap);
			animFrames.pushBack(aniFrame);
			//animFrames->addObject(aniFrame);
			aniFrame->release();
		}
	}

	CCAnimation *animation = CCAnimation::create(animFrames, delay);//需要修改
	CCAnimationCache::sharedAnimationCache()->addAnimation(animation, name);
	dic->release();
	return true;
}

void SHZSpriteHelper::removeAnimation(const char* plist)
{
	CCDictionary* dic = CCDictionary::createWithContentsOfFile(plist);
	if (dic == 0)
		return ;

	CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	SHZDictionaryAide aide(dic);
	CCAnimationCache::sharedAnimationCache()->removeAnimationByName(aide.stringFromKey("name"));
	dic->release();
}