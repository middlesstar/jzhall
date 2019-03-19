#include "GoldDownLayer_SHZ.h"
#include "Sound/SoundManager.h"

USING_NS_CC;

#define GOLDDOWN_PLIST_SHZ	"mainscene/gold2.plist"

float SHZGoldDownLayer::m_yPos;
float SHZGoldDownLayer::m_speed = 800.0f;
float SHZGoldDownLayer::m_time;

bool SHZGoldDownLayer::init()
{
	if (!CCLayer::init())
		return false;

	// 加载plist
	CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	cache->addSpriteFramesWithFile(GOLDDOWN_PLIST_SHZ);

	CCSprite* sprite = CCSprite::createWithSpriteFrame(cache->spriteFrameByName("gold2_0.png"));
	CCSize spriteSize = sprite->getContentSize();
	CCSize winSize = CCDirector::sharedDirector()->getVisibleSize();

	m_minXPos = spriteSize.width / 2;
	m_maxXPos = winSize.width - m_minXPos;
	m_yPos = winSize.height + spriteSize.height / 2;

	m_time = (winSize.height + spriteSize.height) / m_speed;

	//setTouchEnabled(true);
	setTouchSwallow(true);

	char sSound[32] = { 0 };
	sprintf(sSound, "golddown");
	SHZSoundManager::shared()->playSound(sSound, true);
	//SoundManager::shared()->playMusic(sSound);

	// 启动调度器
	schedule(schedule_selector(SHZGoldDownLayer::addGold), 0.02f);

	return true;
}

void SHZGoldDownLayer::onEnter()
{
	CCLayer::onEnter();
}

void SHZGoldDownLayer::onExit()
{
	CCLayer::onExit();
	setTouchEnabled(false);
	//setTouchSwallow(false);
}

void SHZGoldDownLayer::addGold(float dt)
{
	CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();

	CCSprite* sprite = CCSprite::createWithSpriteFrame(cache->spriteFrameByName("gold2_0.png"));
	float xPos = CCRANDOM_0_1() * (m_maxXPos - m_minXPos) + m_minXPos;
	sprite->setPosition(CCPoint(xPos, m_yPos ));

	CCAnimation* animation = CCAnimation::create();
	animation->setDelayPerUnit(0.05f);
	char str[20];
	for (int i = 0; i <= 6; ++i)
	{
		sprintf(str, "gold2_%d.png", i);
		animation->addSpriteFrame(cache->spriteFrameByName(str));
	}

	CCAnimate* animate		= CCAnimate::create(animation);
	CCRepeatForever* repeat = CCRepeatForever::create(animate);
	CCMoveTo* move			= CCMoveTo::create(m_time, CCPoint(xPos, -sprite->getContentSize().height / 2));
	CCCallFuncN* moveDone	= CCCallFuncN::create(this, callfuncN_selector(SHZGoldDownLayer::goldMoveFinish));
	CCSequence* seq			= CCSequence::createWithTwoActions(move, moveDone);
	
	//sprite->setScale(0.6f);
	sprite->runAction(repeat);
	sprite->runAction(seq);
	addChild(sprite);
}

void SHZGoldDownLayer::goldMoveFinish(CCNode* pSender)
{
	removeChild(pSender);
}

void SHZGoldDownLayer::setTouchSwallow(bool bSwallow/* = true*/)
{
	if (bSwallow)
	{		

		m_pEventListener = EventListenerTouchOneByOne::create();
		m_pEventListener->setSwallowTouches(true);

		m_pEventListener->onTouchBegan = [this](Touch* touch, Event* event)->bool
		{
			CCLOG("GoldDownLayer Touched!");
			m_isTouched = true;
			 	// 增加对象引用计数器
			 	retain();
			 	removeFromParentAndCleanup(true);
			 
			 	unschedule(schedule_selector(SHZGoldDownLayer::addGold));
			 
			 	CCLOG("address: %d", this);
			 
				char sSound[32] = { 0 };
				sprintf(sSound, "golddown");
				SHZSoundManager::shared()->stopSound(sSound);
			return true;
		};
		getEventDispatcher()->addEventListenerWithSceneGraphPriority(m_pEventListener, this);
	}
	else
	{
		getEventDispatcher()->removeEventListener(m_pEventListener);
		m_pEventListener = nullptr;
	}
	
}


SHZGoldDownLayer::~SHZGoldDownLayer()
{
	//CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(GOLDDOWN_PLIST_SHZ);
	//Director::getInstance()->getTextureCache()->removeTextureForKey("mainscene/gold2.png");
}