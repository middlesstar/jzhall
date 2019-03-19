#include "MainView.h"
#include "cocostudio/CocoStudio.h"
#include "shark/resourceDef.h"
#include "HallGameScene.h"
#include "shark/NetWork/SharkNetworkKernel.h"
#include "shark/Module/fish/GameUtils_Shark.h"
#include "shark/GameInfo.h"
#include "shark/Module/SharkNotice.h"
#include "LanguageConfig.h"
#include "shark/Module/ReturnDialog.h"
USING_NS_CC;

#define     Shark_player_out    "playergoAway"

MainView::MainView()
:battle_rootNode(nullptr)
, stake_rootNode(nullptr)
, leftBoard_rootNode(nullptr)
, result_rootNode(nullptr)
, menu_rootNode(nullptr)
, fishManager(nullptr)
, fish(nullptr)
//, uiAction(nullptr)
{
	GameUtils_Shark::winSize = Director::getInstance()->getWinSize();
}
MainView::~MainView()
{
	if (fish)
	{
		fish->removeFish();
		fish->removeAllChildren();
		fishManager->removeAllChildrenWithCleanup(true);
	}
	netDisconnect();

	battle_rootNode = nullptr;
	stake_rootNode = nullptr;
	leftBoard_rootNode = nullptr;
	result_rootNode = nullptr;
	menu_rootNode = nullptr;
	fishManager = nullptr;
	fish = nullptr;

}

