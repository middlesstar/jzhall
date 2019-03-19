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

//�رս���ʱ�ĶϿ����ӵĴ���
void MainView::netDisconnect()
{
	auto gameinfo = GameInfo::GetInstance();
	gameinfo->setstartGameFinish(false);
	gameinfo->getroomInfor()->vec.clear();
	//�û���Ϣ
	SharkUserInfo info = GameInfo::GetInstance()->getuserInfor();
	//��������
	SharkNetWorkKernel::getInstance()->SendStandUpPacket(info.wTableID, info.wChairID, TRUE);
	//�Ͽ�����
	SharkNetWorkKernel::getInstance()->relese();

}
bool MainView::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto m_visibleSize = Director::getInstance()->getWinSize();
	//�ڷ������(3d�����)
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
		log("%s", a_u8("���������ʼ��ʧ��"));
	}

	//

	//��ȡս�������ĸ��ڵ�
	battle_rootNode = CSLoader::createNode(COCOS_STUDIO_BATTLSCENE);
	CC_ASSERT(battle_rootNode != nullptr);
	battle_rootNode->setVisible(false);
	addChild(battle_rootNode);

	//��ȡ��㱳��
	fishManager = battle_rootNode->getChildByName("battle_right")->getChildByName("fishLayer");
	CC_ASSERT(fishManager != nullptr);


	//Ѻ�ֽ���ĸ��ڵ�
	stake_rootNode = CSLoader::createNode(COCOS_STUDIO_STAKE);
	CC_ASSERT(stake_rootNode!=nullptr);
	stake_rootNode->setVisible(false);
	addChild(stake_rootNode);

	//��ȡ��������Ϣ�ĸ��ڵ�
	leftBoard_rootNode = CSLoader::createNode(COCOS_STUDIO_BOARD);
	CC_ASSERT(leftBoard_rootNode != nullptr);
	leftBoard_rootNode->setVisible(true);
	addChild(leftBoard_rootNode);

	//�������ĸ��ڵ�
	result_rootNode = CSLoader::createNode(COCOS_STUDIO_RESULT);
	CC_ASSERT(result_rootNode != nullptr);
	result_rootNode->setVisible(false);
	addChild(result_rootNode);


	//�˵�����ĸ��ڵ�
	menu_rootNode = CSLoader::createNode(COCOS_STUDIO_MENU);
	CC_ASSERT(menu_rootNode!=nullptr);
	menu_rootNode->setVisible(true);
	addChild(menu_rootNode);

	//�ȴ�����ĸ��ڵ�
	wait_rootNode = CSLoader::createNode(COCOS_STUDIO_WAIT);
	CC_ASSERT(wait_rootNode!=nullptr);
	wait_rootNode->setVisible(false);
	addChild(wait_rootNode);

	//�ȴ����������
	waitNextManager.dependNode(wait_rootNode->getChildByName(WAITNODE));
	addChild(&waitNextManager);

	//����б�����б����
	m_playerListManager.dependNode(leftBoard_rootNode->getChildByName(BET_LEFT));
	addChild(&m_playerListManager);

	//��¼�����
	recordManager.dependNode(stake_rootNode->getChildByName(STAKERIGHT)->getChildByName(RECORDBOARD));
	addChild(&recordManager);

	//�������ʱ�ı�����ʾ
	animalBgManager.dependNode(battle_rootNode->getChildByName(BATTLERIGHT)->getChildByName("animalBg"));
	addChild(&animalBgManager);

	

	//���浹��ʱ
	sharkTimeCount.dependNode(menu_rootNode->getChildByName("timeNode"));
	addChild(&sharkTimeCount);
	//Ѻ�ֹ���
	stakeManager.dependNode(stake_rootNode->getChildByName(STAKERIGHT));
	addChild(&stakeManager);


	//�����������ת
	sharkRotaManager.dependNode(battle_rootNode->getChildByName(BATTLERIGHT)->getChildByName("sharkRotate"));
	addChild(&sharkRotaManager);

	//������������ת
	silverRotaManager.dependNode(battle_rootNode->getChildByName(BATTLERIGHT)->getChildByName("silverRotate"));
	addChild(&silverRotaManager);

	//��Ȧ�ܶ�
	battleManager.dependNode(battle_rootNode->getChildByName(BATTLERIGHT)->getChildByName(GENELRALBATTLE));
	addChild(&battleManager);

	//�񽱱�����ʾ
	rateManager.dependNode(battle_rootNode->getChildByName(BATTLERIGHT)->getChildByName("rateText"));
	addChild(&rateManager);

	//�н���ʾ
	resultBoardManager.dependNode(result_rootNode->getChildByName("result_board"));
	addChild(&resultBoardManager);


	//����ͨ��

	//�н����㽱
	goldManager.dependNode(battle_rootNode->getChildByName(BATTLERIGHT)->getChildByName(GOLDSHARK));
	addChild(&goldManager);

	//�������㽱
	silverManager.dependNode(battle_rootNode->getChildByName(BATTLERIGHT)->getChildByName(SILVERSHARK));
	addChild(&silverManager);

	//�в�����
	colorManager.dependNode(battle_rootNode->getChildByName(BATTLERIGHT)->getChildByName(COLORSHARK));
	addChild(&colorManager);


   //���������
	if (animalManager.AttachSprite3D(PATH_GOLD,PATH_SWALLOW,PATH_DOVE,PATH_PEACOCK,PATH_HAWK, PATH_LION, PATH_PANDA, PATH_MONKEY, PATH_RABBIT))
	{
		this->addChild(&animalManager,10);
	}
	else
		CCLOG("��ʼ������ʧ��");
		
	//�˵�����Ĺ���
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

//�Ӻ�̨����ǰ��ʱ����
void MainView::comeBack(Ref* ref)
{
	menuManager.goBack(-1);
	SharkNotice::openNotice(GETSTRING(1405), [this](Ref*)
	{
		//���ش���
		Director::getInstance()->replaceScene(CCTransitionFade::create(0.5f, HallGameScene::createScene()));
	});
}
