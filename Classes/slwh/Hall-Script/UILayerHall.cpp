#include "UILayerHall.h"
#include "slwh/Network/NetworkKernel.h"
#include "HallGameScene.h"
#include "df/ServerListData.h"
#include "slwh/Module/SLWHNotice.h"
#include "LanguageConfig.h"
#include "Window/WindowBase.h"
#include "slwh/Module/SLWHReturn.h"
#include "slwh/Main-Script/SLWHGameScene.h"
#include "slwh/Hall-Script/SLWHLoadingScene.h"
#include "slwh/Hall-Script/getHouseIndex.h"

//HallScene
#define		ROOM_RABBIT							336					//兔子房间
#define		ROOM_PANDA							335					//熊猫房间
#define		ROOM_LION							334					//狮子房间
#define		HALL_MENU							337					//SLWH大厅菜单
#define		HALL_YUANBAO_SCORE					1308				//元宝
#define		HALL_GOLD							1307				//金币
#define		HALL_REGULAR						328



//Menu下逻辑节点
#define RECHARGE	1313
#define REGULAR		1312
#define SETTING		1314
#define EXIT		1315

//数字名称
#define NUM_NAME	"Num"


#define Hall_Message_Refresh_Coin			"Hall_Message_Refresh_Coin"			//刷新金币显示
UILayerHall::UILayerHall() :
m_recharge(nullptr),
m_setting(nullptr),
m_regular(nullptr),
m_exit(nullptr),
m_goldNum(nullptr),
m_yuanbaoNum(nullptr),
m_roomRabbit(nullptr),
m_roomPanda(nullptr),
m_roomLion(nullptr),
m_limitLion(nullptr),
m_limitPanda(nullptr),
m_limitRabbit(nullptr)
{
	HALL_NOTIFICATION_REG(Hall_Message_Refresh_Coin, UILayerHall::updateSlwhCoin, nullptr);
}

void UILayerHall::onEnter()
{
	Node::onEnter();
}

void UILayerHall::onExit()
{
	Node::onExit();
}

UILayerHall::~UILayerHall()
{
	HALL_NOTIFICATION_UNREG(Hall_Message_Refresh_Coin);
}

Scene* UILayerHall::createScene()
{
	auto scene = Scene::create();

	auto layer = UILayerHall::create();

	scene->addChild(layer);

	return scene;
}

