#include "shuihu/GameHeader_SHZ.h"
#include "shuihu/ViewHeader_SHZ.h"
#include "UserInfoLayer_SHZ.h"
#include "shuihu/View/ui/UIShezhi_SHZ.h"
#include "shuihu/View/ui/UIHelp_SHZ.h"
#include "shuihu/MTNotification_SHZ.h"
#include "df/LocalStore.h"
#include "Player/Player.h"
#include "Sound/SoundManager.h"
#include "HallGameScene.h"

USING_NS_CC;
USING_NS_CC_EXT;
//艺术字
#define GAME_NUMNER "images/number/number_0.png"
//////////////////////////////////////////////////////////////////////////

/*---------修改bysl----------*/
#define ROOMLIST_PLIST_SHZ      "selectroom/room.plist"
#define ROOMLIST_PNG_SHZ        "selectroom/room.png"

#define IMG_NETWORK_BG          "sr_bg.png"
#define IMG_BT_BACK_1_SHZ		"sr_back.png"
#define IMG_BT_BACK_2_SHZ		"sr_back.png"
#define IMG_BT_SETTING_1	    "sr_settings.png"
#define IMG_BT_SETTING_2	    "sr_settings.png"
#define IMG_BT_HELP_1_SHZ		"sr_rule.png"
#define IMG_BT_HELP_2_SHZ		"sr_rule.png"
#define IMG_BT_RECHARGE_1	    "sr_recharge.png"  
#define IMG_BT_RECHARGE_2	    "sr_recharge.png"



#define TAG_CLOSE_SHZ			0
#define TAG_SETTING			1
#define TAG_RECHARGE		4
#define TAG_HELP_SHZ			5


#define SHARE_TIME_INTERNAL_SHZ   (24*3600.0)

#define Hall_Message_Refresh_Coin			"Hall_Message_Refresh_Coin"			//刷新金币显示
SHZUserInfoLayer::SHZUserInfoLayer()
{
	
	/*-----------修改bysl---------*/
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(ROOMLIST_PLIST_SHZ);
	SoundManager::shared()->preloadEffect();

	HALL_NOTIFICATION_REG(Hall_Message_Refresh_Coin,SHZUserInfoLayer::refreshPlayerCoin, nullptr);
	
}

SHZUserInfoLayer::~SHZUserInfoLayer()
{    
	HALL_NOTIFICATION_UNREG(Hall_Message_Refresh_Coin);
	G_NOTIFICATION_UNREG_SHZ("USER_SCORE");
	/*-----------修改bysl---------*/
	CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(ROOMLIST_PLIST_SHZ);
	Director::getInstance()->getTextureCache()->removeTextureForKey(ROOMLIST_PNG_SHZ);
}

//初始化方法
bool SHZUserInfoLayer::init()
{

	int __gType = DF::shared()->GetGameType();  //得到当前的游戏模式

	do 
	{
		CC_BREAK_IF(!CCLayer::init());
		

		//背景图片
		CCSprite* bg;
		if (0 == __gType)
			bg = CCSprite::createWithSpriteFrameName(IMG_NETWORK_BG);
		bg->setPosition(Vec2(kRevolutionWidth_SHZ / 2, kRevolutionHeight_SHZ / 2));
		addChild(bg);

		//////////////////////////////////////////////////////////////////////////

		//退出按钮
		ControlButton* btBack = createButton_SHZ(IMG_BT_BACK_1_SHZ, IMG_BT_BACK_2_SHZ, 0, this, cccontrol_selector(SHZUserInfoLayer::onBtnClick));
		CCSize si = btBack->getContentSize();
		btBack->setTag(TAG_CLOSE_SHZ);
		btBack->setPosition(Vec2(1011, 683));
		addChild(btBack);

		//设置按钮
		ControlButton* btSetting = createButton_SHZ(IMG_BT_SETTING_1, IMG_BT_SETTING_2, 0, this, cccontrol_selector(SHZUserInfoLayer::onBtnClick));
		btSetting->setTag(TAG_SETTING);
		addChild(btSetting);
		btSetting->setPosition(Vec2(924, 683));


		//帮助按钮
		ControlButton* btHelp = createButton_SHZ(IMG_BT_HELP_1_SHZ, IMG_BT_HELP_2_SHZ, 0, this, cccontrol_selector(SHZUserInfoLayer::onBtnClick));
		btHelp->setTag(TAG_HELP_SHZ);
		btHelp->setPosition(Vec2(841, 683));
		addChild(btHelp);
		//充值按钮
		ControlButton* btRecharge = createButton_SHZ(IMG_BT_RECHARGE_1, IMG_BT_RECHARGE_2, 0, this, cccontrol_selector(SHZUserInfoLayer::onBtnClick));
		btRecharge->setTag(TAG_RECHARGE);
		btRecharge->setPosition(Vec2(748, 683));
		addChild(btRecharge);


		//用户的金币数
		mLbScore = LabelAtlas::create("", GAME_NUMNER, 19, 22, '0');
		mLbScore->setPosition(Vec2(160, 695));
		mLbScore->setAnchorPoint(Vec2(0.5,0.5));
		addChild(mLbScore,10);

		

		//网络版用户的ID
		if (0 == __gType)
		{  
			/*---------修改bysl-----------*/
			//金币数背景
			Sprite* gold = Sprite::createWithSpriteFrameName("sr_gold.png");
			gold->setPosition(Vec2(145, 695));
			this->addChild(gold);
			//元宝数背景
			Sprite* ingot = Sprite::createWithSpriteFrameName("sr_-ingot.png");
			ingot->setPosition(Vec2(400, 695));
			this->addChild(ingot);

			//用户元宝数
			m_LbLv = LabelAtlas::create("", GAME_NUMNER, 19, 22, '0');
			m_LbLv->setAnchorPoint(Vec2(0.5, 0.5));
			m_LbLv->setPosition(Vec2(423, 695));
			addChild(m_LbLv);
		}
		
		
		setDatas();
		G_NOTIFICATION_REG_SHZ("USER_SCORE", SHZUserInfoLayer::onUserScore, 0);		// 用户积分回调

		return true;
	} while (0);

	return false;
}




