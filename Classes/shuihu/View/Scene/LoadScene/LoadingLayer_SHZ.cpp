#include "shuihu/StaticData_SHZ.h"
#include "df/StringData.h"
#include "df/LocalStore.h"
#include "shuihu/MTNotification_SHZ.h"
#include "shuihu/ViewHeader_SHZ.h"
#include "LoadingLayer_SHZ.h"
#include "Player/Player.h"

//#include <pthread.h>
//#include <support/zip_support/ZipUtils.h>

USING_NS_CC;

#define PLIST_LOADSCENE	"loadscene/loadscene.plist"
#define TEX_LOADSCENE	"loadscene/loadscene.png"

#define TIPS			"tips"
#define TIPS_INDEX		"tips_index"

char szNickNameTmp_SHZ[LEN_NICKNAME] = { 0 };								//用户的昵称

CCScene* SHZLoadingLayer::scene()
{
	CCScene* scene = CCScene::create();
	 
	if (!scene)
		return 0;

	SHZLoadingLayer* layer = SHZLoadingLayer::create();
	if (layer)
		scene->addChild(layer);
	return scene;
}

SHZLoadingLayer::SHZLoadingLayer()
	: mLoadedNum(0)
	, mTotalNum(0)
	, mResources(0)	 //通过IP和端口初始化LoginMission对象;
{
}


SHZLoadingLayer::~SHZLoadingLayer()
{

	CCSpriteFrameCache::getInstance()->removeSpriteFramesFromFile(PLIST_LOADSCENE);
	Director::getInstance()->getTextureCache()->removeTextureForKey(TEX_LOADSCENE);
	CC_SAFE_RELEASE_NULL(mResources);
}

//初始化方法
bool SHZLoadingLayer::init()
{
	do 
	{
		ZipUtils::ccSetPvrEncryptionKeyPart(0 , 0x11111111);
		ZipUtils::ccSetPvrEncryptionKeyPart(1 , 0x22222222);
		ZipUtils::ccSetPvrEncryptionKeyPart(2 , 0x33333333);
		ZipUtils::ccSetPvrEncryptionKeyPart(3 , 0x44444444);

		CC_BREAK_IF(!CCLayer::init());
		CC_BREAK_IF(!this->setUpdateView());

		return true;
	} while (0);

	return false;
}


// 初始化组件
bool SHZLoadingLayer::setUpdateView()
{
	do 
	{	
		float bgX = kRevolutionWidth_SHZ/2;
		float bgY = kRevolutionHeight_SHZ/2;
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(PLIST_LOADSCENE);

		////创建背景
		CCSprite* bg = CCSprite::createWithSpriteFrameName("background.png");
		bg->setPosition(Vec2(bgX, bgY));
		addChild(bg);

		//创建进度条
		mBar = CCSprite::createWithSpriteFrameName("progressbar.png");
		mBar->setAnchorPoint(Vec2(0, 0));
		mBar->setPosition(Vec2(bgX-322, bgY-205));
		addChild(mBar);


		//设置进度条的纹理矩形的大小
		mOriginW = mBar->getTextureRect().size.width; //设置进度条的原始大小
		mBar->setTextureRect(CCRectMake(mBar->getTextureRect().origin.x, mBar->getTextureRect().origin.y, 0, mBar->getTextureRect().size.height));

		//创建每日提醒
		mTips = CCLabelTTF::create(a_u8(getTip().c_str()), "arial.ttf", 22);
		mTips->setPosition(Vec2(mBar->getPosition().x + 330, mBar->getPosition().y - mBar->getContentSize().height));
		addChild(mTips);

// 		//得到程序的版本
// 		CCLabelTTF* ttfVersion = CCLabelTTF::create(platformGetPlatformVersion_SHZ().c_str(), "Arial", 24);
// 		ttfVersion->setPosition(ccp(kRevolutionWidth_SHZ-ttfVersion->getContentSize().width-50, ttfVersion->getContentSize().height+10));
// 		addChild(ttfVersion);

		return true;
	} while (0);

	return false;
}

std::string SHZLoadingLayer::getTip()
{
	char szKeyIndex[10];
	// 得到提示的个数
	int tips_count = CCString(SSTRING("tips_count")).intValue();
	// 得到显示提醒的索引
	int tips_index = JZUserDefault::getInstance()->getIntegerForKey(TIPS, TIPS_INDEX);
	// 防止外部修改越界
	tips_index = tips_index % tips_count;
	sprintf(szKeyIndex, "tips_%d", tips_index);

	// 保存下一次提示的索引
	tips_index = ++tips_index % tips_count;
	JZUserDefault::getInstance()->setIntegerForKey(TIPS, TIPS_INDEX, tips_index);
	return std::string(SSTRING(szKeyIndex));
}

//当Director调用runWithScene的时候，启动的第一个程序
void SHZLoadingLayer::onEnterTransitionDidFinish()
{ 
	CCLayer::onEnterTransitionDidFinish();
	
	DF::shared()->init(KIND_ID_SHZ, GAME_PLAYER_SHZ, VERSION_CLIENT_SHZ, STATIC_DATA_STRING("appname"), 0);
	mResources = __Array::createWithContentsOfFileThreadSafe("resources.plist");
	
	CC_SAFE_RETAIN(mResources);
	//初始化相应的参数
	mLoadedNum = 0;
	mTotalNum = mResources == 0 ? 0 : mResources->count();   //需要加载的个数
	mCurW = 0;
	mTargetW = 0;

	// 启动加载
	this->scheduleUpdate();  //启动程序的调度器
}

void SHZLoadingLayer::update(float delta)
{
	static const int FRAME_LOAD = 2;
	static int FRAME_CUR = FRAME_LOAD;

	if (FRAME_CUR >= FRAME_LOAD)
	{
		FRAME_CUR = 0;

		if (mLoadedNum < mTotalNum)
		{
			const char* name = ((CCString*)mResources->objectAtIndex(mLoadedNum))->getCString();
			char sPlist[80] = {0};
			sprintf(sPlist, "%s.plist", name);
			CCSpriteFrameCache::getInstance()->addSpriteFramesWithFile(sPlist);
			mLoadedNum++;

			mTargetW = mOriginW*((float)mLoadedNum / mTotalNum);
		}
	}
	else
	{
		FRAME_CUR++;
	}

	if (mCurW < mTargetW)
	{
		mCurW += (mTargetW - mCurW) / 2 + 1;

		if (mCurW >= mTargetW)
		{
			mCurW = mTargetW;
		}

		mBar->setTextureRect(CCRectMake(mBar->getTextureRect().origin.x, mBar->getTextureRect().origin.y, mCurW, mBar->getTextureRect().size.height));
	}

	if (mLoadedNum < mTotalNum)
		return;

	if (mCurW < mTargetW)
		return;

	//资源加载完成
	this->unscheduleUpdate();
	CC_SAFE_RELEASE_NULL(mResources);

	mBar->setTextureRect(CCRectMake(mBar->getTextureRect().origin.x, mBar->getTextureRect().origin.y, mOriginW, mBar->getTextureRect().size.height));

	G_NOTIFICATION_SHZ("MODE_SELECTED", SHZMTData::create(1));

	return;
}
