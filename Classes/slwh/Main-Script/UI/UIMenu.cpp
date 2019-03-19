#include "UIMenu.h"
#include "slwh/Hall-Script/UILayerHall.h"
#include "slwh/Network/NetworkKernel.h"
#include "HallGameScene.h"
#include "slwh/Module/SLWHReturn.h"
#include "slwh/Main-Script/SLWHGameScene.h"
#include "LanguageConfig.h"

//Menu节点下按钮逻辑标签
#define RECHARGE	1329	
#define	SETTING		46
#define	REGULAR		47
#define	EXIT		48

UIMenu::UIMenu():
m_recharge(nullptr),
m_setting(nullptr),
m_regular(nullptr),
m_exit(nullptr)
{
}

UIMenu::~UIMenu()
{
}

void UIMenu::Start()
{
	m_recharge = dynamic_cast<Button*>(m_node->getChildByTag(RECHARGE));
	CC_ASSERT(nullptr != m_recharge);
	m_recharge->addTouchEventListener(this, toucheventselector(UIMenu::BtEvent));

	m_setting = dynamic_cast<Button*>(m_node->getChildByTag(SETTING));
	CC_ASSERT(nullptr != m_setting);
	m_setting->addTouchEventListener(this, toucheventselector(UIMenu::BtEvent));

	m_regular = dynamic_cast<Button*>(m_node->getChildByTag(REGULAR));
	CC_ASSERT(nullptr != m_regular);
	m_regular->addTouchEventListener(this, toucheventselector(UIMenu::BtEvent));

	m_exit = dynamic_cast<Button*>(m_node->getChildByTag(EXIT));
	CC_ASSERT(nullptr != m_exit);
	m_exit->addTouchEventListener(this, toucheventselector(UIMenu::BtEvent));

}

void UIMenu::BtEvent(Ref *pSender, TouchEventType type)
{
	auto bt = dynamic_cast<Button*>(pSender);

	switch (type)
	{
	case TOUCH_EVENT_BEGAN:
		bt->setColor(Color3B(_GRAY, _GRAY, _GRAY));
		bt->runAction(ScaleTo::create(0.05f, 1.1f));
		break;
	case TOUCH_EVENT_MOVED:
		break;
	case TOUCH_EVENT_ENDED:
		bt->setColor(Color3B(_ORIGINAL, _ORIGINAL, _ORIGINAL));
		bt->runAction(ScaleTo::create(0.05f, 1.0f));
		_PLAY_MUSCI_EFF(_CLICK);
		switch (bt->getTag())
		{
		case RECHARGE:
		{
						 auto rechargeNode = (Node*)HallGameScene::createWindow(EWT_Shop);
						 rechargeNode->setPosition(Vec2(0, 0));
						 this->addChild(rechargeNode, 100);
		}
			break;
		case SETTING:
			this->addChild((Node*)HallGameScene::createWindow(EWT_Setting));
			break;
		case REGULAR:
			Regular::GetInstance()->Show();
			break;
		case EXIT:			
			SLWHReturn::openReturnNotice(GETSTRING(1412), [this](Ref* ref){
				exit();
			});

			 break;
		default:
			break;
		}
		break;
	case TOUCH_EVENT_CANCELED:
		bt->runAction(ScaleTo::create(0.05f, 1.0f));
		bt->setColor(Color3B(_ORIGINAL, _ORIGINAL, _ORIGINAL));
		break;
	default:
		break;
	}
}

void UIMenu::exit(int code /*= 0*/)
{
	//auto gameinfo = OneGameInfo::GetInstance();
	//gameinfo->setStartGameFinish(false);
	//gameinfo->getroomInfo()->vec.clear();

	//如果是在压分阶段，清除玩家压分
	auto gameinfo = OneGameInfo::GetInstance();
	if (gameinfo->getServerState() == _Jet)
	{
		//发送取消压分消息
		NetworkKernel::getInstance()->send(MDM_GF_GAME_SLWH, SUB_G_GAMECHEARNOTES);
		//自己加上金币 不能保证金币一定正确
		SLWHGameScene::m_uiLayer->m_myJetManager.successCancelJet();
	}
	else
	{
		//用户退出时设置用户的金币数
		int coinScore = gameinfo->getscoreChange();
		int outScore = coinScore - Player::getInstance()->getLogonData().lUserScore;
		if (outScore > 0)
		{
			Player::getInstance()->addCoin(outScore, false);
		}
	}
	//用户信息
	SLWHUserInfo info = OneGameInfo::GetInstance()->getUserInfo();
	//发送起立
	NetworkKernel::getInstance()->SendStandUpPacket(info.wTableID, info.wChairID, TRUE);
	//断开连接
	NetworkKernel::getInstance()->relese();
	//清空信息
	gameinfo->relese();

	if (code == 0)
	{
		//返回选择房间界面
		Director::getInstance()->replaceScene(CCTransitionFade::create(0.5f, UILayerHall::createScene()));
	}
	else if (code == -1)
	{
		//只断开，不退出(不切换场景)
	}
	else
	{
		//返回大厅
		Director::getInstance()->replaceScene(CCTransitionFade::create(0.5f, HallGameScene::createScene()));
	}  
}