bool UILayerHall::init()
{
	if (!Layer::init())	return false;

	//请求游戏服务器列表
	Player::getInstance()->requsetServerList();

	//获取COCOS_STUDIO主场景UI根节点
	auto rootNode = CSLoader::createNode(COCOS_STUDIO_HALLSCENE);
	CC_ASSERT(nullptr != rootNode);
	addChild(rootNode);

	m_roomRabbit = dynamic_cast<Button*>(rootNode->getChildByTag(ROOM_RABBIT));
	CC_ASSERT(nullptr != m_roomRabbit);
	m_roomRabbit->addTouchEventListener(this, toucheventselector(UILayerHall::BtEvent));

	m_limitRabbit = dynamic_cast<TextAtlas*>(m_roomRabbit->getChildByName("limit")->getChildByName(NUM_NAME));
	CC_ASSERT(nullptr != m_limitRabbit);
	refreshNum(m_limitRabbit, _ZERO);

	m_roomPanda = dynamic_cast<Button*>(rootNode->getChildByTag(ROOM_PANDA));
	CC_ASSERT(nullptr != m_roomPanda);
	m_roomPanda->addTouchEventListener(this, toucheventselector(UILayerHall::BtEvent));

	m_limitPanda = dynamic_cast<TextAtlas*>(m_roomPanda->getChildByName("limit")->getChildByName(NUM_NAME));
	CC_ASSERT(nullptr != m_limitPanda);
	refreshNum(m_limitPanda, _ZERO);

	m_roomLion = dynamic_cast<Button*>(rootNode->getChildByTag(ROOM_LION));
	CC_ASSERT(nullptr != m_roomLion);
	m_roomLion->addTouchEventListener(this, toucheventselector(UILayerHall::BtEvent));

	m_limitLion = dynamic_cast<TextAtlas*>(m_roomLion->getChildByName("limit")->getChildByName(NUM_NAME));
	CC_ASSERT(nullptr != m_limitLion);
	refreshNum(m_limitLion,_ZERO);

	m_recharge = dynamic_cast<Button*>(rootNode->getChildByTag(HALL_MENU)->getChildByTag(RECHARGE));
	CC_ASSERT(nullptr != m_recharge);
	m_recharge->addTouchEventListener(this, toucheventselector(UILayerHall::BtEvent));

	m_setting = dynamic_cast<Button*>(rootNode->getChildByTag(HALL_MENU)->getChildByTag(SETTING));
	CC_ASSERT(nullptr != m_setting);
	m_setting->addTouchEventListener(this, toucheventselector(UILayerHall::BtEvent));

	m_regular = dynamic_cast<Button*>(rootNode->getChildByTag(HALL_MENU)->getChildByTag(REGULAR));
	CC_ASSERT(nullptr != m_regular);
	m_regular->addTouchEventListener(this, toucheventselector(UILayerHall::BtEvent));

	m_exit = dynamic_cast<Button*>(rootNode->getChildByTag(HALL_MENU)->getChildByTag(EXIT));
	CC_ASSERT(nullptr != m_exit);
	m_exit->addTouchEventListener(this, toucheventselector(UILayerHall::BtEvent));

	m_goldNum = dynamic_cast<TextAtlas*>(rootNode->getChildByTag(HALL_GOLD)->getChildByName(NUM_NAME));
	CC_ASSERT(nullptr != m_goldNum);

	m_yuanbaoNum = dynamic_cast<TextAtlas*>(rootNode->getChildByTag(HALL_YUANBAO_SCORE)->getChildByName(NUM_NAME));
	CC_ASSERT(nullptr != m_yuanbaoNum);

	Regular* reg = Regular::GetInstance();
	reg->AttachNode(rootNode->getChildByTag(HALL_REGULAR));
	//addChild(reg);

	//设置元宝和钱
	auto info = Player::getInstance();
	SetGold(info->getLogonData().lUserScore);
	SetYunBao(info->getHallData().llYuanBao);

	setKeyboardEnabled(true);

	return true;
}

void UILayerHall::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_BACK)
	{
		if (SLWHReturn::GetBOpen())
		{
			SLWHReturn::cancelDialog();
		}
		else
		{
			SLWHReturn::openReturnNotice(GETSTRING(1411), [this](Ref* ref){
				ExitToHall();
		
			});
		}
	}

	
}

void UILayerHall::SetYunBao(SCORE y)
{
	std::stringstream str;
	str << y;
	m_yuanbaoNum->setString(str.str());
}

void UILayerHall::SetGold(SCORE g)
{
	std::stringstream str;
	str << g;
	m_goldNum->setString(str.str());
}

void UILayerHall::BtEvent(Ref *pSender, TouchEventType type)
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
		_PLAY_MUSCI_EFF(_CLICK);
		bt->setColor(Color3B(_ORIGINAL, _ORIGINAL, _ORIGINAL));
		bt->runAction(ScaleTo::create(0.05f, 1.f));
		switch (bt->getTag())
		{
		case ROOM_RABBIT:
			goRoom(0);
			break;
		case ROOM_PANDA:
			goRoom(1); 
			break;
		case ROOM_LION:
			goRoom(2);
			break;
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
			SLWHReturn::openReturnNotice(GETSTRING(1411), [this](Ref* ref){
				ExitToHall();
			});
			break;
		default:
			break;
		}
		break;
	case TOUCH_EVENT_CANCELED:
		bt->runAction(ScaleTo::create(0.05f, 1.f));
		bt->setColor(Color3B(_ORIGINAL, _ORIGINAL, _ORIGINAL));
		break;
	default:
		break;
	}
}

void UILayerHall::ExitToHall()
{
	////返回选择房间界面
	//if (SLWHLoadingScene::m_pMainGameScene)
	//{
	//	SLWHLoadingScene::m_pMainGameScene->release();
	//	SLWHLoadingScene::m_pMainGameScene = nullptr;
	//}

	HallGameScene::switchScene(EHST_HALL);
}

