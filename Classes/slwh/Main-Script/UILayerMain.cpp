#include "UILayerMain.h"
#include "slwh/Module/GoldFallen3D.h"
#include "slwh/Module/SLWHGoldFallen.h"
#include "slwh/Module/SLWHGoldHeap.h"
#include "slwh/ResourcePath.h"
#include "cocostudio/CocoStudio.h"
#include "UI/MyJetton/OneOfJetton.h"
#include "UI/TitleLabel.h"
#include "UI/UIMenu.h"
#include "slwh/Module/SLWH_CD.h"
#include "HallGameScene.h"
#include "slwh/Hall-Script/UILayerHall.h"
#include "slwh/Module/SLWHReturn.h"
#include "LanguageConfig.h"
using namespace ui;


#define		BET_RIGHT							"bet_right"
#define		BET_LEFT							"bet_left"
#define		BET_OPEN							"open"


#define		RES_RECORD							"Res_Record"			//记录板
#define		_TITLE								"Title"					//标题节点
#define		USER_INFO							"UserInfo"				//用户信息
#define		_ZHX								"ZHX"					//庄和闲（滚动）
#define		TIME_NUM							"TimeNum"				//游戏开始倒计时
#define		_MENU								"Menu"					//菜单节点
#define		_JC									"JC"					//奖池
#define		_REGULAR							"Regular"				//规则界面
#define		_WAIT_NODE							"wait_node"				//等待开奖结束

UILayerMain::UILayerMain():
m_rootNode(NULL),
Jetroot(NULL),
listroot(NULL),
openroot(NULL),
jetbt(NULL),
m_bEnble(true)
{
}

UILayerMain::~UILayerMain()
{
	m_rootNode = nullptr;
	Jetroot = nullptr;
	listroot = nullptr;
	openroot = nullptr;
	jetbt = nullptr;
	TitleLabel::GetInstance()->relese();
}

bool UILayerMain::init()
{

	if (!Layer::init())
	{
		return false;
	}

	//获取COCOS_STUDIO主场景UI根节点
	m_rootNode = CSLoader::createNode(COCOS_STUDIO_GAMESCENE);
	CC_ASSERT(m_rootNode != nullptr);
	addChild(m_rootNode);

	//管理标题UI
	TitleLabel* t = TitleLabel::GetInstance();
	t->AttachNode(m_rootNode->getChildByName(_TITLE));
	addChild(t, 500);

	//管理规则UI
	Regular* reg = Regular::GetInstance();
	reg->AttachNode(m_rootNode->getChildByName(_REGULAR));
	addChild(reg);

	//管理用户信息UI
	m_uiUserInfo.AttachNode(m_rootNode->getChildByName(USER_INFO));
	addChild(&m_uiUserInfo);

	//管理等待界面
	m_waitNextJet.AttachNode(m_rootNode->getChildByName(_WAIT_NODE));
	addChild(&m_waitNextJet);

	//管理时间UI
	/*m_timeCount.AttachNode(m_rootNode->getChildByName(TIME_NUM));
	addChild(&m_timeCount);*/

	//管理记录板UI
	m_resRecord.AttachNode(m_rootNode->getChildByName(RES_RECORD));
	addChild(&m_resRecord);

	//管理奖池UI
	m_jcNum.AttachNode(m_rootNode->getChildByName(_JC));
	addChild(&m_jcNum);

	//管理菜单UI
	m_uiMenu.AttachNode(m_rootNode->getChildByName(_MENU));
	addChild(&m_uiMenu);

	//管理庄和闲UI
	m_zhxControl.AttachNode(m_rootNode->getChildByName(_ZHX));
	addChild(&m_zhxControl);
	
	//管理压分UI
	Jetroot = CSLoader::createNode(COCOS_STUDIO_JET_PANEL);
	CC_ASSERT(Jetroot != nullptr);
	m_myJetManager.AttachNode(Jetroot->getChildByName(BET_RIGHT));
	addChild(&m_myJetManager);
	Jetroot->setVisible(true);

	//管理玩家列表
	listroot = CSLoader::createNode(COCOS_STUDIO_USER_LIST);
	CC_ASSERT(listroot != nullptr);
	m_userListManager.AttachNode(listroot->getChildByName(BET_LEFT));
	addChild(&m_userListManager);
	listroot->setVisible(true);

	//管理开奖UI
	openroot = CSLoader::createNode(COCOS_STUDIO_OPEN);
	CC_ASSERT(openroot != nullptr);
	m_resManager.AttachNode(openroot);
	addChild(&m_resManager);
	openroot->setVisible(true);

	//管理压分按钮
	jetbt = CSLoader::createNode(COCOS_STUDIO_UPBT);
	CC_ASSERT(jetbt != nullptr);
	addChild(jetbt);
	m_jetandresManager.AttachNode(jetbt);
	addChild(&m_jetandresManager);
	m_jetandresManager.Attach3Nodes(openroot, Jetroot, listroot);


	//允许UI层接收触摸事件  
	this->setTouchEnabled(true);
	EventDispatcher* eventDispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(UILayerMain::onTouchBegan, this);
	listener->setSwallowTouches(true);
	eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	setKeyboardEnabled(true);

	//非常不愿意做的事情，要牵线搭桥了，开奖界面的金币飞行结束要促发 用户列表金币抖动
	//m_resManager.attachBetgold(m_userListManager.GetBetGold());

	//测试
	//addChild(GoldFallen::GetInstance());

	//m_timeCount.StartPlay(10);
	//info->Init("Dais", 5, 10000, 5);

	//auto gold = SLWH_GoldHeap::create(30);
	//addChild(gold);
	//gold->setPosition(Vec2(300,300));
	//gold->StartPlay(30,100);

	return true;
}

//void UILayerMain::OutBetEvent(Ref *pSender, TouchEventType type)
//{
//
//	if (type == TOUCH_EVENT_ENDED)
//	{
//		m_myJetManager.MyJettonOut();
//	} 
//}

/*-------------todosl-----------------*/
//void UILayerMain::onEnter()
//{
//
//	Layer::onEnter();
//}
//
//
//
//void UILayerMain::onExit()
//{
//	Layer::onExit();
//}

bool UILayerMain::onTouchBegan(Touch* touch, Event* event)
{

	//if (!m_bEnble)
	//{
	//	//TitleLabel::SetTitle(TitleLabel::Normal3);
	//}

	if (OneGameInfo::GetInstance()->getServerState() == _Jet)
	{
		m_jetandresManager.MyJettonOut();
	}
	

	//测试
	//GoldFallen3D::StartPlay();
	//GoldFallen3D::StartPlay();
	//SLWH_GoldFallen::StartPlay();
	//SLWH_CD::StartPlay();
	//测试
	//int i[] = { 1, 2, 3, 4 };
	//m_XYCJManager.StartPlay(i, 4);
	//m_speCJ.StartPlay("Dais",321321);
	//m_XYCJResManager.StartPlay();

	return true;
}

void UILayerMain::SetEnble(bool b)
{
	if (m_bEnble == b) return;
	m_bEnble = b;
	m_myJetManager.enable(b);
}

void UILayerMain::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_BACK)
	{
		if (SLWHReturn::GetBOpen())
		{
			SLWHReturn::cancelDialog();
		}
		else
		{
			SLWHReturn::openReturnNotice(GETSTRING(1412), [this](Ref* ref){
				m_uiMenu.exit();
			});
		}
	}
}

