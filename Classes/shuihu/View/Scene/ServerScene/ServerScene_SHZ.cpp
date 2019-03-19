#include "shuihu/GameHeader_SHZ.h"
#include "ServerScene_SHZ.h"
#include "ChatLayer_SHZ.h"

#include "Player/Player.h"

USING_NS_CC;

//////////////////////////////////////////////////////////////////////////
SHZServerScene* SHZServerScene::create(HallGameServerItem* pGameServerItem)
{
	SHZServerScene* scene = new SHZServerScene();
	if (scene && scene->init(pGameServerItem))
	{
		scene->autorelease();
		return scene;
	}

	delete scene;
	return 0;
}
//////////////////////////////////////////////////////////////////////////

SHZServerScene::SHZServerScene()
{

}

SHZServerScene::~SHZServerScene()
{
	if (SHZIServerItem::get())
	{
		mServerItem->SetServerItemSink(0);
		mServerItem->SetChatSink(0);
		mServerItem->SetStringMessageSink(0);
	}
	
	mServerItem=0;

	PLAZZ_PRINTF("~ServerScene");

	//切换场景时将金币数量更新至player上
	CMD_GP_LogonSuccess HallLogonData = Player::getInstance()->getLogonData();
	HallLogonData.lUserScore = SHZCGlobalUserInfo::GetInstance()->GetUserInsureInfo()->lUserScore;
	Player::getInstance()->setLogonData(HallLogonData);
}

//初始化方法
bool SHZServerScene::init(HallGameServerItem* pGameServerItem)
{
	do 
	{
		CC_BREAK_IF(!CCScene::init());
		mGameServerItem = pGameServerItem;
		mServerItem = 0;

		mChatLayer = SHZChatLayer::create();
		addChild(mChatLayer);

		CCSprite *bg=CCSprite::createWithSpriteFrameName("loading_circle.png");
		bg->setPosition(Vec2(kRevolutionWidth_SHZ/2,kRevolutionHeight_SHZ/2));

		CCActionInterval * a1=CCRotateBy::create(0.5f,360);

		bg->runAction(cocos2d::CCRepeatForever::create(a1)); 

		addChild(bg);

		return true;
	} while (0);

	return false;
}

void SHZServerScene::onEnterTransitionDidFinish()
{	
	CCScene::onEnterTransitionDidFinish();

	if (mServerItem == 0)
	{
		// 网络版
		if (0 == DF::shared()->GetGameType() % 2)
		{
			mServerItem = SHZIServerItem::create();
			mServerItem->SetServerItemSink(this);
			mServerItem->SetChatSink(mChatLayer);
			mServerItem->SetStringMessageSink(mChatLayer);
			mServerItem->ConnectServer(mGameServerItem, 0, 0);
		   
		}
		else
		{
			CreateKernel();
		}
		
	}

}


void SHZServerScene::onQuickSitdown(float dt)
{
	mServerItem->PerformQuickSitDown();
}