void UILayerHall::goRoom(int roomIndex)
{
	//登陆游戏服务器
	HallGameServerItem* serverinfo = HallServerListData::shared()->getGameServerInfo(KIND_ID_SLWH, roomIndex);
	log("----------------dais:logon game server----------------------");

	if (nullptr == serverinfo)
	{
		log("-----------------%s-----------------", a_u8("房间暂未开放"));

		SLWHNotice::openNotice(GETSTRING(1401));
		return;
	}


	log("-------score limit:%d,vip limit:%d-----", serverinfo->m_GameRoomInfo_SLWH.dwScoreLimit, serverinfo->m_GameRoomInfo_SLWH.dwVip);
	//没有达到进入房间限制条件
	switch (roomIndex)
	{
	case 0:
		if (Player::getInstance()->getLogonData().lUserScore < serverinfo->m_GameRoomInfo_SLWH.dwScoreLimit)
		{
			log("-----------------%s-----------------", a_u8("分数不够"));
			char buf[256];
			sprintf(buf, GETSTRING(1407).c_str(), serverinfo->m_GameRoomInfo_SLWH.dwScoreLimit);
			SLWHNotice::openNotice(buf, [this](Ref*)
			{
				auto rechargeNode = (Node*)HallGameScene::createWindow(EWT_Shop);
				rechargeNode->setPosition(Vec2(0, 0));
				this->addChild(rechargeNode, 100);
			});
			return;
		}

		break;
	case 1:
		if (Player::getInstance()->getLogonData().lUserScore < serverinfo->m_GameRoomInfo_SLWH.dwScoreLimit)
		{
			log("-----------------%s-----------------", a_u8("分数不够"));
			char buf[256];
			sprintf(buf, GETSTRING(1407).c_str(), serverinfo->m_GameRoomInfo_SLWH.dwScoreLimit);
			SLWHNotice::openNotice(buf, [this](Ref*)
			{
				auto rechargeNode = (Node*)HallGameScene::createWindow(EWT_Shop);
				rechargeNode->setPosition(Vec2(0, 0));
				this->addChild(rechargeNode, 100);
			});
			return;
		}
		break;
	case 2:
		if (Player::getInstance()->getHallData().dwVipLevel < serverinfo->m_GameRoomInfo_SLWH.dwVip)
		{
			log("-----------------%s-----------------", a_u8("您不是VIP"));

			SLWHNotice::openNotice(GETSTRING(1408), [this](Ref*)
			{
				//此处需弹出提示框并跳转到了解vip界面
				WindowBase* vipwin = HallGameScene::createWindow(EWT_Shop);
				vipwin->setPosition(Vec2(0, 0));
				this->addChild(vipwin, 100);
				vipwin->updateWindow(0);

			});	
			return;
		}
		if (Player::getInstance()->getLogonData().lUserScore < serverinfo->m_GameRoomInfo_SLWH.dwScoreLimit)
		{
			log("-----------------%s-----------------", a_u8("分数不够"));
			char buf[256];
			sprintf(buf, GETSTRING(1407).c_str(), serverinfo->m_GameRoomInfo_SLWH.dwScoreLimit);
			SLWHNotice::openNotice(buf, [this](Ref*)
			{
				auto rechargeNode = (Node*)HallGameScene::createWindow(EWT_Shop);
				rechargeNode->setPosition(Vec2(0, 0));
				this->addChild(rechargeNode, 100);
			});
			return;
		}
		break;
	default:
		break;
	}

	NetworkKernel::getInstance()->setUrl(serverinfo->m_GameServer.szServerAddr, serverinfo->m_GameServer.wServerPort);
	//连接服务器
	NetworkKernel::getInstance()->start();
	NetworkKernel::getInstance()->SendLogonPacket();
	log("----------------dais:send logon game server packet----------------------");
	slwhRoomInfo* info = OneGameInfo::GetInstance()->getroomInfo();
	info->wType = roomIndex;

	GetHouseIndex::getInstance()->setIndex(roomIndex);

	//客户端写死房间压分信息
	switch (roomIndex)
	{
	case 0:
		info->jetinfo.dwJet[0] = 10;
		info->jetinfo.dwJet[1] = 50;
		info->jetinfo.dwJet[2] = 100;
		info->jetinfo.dwJet[3] = 200;
		info->jetinfo.dwJet[4] = 500;
		info->jetinfo.index = 0;
		info->jetinfo.dwOneLimit = 5000;
		break;
	case 1:
		info->jetinfo.dwJet[0] = 100;
		info->jetinfo.dwJet[1] = 500;
		info->jetinfo.dwJet[2] = 1000;
		info->jetinfo.dwJet[3] = 2000;
		info->jetinfo.dwJet[4] = 5000;
		info->jetinfo.index = 0;
		info->jetinfo.dwOneLimit = 50000;
		break;
	case 2:
		info->jetinfo.dwJet[0] = 1000;
		info->jetinfo.dwJet[1] = 5000;
		info->jetinfo.dwJet[2] = 10000;
		info->jetinfo.dwJet[3] = 20000;
		info->jetinfo.dwJet[4] = 50000;
		info->jetinfo.index = 0;
		info->jetinfo.dwOneLimit = 500000;
		break;
	default:
		break;
	}
	schedule(schedule_selector(UILayerHall::setTime), 0.5f);
}

