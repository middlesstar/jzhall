#include "SelectRoom.h"
#include "cocostudio/CocoStudio.h"
#include "shark/resourceDef.h"
#include "shark/UI/GameScene/SharkGameScene.h"
#include "shark/NetWork/SharkNetworkKernel.h"
#include "HallGameScene.h"
#include "df/ServerListData.h"
#include "shark/UI/RuleShow.h"
#include "shark/Module/SharkNotice.h"
#include "LanguageConfig.h"
#include "shark/Module/ReturnDialog.h"
#include "shark/UI/LoadingScene/LoadingShark.h"

#define		ROOM_SWALLOW						"room1"					//燕子房间
#define		ROOM_LION							"room2"					//狮子房间
#define		ROOM_HAWK							"room3"					//老鹰房间
#define     RECHARGE                            "btn_recharge"          //充值按钮
#define     RULE                                "btn_rule"              //规则按钮
#define     SETTING                             "btn_setting"           //设置按钮
#define     BACK                                "btn_back"              //返回按钮

#define     RECHARGELIGHT                       "rechargeLight"
#define     shark_call_netWork                  "callGameServer"

SelectRoom::SelectRoom()
:m_roomSwallow(nullptr),
m_roomHawk(nullptr),
m_roomLion(nullptr),
recharge(nullptr),
rechargeLight(nullptr),
rule(nullptr),
setting(nullptr),
back(nullptr),
goldLabel(nullptr),
ingotLabel(nullptr),
serverPort(_ZERO)
{
	HALL_NOTIFICATION_REG(Hall_Message_Refresh_Coin, SelectRoom::updateSharkCoin, nullptr);
	HALL_NOTIFICATION_REG(shark_call_netWork, SelectRoom::connectGame,nullptr);
}
SelectRoom::~SelectRoom()
{
	HALL_NOTIFICATION_UNREG(Hall_Message_Refresh_Coin);
	HALL_NOTIFICATION_UNREG(shark_call_netWork);
}
Scene* SelectRoom::createScene()
{
	auto scene = Scene::create();

	auto layer = SelectRoom::create();

	scene->addChild(layer);

	return scene;
}
bool SelectRoom::init()
{
	if (!Layer::init())
	{
		return false;
	}
	//请求游戏服务器列表
	Player::getInstance()->requsetServerList();
	//播放音乐
	GameInfo::GetInstance()->selectMusicBack();


	//获取COCOS_STUDIO主场景UI根节点
	pMainNode = CSLoader::createNode(COCOS_STUDIO_SELECTSCENE);
	CC_ASSERT(nullptr != pMainNode);
	addChild(pMainNode);

	m_roomSwallow = dynamic_cast<Button*>(pMainNode->getChildByName(ROOM_SWALLOW));
	CC_ASSERT(nullptr != m_roomSwallow);
	m_roomSwallow->setTag(101);
	m_roomSwallow->addTouchEventListener(this, toucheventselector(SelectRoom::BtEvent));

	m_roomLion = dynamic_cast<Button*>(pMainNode->getChildByName(ROOM_LION));
	CC_ASSERT(nullptr != m_roomLion);
	m_roomLion->setTag(102);
	m_roomLion->addTouchEventListener(this, toucheventselector(SelectRoom::BtEvent));

	m_roomHawk = dynamic_cast<Button*>(pMainNode->getChildByName(ROOM_HAWK));
	CC_ASSERT(nullptr != m_roomLion);
	m_roomHawk->setTag(103);
	m_roomHawk->addTouchEventListener(this, toucheventselector(SelectRoom::BtEvent));


	recharge = dynamic_cast<Button*>(pMainNode->getChildByName(RECHARGE));
	CC_ASSERT(nullptr != recharge);
	recharge->setTag(104);
	recharge->addTouchEventListener(this, toucheventselector(SelectRoom::BtEvent));

	rechargeLight = dynamic_cast<ImageView*>(pMainNode->getChildByName(RECHARGELIGHT));
	CC_ASSERT(nullptr != rechargeLight);

	rule = dynamic_cast<Button*>(pMainNode->getChildByName(RULE));
	CC_ASSERT(nullptr != rule);
	rule->setTag(105);
	rule->addTouchEventListener(this, toucheventselector(SelectRoom::BtEvent));

	setting = dynamic_cast<Button*>(pMainNode->getChildByName(SETTING));
	CC_ASSERT(nullptr != setting);
	setting->setTag(106);
	setting->addTouchEventListener(this, toucheventselector(SelectRoom::BtEvent));

	back = dynamic_cast<Button*>(pMainNode->getChildByName(BACK));
	CC_ASSERT(nullptr != back);
	back->setTag(107);
	back->addTouchEventListener(this, toucheventselector(SelectRoom::BtEvent));

	goldLabel = dynamic_cast<TextAtlas*>(pMainNode->getChildByName("bg_gold")->getChildByName("label_gold"));
	CC_ASSERT(nullptr != goldLabel);

	ingotLabel = dynamic_cast<TextAtlas*>(pMainNode->getChildByName("bg_ingot")->getChildByName("label_ingot"));
	CC_ASSERT(nullptr != ingotLabel);

	//设置元宝和金币数
	setScore();
	//充值按钮一直闪动
	moveRecharge();
	setKeyboardEnabled(true);

	//开启聊天
	HallGameScene::openChatSprite(this);
	selectFlag = true;
	return true;
}


