#include "GameHeader_SHZ.h"
#include "ViewHeader_SHZ.h"
#include "View/Scene/ServerListScene/ServerListScene_SHZ.h"
#include "View/Scene/ServerScene/ServerScene_SHZ.h"
#include "MTNotification_SHZ.h"
#include "View/Scene/GameScene/TimestampManager_SHZ.h"
#include "View/Scene/GameScene/GameClientView_SHZ.h"

USING_NS_CC;

extern void split_SHZ(const std::string &s, char delim, std::vector<std::string> &elems);
CCScheduler* __gGloablScheduler_SHZ = 0;

//////////////////////////////////////////////////////////////////////////
class SHZMTNotificationImp : public cocos2d::CCNode
{
private:
	int mDefaultModeSelected;

public:
	SHZMTNotificationImp()
	{
		mDefaultModeSelected = 0;

		//注册消息标示	
		G_NOTIFICATION_REG_SHZ("MODE_SELECTED"      , SHZMTNotificationImp::modeSelectedNotify, 0);	
		G_NOTIFICATION_REG_SHZ("BACK_TO_SERVER_LIST", SHZMTNotificationImp::backToServerListNofity, 0);	
		G_NOTIFICATION_REG_SHZ("CONNECT_SERVER"     , SHZMTNotificationImp::connectServerNotify, 0);	
		G_NOTIFICATION_REG_SHZ("BACK_TO_SERVER"     , SHZMTNotificationImp::backToServerNotify, 0);	
		G_NOTIFICATION_REG_SHZ("GAME_EXIT_TIMEOUT"  , SHZMTNotificationImp::gameExitTimeout, 0);
		G_NOTIFICATION_REG_SHZ("LAUNCH_LOCAL", SHZMTNotificationImp::onLaunchLocalNotify, 0);
	}
	~SHZMTNotificationImp()
	{
		G_NOTIFICATION_UNREG_SHZ("MODE_SELECTED");	
		G_NOTIFICATION_UNREG_SHZ("BACK_TO_SERVER_LIST");	
		G_NOTIFICATION_UNREG_SHZ("CONNECT_SERVER");	
		G_NOTIFICATION_UNREG_SHZ("BACK_TO_SERVER");	
		G_NOTIFICATION_UNREG_SHZ("GAME_EXIT_TIMEOUT");
		G_NOTIFICATION_UNREG_SHZ("SELECTED_GAME_MODE");

		G_NOTIFICATION_UNREG_SHZ("LAUNCH_LOCAL");
	}

public:
	void onLaunchLocalNotify(CCObject* obj){

		CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, SHZCGameClientView::create()));

	}



	void modeSelectedNotify(CCObject* obj)
	{
		SHZMTData* data = (SHZMTData*)obj;
		CCAssert(data, "modeSelectedNotify");
		mDefaultModeSelected = data->mData1;
						
		switch (mDefaultModeSelected)
		{
		case 1:
			{
				CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f,SHZServerListScene::create(mDefaultModeSelected)));  
			}
			break;
		}
	}

	void backToServerListNofity(CCObject* obj)
	{

		SHZServerListScene* scene = SHZServerListScene::create(mDefaultModeSelected);

		if (obj)
		{
			SHZSoundManager::shared()->stopSound("gundong");
			SHZMTData* data = (SHZMTData*)obj;
			popup_SHZ(data->mStr1.c_str(), data->mStr2.c_str(), 0, 0.6f, scene, callfuncN_selector(SHZServerListScene::closeCallback), scene);
		}

		CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f,scene));  
	}

	void connectServerNotify(CCObject* obj)
	{
		SHZMTData* data = (SHZMTData*)obj;
		CCAssert(data, "connectServerNotify");
		CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.1f, SHZServerScene::create((HallGameServerItem*)data->m_ptData)));  
	}

	void backToServerNotify(CCObject* obj)
	{
		CCDirector::sharedDirector()->popScene();
	}
	
	void gameExitTimeout(CCObject* obj)
	{
		__gGloablScheduler_SHZ->unscheduleSelector(schedule_selector(SHZMTNotificationImp::onGameExitTimeout), this);
		__gGloablScheduler_SHZ->scheduleSelector(schedule_selector(SHZMTNotificationImp::onGameExitTimeout), this, 0.0f, 1, 1.5f, false);
	}

	void onGameExitTimeout(float dt)
	{
		__gGloablScheduler_SHZ->unscheduleSelector(schedule_selector(SHZMTNotificationImp::onGameExitTimeout), this);
		popup_SHZ(SSTRING("system_tips_title").c_str(), a_u8("由于您一分钟未发射子弹,自动退出游戏"));
	}
}; // MTNotificationImp


//////////////////////////////////////////////////////////////////////////

SHZMTNotification* SHZMTNotification::msInstance = 0;

SHZMTNotification* SHZMTNotification::shared()
{
	if (msInstance==0)
		msInstance = new SHZMTNotification();
	return msInstance;
}

void SHZMTNotification::purge()
{
	CSocket::purge();
	if (msInstance)
		delete msInstance;
	msInstance = 0;
}

//////////////////////////////////////////////////////////////////////////

SHZMTNotification::SHZMTNotification()
{
	__gGloablScheduler_SHZ = new CCScheduler();
	mImp = new SHZMTNotificationImp();
}

SHZMTNotification::~SHZMTNotification()
{
	delete mImp;
	delete __gGloablScheduler_SHZ;
	__gGloablScheduler_SHZ = 0;
}

void SHZMTNotification::post(float dt)
{
	if (__gGloablScheduler_SHZ)
		__gGloablScheduler_SHZ->update(dt);

	gTimestampManager_SHZ.tick(dt);
	CSocket::post();
}