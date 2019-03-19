#include "spriteFly.h"
#include "cocos2d.h"
using namespace cocos2d;

USING_NS_CC;



spriteFly::spriteFly()
{
	type = 0;
	score = 0;
	getScore = 0;
	disScore = 0;
	scoreLabel = nullptr;
	beginPos = Vec2(0,0);
	endPos = Vec2(0,0);
}
spriteFly::~spriteFly()
{

	//清除元宝和金币的图片
	CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("shuihu/mainscene/gold2.plist");
	Director::getInstance()->getTextureCache()->removeTextureForKey("shuihu/mainscene/gold2.png");

}

spriteFly* spriteFly::create()
{
	spriteFly * ret = new (std::nothrow) spriteFly();
	if (ret && ret->init())
	{
		ret->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(ret);
	}
	return ret;
}


bool spriteFly::init()
{
	//加载元宝和金币的图片
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("shuihu/mainscene/gold2.plist");
	return true;
}
//创建元宝
void spriteFly::createYuanbao()
{
	int m = 0;
	for (int n = 0; n < 9; n++)
	{
		auto sp = Sprite::create("shuihu/mainscene/yuanbao.png");
		sp->setPosition(beginPos);
		this->createYuanbaoFly(sp, m);
		this->addChild(sp, 1000);
		m++;
	}
}

//创建元宝飞
void spriteFly::createYuanbaoFly(Node* sp, int m)
{
	//创建动画
	Sprite* pSprite = (Sprite*)sp;
	int x = rand() % 200;
	int y = rand() % 200;
	if (y < 80)
		y = y + 50;

	auto act1 = MoveTo::create(0.3 + m*0.05, Vec2(pSprite->getPositionX() - 120 + x, pSprite->getPositionY() + y));
	auto act2 = MoveTo::create(0.3 + m*0.02, endPos);
	Action* call = CallFuncN::create(pSprite, callfuncN_selector(spriteFly::removeSpriteFly));
	Action* call2 = CallFuncN::create(this, callfuncN_selector(spriteFly::yuanBaoChange));
	auto seq = Sequence::create(act1, act2, call, call2, NULL);
	pSprite->runAction(seq);
}

//金币放大效果
void spriteFly::yuanBaoChange(Node* obj)
{
	//添加元宝
	auto  newSp = Sprite::create("shuihu/mainscene/yuanbao.png");
	newSp->setPosition(endPos);
	this->addChild(newSp, 101);
	auto seq = Sequence::create(ScaleTo::create(0.1, 1.3), CallFunc::create([newSp](){newSp->removeFromParentAndCleanup(true); }), NULL);
	newSp->runAction(seq);
}

//创建金币
void spriteFly::createCoin()
{
	srand(unsigned(time(NULL)));
	int m = 0;
	for (int n = 0; n < 9; n++)
	{
		auto sp = Sprite::create();
		sp->setPosition(beginPos);
		this->createCoinFly(sp, m);
		this->addChild(sp, 1000);
		m++;
	}
}

void spriteFly::createCoinFly(Node* sp, int m)
{
	//创建动画
	Sprite* pSprite = (Sprite*)sp;
	int x = rand() % 200;
	int y = rand() % 200;
	if (y < 80)
		y = y + 50;
	int iCount = 6;
	CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	Vector<AnimationFrame*> animFrames;

	char str[64];
	ValueMap mapTmp;
	for (int n = rand() % 3; n <= iCount; n++)
	{
		sprintf(str, "gold2_%d.png", n);
		CCSpriteFrame* frame = cache->spriteFrameByName(str);

		if (frame)
		{
			CCAnimationFrame* aniFrame = new CCAnimationFrame();
			aniFrame->initWithSpriteFrame(frame, 1, mapTmp);
			animFrames.pushBack(aniFrame);
			aniFrame->release();
		}
	}
	cocos2d::CCAnimation *animation = CCAnimation::create(animFrames, 0.08f);

	//把animation赋予animate，使animate有生命  
	cocos2d::CCAnimate *animate = cocos2d::CCAnimate::create(animation);
	auto act1 = MoveTo::create(0.3 + m*0.05, Vec2(pSprite->getPositionX() - 120 + x, pSprite->getPositionY() + y));
	auto act2 = MoveTo::create(0.3 + m*0.02, endPos);
	Action* call = CallFuncN::create(pSprite, callfuncN_selector(spriteFly::removeSpriteFly));
	Action* call2 = CallFuncN::create(this, callfuncN_selector(spriteFly::coinChange));
	auto seq = Sequence::create(act1, act2, call, call2, NULL);
	pSprite->runAction(seq);
	pSprite->runAction(RepeatForever::create(animate));


}
void spriteFly::coinChange(Node* obj)
{
	//添加金币
	auto  newSp = Sprite::create("shuihu/mainscene/gold.png");
	newSp->setPosition(endPos);
	this->addChild(newSp, 101);
	auto seq = Sequence::create(ScaleTo::create(0.1, 1.3), CallFunc::create([newSp](){newSp->removeFromParentAndCleanup(true); }), NULL);
	newSp->runAction(seq);
}
void spriteFly::removeSpriteFly(Node* obj)
{
	Sprite* sp = (Sprite*)obj;
	sp->stopAllActions();
	sp->removeFromParent();

}
//设置金币或元宝飞出的起始和结束位置
void spriteFly::setPos(Vec2 pos1, Vec2 pos2)
{
	beginPos = pos1;
	endPos = pos2;
}

//开始播放动画
void spriteFly::startAni(unsigned int n)
{
	type = n;
	if (1 == n)
	{
		createCoin();
	}
	if (2 == n)
	{
		createYuanbao();
	}
}

//数字形成一分一分加的效果
//加分效果
void spriteFly::addscore(cocos2d::LabelAtlas* label, int totaleNum,int num)
{
	scoreLabel = label;
	score = totaleNum;
	getScore = num;

	disScore = getScore / 10;
	if (disScore < 1)   disScore = 1;
	
	this->schedule(schedule_selector(spriteFly::scoreTimer),0.1f);
}

//加分效果
void spriteFly::scoreTimer(float t)
{
	getScore -= disScore;	
	if (getScore < 0)
	{
		getScore = 0;
		
	}
	if (getScore == 0)
	{
		score += disScore;
		scoreLabel->setString(StringUtils::format("%d", score));
		this->unschedule(schedule_selector(spriteFly::scoreTimer));
	}
	if (getScore >0)
	{
		 score += disScore;
		 scoreLabel->setString(StringUtils::format("%d", score));
	}
}