//设置金币数和元宝数
void SelectRoom::setScore()
{
	int goldScore = Player::getInstance()->getLogonData().lUserScore;
	goldLabel->setString(StringUtils::format("%d", goldScore));
	int ingotScore =  Player::getInstance()->getHallData().llYuanBao;
	ingotLabel->setString(StringUtils::format("%d",ingotScore));
}
void SelectRoom::BtEvent(Ref *pSender, TouchEventType type)
{
	auto bt = dynamic_cast<Button*>(pSender);

	switch (type)
	{
	case TOUCH_EVENT_BEGAN:
		bt->setColor(Color3B(180, 180, 180));
		bt->runAction(ScaleTo::create(0.05f, 1.1f));
		break;
	case TOUCH_EVENT_MOVED:
		break;
	case TOUCH_EVENT_ENDED:
		_PLAY_SOUND_SHARK(PRESS);
		bt->setColor(Color3B(255, 255, 255));
		bt->runAction(ScaleTo::create(0.05f, 1.f));
		switch (bt->getTag())
		{
		case 101:
		{
					if (selectFlag)
					{
						goRoom(0);
					}
					break;
		}
		case 102:
		{
					if (selectFlag)
					{
						goRoom(1);
					}
					break;
		}
		case 103:
		{
					if (selectFlag)
					{
						goRoom(2);
					}
					break;
		}
		case 104:
			//充值按钮
		{
					auto rechargeNode = (Node*)HallGameScene::createWindow(EWT_Shop);
					rechargeNode->setPosition(Vec2(0, 0));
					this->addChild(rechargeNode, 100);
					break;
		}

		case 105:
		{
					auto ruleNode = RuleShow::create();
					ruleNode->setPosition(Vec2(0, 0));
					this->addChild(ruleNode, 100);
					break;
		}

		case 106:
		{
					auto settingNode = (Node*)HallGameScene::createWindow(EWT_Setting);
					settingNode->setPosition(Vec2(0, 0));
					this->addChild(settingNode, 100);
					break;
		}

		case 107:
		{
					ReturnDialog::openReturnNotice(GETSTRING(1411), [this](Ref*){
						this->goHall();
					});
		}
			break;
		default:
			break;
		}
		break;
	case TOUCH_EVENT_CANCELED:
		bt->runAction(ScaleTo::create(0.05f, 1.f));
		bt->setColor(Color3B(255, 255, 255));
		break;
	default:
		break;
	}
}



void SelectRoom::goHall()
{
	HallGameScene::switchScene(EHST_HALL);

}

