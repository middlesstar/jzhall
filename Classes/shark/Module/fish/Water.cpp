#include "Water.h"
#include "df/DF.h"

#define SUNLIGHT  "images/shak/sunLight/sunlight.plist"      
using namespace std;


void cacheAnimation(ValueMap& info)
{
	SpriteFrameCache *cache = SpriteFrameCache::getInstance();
	ValueMap valMap;

	string name = info["name"].asString();
	string format = info["format"].asString();
	float delay = info["delay"].asFloat();
	ValueVector frames = info["frames"].asValueVector();
	int count = (int)frames.size();

	Vector<AnimationFrame* > animFrames;

	string str;
	for (int k = 0; k < count; k++)
	{
		string sFrame = frames.at(k).asString();
		int frame = -1;
		float frameDelay = 0;
		sscanf(sFrame.c_str(), "%d,%f", &frame, &frameDelay);
		str = stringFormat(format, frame);
		SpriteFrame *sptFrame = cache->getSpriteFrameByName(str.c_str());
		CCAssert(sptFrame != 0, str.c_str());

		if (sptFrame)
		{
			AnimationFrame* aniFrame = AnimationFrame::create(sptFrame, frameDelay, valMap);
			animFrames.pushBack(aniFrame);
		}
	}

	Animation *animation = Animation::create(animFrames, delay);
	AnimationCache::getInstance()->addAnimation(animation, name);
}
CWater::CWater()
{
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(SUNLIGHT);
}


CWater::~CWater()
{
	m_spWater->stopAllActions();
	stopAllActions();
	removeAllChildren();
	

	CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("images/shak/sunLight/sunlight.plist");
	Director::getInstance()->getTextureCache()->removeTextureForKey("images/shak/sunLight/sunlight.png");
}


bool CWater::init()
{
	do
	{
		CC_BREAK_IF(!Layer::init());
		cacheSunAni();
		m_pWater = RepeatForever::create(createSunAni("sunlight"));
		m_spWater = Sprite::create();
		addChild(m_spWater);

		float w = 256;
		float h = 256;
		int row = 1074 / h + 1;
		int col = 740 / w + 1;
		for (int i = 0; i < row; ++i)
		{
			for (int j = 0; j < col; ++j)
			{
				auto m_spWaterClone = Sprite::create();
				addChild(m_spWaterClone);
				m_spWaterClone->setAnchorPoint(Point(0, 0));
				auto m_pWaterAniClone = m_pWater->clone();
				m_spWaterClone->runAction(m_pWaterAniClone);
				m_spWaterClone->setPosition(Point(j * h, i * w));
			}
		}

		return true;
	} while (0);
	return false;
}

Animate* CWater::createSunAni(const char* name)
{
	Animation* animation = AnimationCache::getInstance()->getAnimation(name);
	if (animation == 0)
		return 0;

	return Animate::create(animation);
}

bool CWater::cacheSunAni()
{
	ValueVector arr = FileUtils::getInstance()->getValueVectorFromFile("images/shak/sunLight/animation.plist");
	if (arr.empty())
		return false;

	int arrCount = (int)arr.size();
	for (int i = 0; i < arrCount; i++)
	{
		ValueMap data = arr.at(i).asValueMap();
		::cacheAnimation(data);
	}

	return true;
}
//清除所有的阳光
void CWater::removeAllLight()
{
}