void UILayerHall::setTime(float t)
{

	log("----------------dais:wait logon game server3----------------------");
	static float waittime = _ZERO;
	waittime += t;
	if (waittime > 10.f)
	{
		//提示重新进入
		SLWHNotice::openNotice(GETSTRING(1410), [this](Ref*){
			auto gameinfo = OneGameInfo::GetInstance();
			//用户信息
			SLWHUserInfo info = OneGameInfo::GetInstance()->getUserInfo();
			//发送起立
			NetworkKernel::getInstance()->SendStandUpPacket(info.wTableID, info.wChairID, TRUE);
			//断开连接
			NetworkKernel::getInstance()->relese();
			//清空信息
			gameinfo->relese();
			//返回大厅
			Director::getInstance()->replaceScene(CCTransitionFade::create(0.5f, HallGameScene::createScene()));
		});
		//停止等待
		unschedule(schedule_selector(UILayerHall::setTime));
		waittime =_ZERO;
	}

	if (!OneGameInfo::GetInstance()->getStartGameFinish()) return;

	//成功进入游戏
	auto pScene = SLWHGameScene::create();
	unschedule(schedule_selector(UILayerHall::setTime));
	waittime = _ZERO;
	//Director::getInstance()->runWithScene(CCTransitionFade::create(0.5f, SLWHLoadingScene::m_pMainGameScene));
	Director::getInstance()->replaceScene(CCTransitionFade::create(0.5f, pScene));
	
}

void UILayerHall::onEnterTransitionDidFinish()
{
	HallGameScene::openChatSprite(this);
	_PLAY_MUSCI_BACK(OneGameInfo::GetInstance()->getSLWHMusicFileFullPath("slwh/jetwait").c_str(), true);

	HallGameServerItem* serverinfo0 = HallServerListData::shared()->getGameServerInfo(KIND_ID_SLWH, 0);
	if (serverinfo0 != nullptr) refreshNum(m_limitRabbit, (SCORE)serverinfo0->m_GameRoomInfo_SLWH.dwScoreLimit);
	else refreshNum(m_limitRabbit, (SCORE)0);

	HallGameServerItem* serverinfo2 = HallServerListData::shared()->getGameServerInfo(KIND_ID_SLWH, 2);
	if (serverinfo2 != nullptr) refreshNum(m_limitLion, (SCORE)serverinfo2->m_GameRoomInfo_SLWH.dwScoreLimit);
	else refreshNum(m_limitLion, (SCORE)0);

	HallGameServerItem* serverinfo1 = HallServerListData::shared()->getGameServerInfo(KIND_ID_SLWH, 1);
	if (serverinfo1 != nullptr) refreshNum(m_limitPanda, (SCORE)serverinfo1->m_GameRoomInfo_SLWH.dwScoreLimit);
	else refreshNum(m_limitPanda, (SCORE)0);
}

void UILayerHall::refreshNum(TextAtlas* text, SCORE num)
{
	std::stringstream str;
	str << num;
	text->setString(str.str());
}

//刷新金币
void UILayerHall::updateSlwhCoin(Ref* ref)
{
	auto info = Player::getInstance();
	SetGold(info->getLogonData().lUserScore);
}