void SelectRoom::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_BACK)
	{
		if (ReturnDialog::GetBOpen())
		{
			ReturnDialog::cancelDialog();
		}
		else
		{
			ReturnDialog::openReturnNotice(GETSTRING(1411), [this](Ref* ref){
				this->goHall();
			});
		}				
	}
}

void SelectRoom::goRoom(int roomIndex)
{
	//登陆游戏服务器
	HallGameServerItem* serverinfo = HallServerListData::shared()->getGameServerInfo(KIND_ID_SHARK, roomIndex);
	selectFlag = false;

	if (nullptr == serverinfo)
	{
		log("-----------------%s-----------------", a_u8("房间暂未开放"));
		SharkNotice::openNotice(GETSTRING(1401));

		selectFlag = true;
		return;
	}


	log("-------SCORE LIMIT:%d,VIP LIMIT:%d-----", serverinfo->m_GameRoomInfo_SHARK.roomScoreLimit, serverinfo->m_GameRoomInfo_SHARK.roomVip);
	//没有达到进入房间限制条件
	auto gameInfo = GameInfo::GetInstance();
	switch (roomIndex)
	{
	case 0:
	{
			  if (Player::getInstance()->getLogonData().lUserScore < serverinfo->m_GameRoomInfo_SHARK.roomScoreLimit)
			  {
				  //log("-----------------%s-----------------", a_u8("分数太低请去充值"));
				  SharkNotice::openNotice(StringUtils::format(GETSTRING(1407).c_str(), serverinfo->m_GameRoomInfo_SHARK.roomScoreLimit), [this](Ref*)
				  {
					  auto rechargeNode = (Node*)HallGameScene::createWindow(EWT_Shop);
					  rechargeNode->setPosition(Vec2(0, 0));
					  this->addChild(rechargeNode, 100);

					  selectFlag = true;
				  });
				  return;
			  }
			  else
			  {
				 
				  int room = gameInfo->getroomIndex();
				  room = 0;
				  gameInfo->setroomIndex(room);

			  }
			  break;
	}
		
	case 1:
	{
			  if (Player::getInstance()->getLogonData().lUserScore < serverinfo->m_GameRoomInfo_SHARK.roomScoreLimit)
			  {
				  SharkNotice::openNotice(StringUtils::format(GETSTRING(1407).c_str(), serverinfo->m_GameRoomInfo_SHARK.roomScoreLimit), [this](Ref*)
				  {
					  auto rechargeNode = (Node*)HallGameScene::createWindow(EWT_Shop);
					  rechargeNode->setPosition(Vec2(0, 0));
					  this->addChild(rechargeNode, 100);

					  selectFlag = true;
				  });
				  return;
			  }
			  else
			  {

				  int room = gameInfo->getroomIndex();
				  room = 1;
				  gameInfo->setroomIndex(room);

			  }
			  break;
	}
		
	case 2:
	{
			  if (Player::getInstance()->getHallData().dwVipLevel < serverinfo->m_GameRoomInfo_SHARK.roomVip)
			  {
				  log("-----------------%s-----------------", a_u8("您不是VIP"));
				  SharkNotice::openNotice(GETSTRING(1408), [this](Ref*)
				  {
					  //此处需弹出提示框并跳转到了解vip界面
					  WindowBase* pWindow = HallGameScene::createWindow(EWT_Shop);
					  pWindow->setPosition(Vec2(0,0));
					  this->addChild(pWindow,100);
					  pWindow->updateWindow(0);

					  selectFlag = true;
				  });
				  return;
			  }
			  else
			  {
				  if (Player::getInstance()->getLogonData().lUserScore < serverinfo->m_GameRoomInfo_SHARK.roomScoreLimit)
				  {
					  log("-----------------%s-----------------", a_u8("分数不够"));
					  SharkNotice::openNotice(StringUtils::format(GETSTRING(1407).c_str(), serverinfo->m_GameRoomInfo_SHARK.roomScoreLimit), [this](Ref*)
					  {
						  auto rechargeNode = (Node*)HallGameScene::createWindow(EWT_Shop);
						  rechargeNode->setPosition(Vec2(0, 0));
						  this->addChild(rechargeNode, 100);

						  selectFlag = true;
					  });
					  return;
				  }
				  else
				  {
					  int room = gameInfo->getroomIndex();
					  room = 2;
					  gameInfo->setroomIndex(room);
				  }
				  
			  }
			  break;
	}
		
	default:
		break;
	}

	serverPort = serverinfo->m_GameServer.wServerPort;
	strcpy(serverName, serverinfo->m_GameServer.szServerAddr);
	
	setTime();
	//schedule(schedule_selector(SelectRoom::setTime), 0.5f);
	
}