//设置用户的数据
void SHZUserInfoLayer::setDatas()
{
	onUserScore(0);     //设置用户分数
}


void SHZUserInfoLayer::setCloseDialogInfo(cocos2d::CCObject* target, cocos2d::SEL_CallFuncN callfun, const std::string& title, const std::string& content)
{
	mTarget		= target;
	mCallback	= callfun;
	mTitle		= title;
	mContent	= content;
}


//////////////////////////////////////////////////////////////////////////
//按钮事件
void SHZUserInfoLayer::onBtnClick(cocos2d::CCObject* obj, cocos2d::extension::Control::EventType e)
{
	Control* ctr = (Control*)obj;

	SHZSoundManager::shared()->playSound("btsounds");

	switch (ctr->getTag())
	{
	case TAG_CLOSE_SHZ:
		popup_SHZ(mTitle.c_str(), mContent.c_str(), DLG_MB_OK|DLG_MB_CANCEL, 0, mTarget, mCallback);
		break;
	case TAG_RECHARGE:  //充值回调
		{
							auto rechargeNode = (Node*)HallGameScene::createWindow(EWT_Shop);
							rechargeNode->setPosition(Vec2(0, 0));
							getParent()->addChild(rechargeNode, 2000);
		}
		break;
	case TAG_SETTING:   //设置回调
		{

			WindowBase* pWindowSetting = HallGameScene::createWindow(EWT_Setting);
			getParent()->addChild((Node*)pWindowSetting, 2000);
		}
		break;
	case TAG_HELP_SHZ:   //帮助回调
		{
			SHZUIHelp* help = SHZUIHelp::create();
			help->setPosition(Vec2(kRevolutionWidth_SHZ/2,kRevolutionHeight_SHZ/2));
			getParent()->addChild(help);
		}
		break;
	}
	
}


//////////////////////////////////////////////////////////////////////////
//通知消息

void SHZUserInfoLayer::onUserScore(cocos2d::CCObject* obj)
{
	//变量定义
	SHZCGlobalUserInfo   * pGlobalUserInfo=SHZCGlobalUserInfo::GetInstance();
	tagSHZUserInsureInfo * pUserInsureInfo=pGlobalUserInfo->GetUserInsureInfo();
	tag_SHZGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	int __gType = DF::shared()->GetGameType();  //得到当前的游戏模式

	auto player = Player::getInstance();

	//mLbScore->setString(toString((int)Player::getInstance()->getLogonData().lUserScore));
	m_LbLv->setString(toString((int)Player::getInstance()->getHallData().llYuanBao));

	mLbScore->setString(toString(pGlobalUserInfo->GetUserInsureInfo()->lUserScore));

	if (0 == __gType) { 
		//todosl
		//m_LbLv->setString(toString((int)pGlobalUserData->dwGameID));		
	}

}

void SHZUserInfoLayer::onEnter()
{
	CCLayer::onEnter();

	onUserScore(0);
}
     




//////////////////////////////////////////////////////////////////////////
// IGPInsureMissionSink
void SHZUserInfoLayer::onInsureInfo()
{
	onUserScore(0);
}


void SHZUserInfoLayer::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	switch (keyCode)
	{
	case EventKeyboard::KeyCode::KEY_ESCAPE:
		popup_SHZ(mTitle.c_str(), mContent.c_str(), DLG_MB_OK | DLG_MB_CANCEL, 0, mTarget, mCallback);
		break;
	default:
		break;
	}
}

void SHZUserInfoLayer::onShareSucceed(int score){
	
	SHZCGlobalUserInfo *pGlobalUserInfo = SHZCGlobalUserInfo::GetInstance();
	mLbScore->setString(toString(pGlobalUserInfo->GetUserInsureInfo()->lUserScore));
}

void SHZUserInfoLayer::onShareFailed(){

}

void SHZUserInfoLayer::onReward(cocos2d::CCObject* obj)
{

	SHZMTData* __data = (SHZMTData*)obj;

	if (!__data) return;

	int reward = __data->mData1;

	SHZCGlobalUserInfo *pGlobalUserInfo = SHZCGlobalUserInfo::GetInstance();
	pGlobalUserInfo->GetUserInsureInfo()->lUserScore += reward;
	mLbScore->setString(toString(pGlobalUserInfo->GetUserInsureInfo()->lUserScore));
	
}

//刷新界面显示
void SHZUserInfoLayer::refreshPlayerCoin(Ref* ref)
{
	auto player = Player::getInstance();
	SHZCGlobalUserInfo *pGlobalUserInfo = SHZCGlobalUserInfo::GetInstance();
	pGlobalUserInfo->GetUserInsureInfo()->lUserScore = player->getLogonData().lUserScore;
	mLbScore->setString(toString(pGlobalUserInfo->GetUserInsureInfo()->lUserScore));
	onUserScore(0);
}