//关闭进程时的断开连接的处理
void MainView::netDisconnect()
{
	auto gameinfo = GameInfo::GetInstance();
	gameinfo->setstartGameFinish(false);
	gameinfo->getroomInfor()->vec.clear();
	//用户信息
	SharkUserInfo info = GameInfo::GetInstance()->getuserInfor();
	//发送起立
	SharkNetWorkKernel::getInstance()->SendStandUpPacket(info.wTableID, info.wChairID, TRUE);
	//断开连接
	SharkNetWorkKernel::getInstance()->relese();

}
bool MainView::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto m_visibleSize = Director::getInstance()->getWinSize();
	//摆放摄像机(3d动物层)
	auto ca = Camera::createPerspective(60, (GLfloat)m_visibleSize.width / m_visibleSize.height, 1, 1000);
	ca->setDepth(10);

	ca->setCameraFlag(CameraFlag::USER1);
	ca->setPosition3D(Vec3(0, 0, 0));
	ca->lookAt(Vec3(0, 0, -1));

	if (nullptr != ca)
	{
		cameraManager.AttachCamera(ca);
		addChild(&cameraManager);
	}
	else
	{
		log("%s", a_u8("主摄像机初始化失败"));
	}

	//

	//获取战斗场景的根节点
	battle_rootNode = CSLoader::createNode(COCOS_STUDIO_BATTLSCENE);
	CC_ASSERT(battle_rootNode != nullptr);
	battle_rootNode->setVisible(false);
	addChild(battle_rootNode);

	//获取鱼层背景
	fishManager = battle_rootNode->getChildByName("battle_right")->getChildByName("fishLayer");
	CC_ASSERT(fishManager != nullptr);


	//押分界面的根节点
	stake_rootNode = CSLoader::createNode(COCOS_STUDIO_STAKE);
	CC_ASSERT(stake_rootNode!=nullptr);
	stake_rootNode->setVisible(false);
	addChild(stake_rootNode);

	//获取左边面板信息的根节点
	leftBoard_rootNode = CSLoader::createNode(COCOS_STUDIO_BOARD);
	CC_ASSERT(leftBoard_rootNode != nullptr);
	leftBoard_rootNode->setVisible(true);
	addChild(leftBoard_rootNode);

	//结果界面的根节点
	result_rootNode = CSLoader::createNode(COCOS_STUDIO_RESULT);
	CC_ASSERT(result_rootNode != nullptr);
	result_rootNode->setVisible(false);
	addChild(result_rootNode);


	//菜单界面的根节点
	menu_rootNode = CSLoader::createNode(COCOS_STUDIO_MENU);
	CC_ASSERT(menu_rootNode!=nullptr);
	menu_rootNode->setVisible(true);
	addChild(menu_rootNode);

	//等待界面的根节点
	wait_rootNode = CSLoader::createNode(COCOS_STUDIO_WAIT);
	CC_ASSERT(wait_rootNode!=nullptr);
	wait_rootNode->setVisible(false);
	addChild(wait_rootNode);

	//等待界面管理类
	waitNextManager.dependNode(wait_rootNode->getChildByName(WAITNODE));
	addChild(&waitNextManager);

	//玩家列表管理列表管理
	m_playerListManager.dependNode(leftBoard_rootNode->getChildByName(BET_LEFT));
	addChild(&m_playerListManager);

	//记录板管理
	recordManager.dependNode(stake_rootNode->getChildByName(STAKERIGHT)->getChildByName(RECORDBOARD));
	addChild(&recordManager);

	//动物出现时的背景显示
	animalBgManager.dependNode(battle_rootNode->getChildByName(BATTLERIGHT)->getChildByName("animalBg"));
	addChild(&animalBgManager);

	

	//界面倒计时
	sharkTimeCount.dependNode(menu_rootNode->getChildByName("timeNode"));
	addChild(&sharkTimeCount);
	//押分管理
	stakeManager.dependNode(stake_rootNode->getChildByName(STAKERIGHT));
	addChild(&stakeManager);


	//管理金鲨鱼旋转
	sharkRotaManager.dependNode(battle_rootNode->getChildByName(BATTLERIGHT)->getChildByName("sharkRotate"));
	addChild(&sharkRotaManager);

	//管理银鲨鱼旋转
	silverRotaManager.dependNode(battle_rootNode->getChildByName(BATTLERIGHT)->getChildByName("silverRotate"));
	addChild(&silverRotaManager);

	//光圈跑动
	battleManager.dependNode(battle_rootNode->getChildByName(BATTLERIGHT)->getChildByName(GENELRALBATTLE));
	addChild(&battleManager);

	//获奖倍率显示
	rateManager.dependNode(battle_rootNode->getChildByName(BATTLERIGHT)->getChildByName("rateText"));
	addChild(&rateManager);

	//中奖显示
	resultBoardManager.dependNode(result_rootNode->getChildByName("result_board"));
	addChild(&resultBoardManager);


	//中普通奖

	//中金鲨鱼奖
	goldManager.dependNode(battle_rootNode->getChildByName(BATTLERIGHT)->getChildByName(GOLDSHARK));
	addChild(&goldManager);

	//中银鲨鱼奖
	silverManager.dependNode(battle_rootNode->getChildByName(BATTLERIGHT)->getChildByName(SILVERSHARK));
	addChild(&silverManager);

	//中彩鲨鱼
	colorManager.dependNode(battle_rootNode->getChildByName(BATTLERIGHT)->getChildByName(COLORSHARK));
	addChild(&colorManager);


   //动物管理类
	if (animalManager.AttachSprite3D(PATH_GOLD,PATH_SWALLOW,PATH_DOVE,PATH_PEACOCK,PATH_HAWK, PATH_LION, PATH_PANDA, PATH_MONKEY, PATH_RABBIT))
	{
		this->addChild(&animalManager,10);
	}
	else
		CCLOG("初始化动物失败");
		
	//菜单管理的管理
	menuManager.dependNode(menu_rootNode->getChildByName("menuNode"));
	addChild(&menuManager);

	
	setKeyboardEnabled(true);
	return true;
}


void MainView::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_BACK)
	{
		if (ReturnDialog::GetBOpen())
		{
			ReturnDialog::cancelDialog();
		}
		else
		{
			ReturnDialog::openReturnNotice(GETSTRING(1412), [this](Ref* ref){
				menuManager.goBack();
			});
		}
	}
}


void MainView::showFishLayer()
{
	fish = FishLayer::create();
	fishManager->addChild(fish);
}
void MainView::clearFishLayer()
{
	fish->removeFish();
	fish->removeAllChildren();
	fish = nullptr;
	fishManager->removeAllChildrenWithCleanup(true);
}

//从后台进入前端时调用
void MainView::comeBack(Ref* ref)
{
	menuManager.goBack(-1);
	SharkNotice::openNotice(GETSTRING(1405), [this](Ref*)
	{
		//返回大厅
		Director::getInstance()->replaceScene(CCTransitionFade::create(0.5f, HallGameScene::createScene()));
	});
}