void SelectRoom::setTime()
{
	auto pScene = SharkGameScene::create();
	Director::getInstance()->replaceScene(CCTransitionFade::create(0.1f,pScene));
	//Director::getInstance()->replaceScene(pScene);
}


void SelectRoom::connectGame(Ref* ref)
{

	auto gameInfo = GameInfo::GetInstance();
	int roomIndex = gameInfo->getroomIndex();
	SharkNetWorkKernel::getInstance()->setUrl(serverName,serverPort);
	//连接服务器
	SharkNetWorkKernel::getInstance()->start();
	SharkNetWorkKernel::getInstance()->SendLogonPacket();

	RoomInfo_Shark* info = GameInfo::GetInstance()->getroomInfor();

	//客户端写死房间压分信息
	switch (roomIndex)
	{
	case 0:
		info->stakeinfo.dwStake[0] = 10;
		info->stakeinfo.dwStake[1] = 50;
		info->stakeinfo.dwStake[2] = 100;
		info->stakeinfo.dwStake[3] = 200;
		info->stakeinfo.dwStake[4] = 500;
		info->stakeinfo.index = 0;
		info->stakeinfo.dwOneLimit = 5000;
		break;
	case 1:
		info->stakeinfo.dwStake[0] = 100;
		info->stakeinfo.dwStake[1] = 500;
		info->stakeinfo.dwStake[2] = 1000;
		info->stakeinfo.dwStake[3] = 2000;
		info->stakeinfo.dwStake[4] = 5000;
		info->stakeinfo.index = 0;
		info->stakeinfo.dwOneLimit = 50000;
		break;
	case 2:
		info->stakeinfo.dwStake[0] = 1000;
		info->stakeinfo.dwStake[1] = 5000;
		info->stakeinfo.dwStake[2] = 10000;
		info->stakeinfo.dwStake[3] = 20000;
		info->stakeinfo.dwStake[4] = 50000;
		info->stakeinfo.index = 0;
		info->stakeinfo.dwOneLimit = 500000;
		break;
	default:
		break;
	}

	info->wType = roomIndex;
}


void SelectRoom::setTime(float t)
{
	auto pScene = SharkGameScene::create();
	Director::getInstance()->replaceScene(pScene);
	unschedule(schedule_selector(SelectRoom::setTime));
}


//充值宝箱动画
void SelectRoom::moveRecharge()
{
	if (!recharge || !rechargeLight)
	{
		return;
	}
	recharge->stopAllActions();
	rechargeLight->stopAllActions();

	auto rotateAction = Sequence::create(RotateTo::create(0.06f, 20.0f), RotateTo::create(0.12f, -20.0f),
		RotateTo::create(0.09f, 10.0f), RotateTo::create(0.06f, -10.0f), RotateTo::create(0.03, 0), NULL);
	auto scaleAction = Sequence::create(ScaleTo::create(0.6f, 1.2f), ScaleTo::create(0.6f, 1.0f), NULL);
	recharge->runAction(RepeatForever::create(Sequence::create(DelayTime::create(2.0f), EaseSineInOut::create(rotateAction), scaleAction, NULL)));

	rechargeLight->runAction(RepeatForever::create(RotateBy::create(0.1f, 1.f)));
}

//刷新金币
void SelectRoom::updateSharkCoin(Ref* ref)
{
	setScore();
}