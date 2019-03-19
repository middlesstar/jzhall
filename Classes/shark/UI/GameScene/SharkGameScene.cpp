#include "SharkGameScene.h"
#include "shark/GameInfo.h"
#include "shark/resourceDef.h"
#include "HallGameScene.h"
#include "shark/Module/SharkNotice.h"
#include "LanguageConfig.h"
#include "shark/NetWork/SharkNetworkKernel.h"
#include "Window/chat/LogicChat.h"
#include "df/ServerListData.h"
#include "shark/UI/SelectRoom/SelectRoom.h"

#define     shark_call_netWork                  "callGameServer"
#define     game_comeToBack                     "comeToBackground"
#define     game_comefromBack                   "comeFromBackground"


static int general[20] = { 0, 1, 3, 4, 6, 7, 8, 10, 11, 12, 14, 15, 17, 18, 20, 21, 22, 24, 25, 26 }; //��ͨ��������
static int goldShark[3] = { 2, 9, 16};                                                        //��������
static int silverShark[3] = { 13, 19, 27 };                                                   //��������
static int color[2] = { 5, 23 }; 

//10�붨ʱ����Ƿ���������Ӧ
#define _HEART_TIME		10	//������ʱ��
#define _CHECK_TIME		3  //��ɫ��������

MainView*  SharkGameScene::m_mainView = nullptr;
SharkGameScene*  SharkGameScene::This = nullptr;

bool SharkGameScene::init()
{
	if (!Scene::init())
	{
		return false;
	}

	resultIndex = 0;
	//outTime = 0;
	return true;
}

void SharkGameScene::onEnter()
{
	This = this;
	Node::onEnter();

	//ע������̨��ǰ�˵Ĵ����¼�
	HALL_NOTIFICATION_REG(game_comeToBack,SharkGameScene::comeToBack, nullptr);
	HALL_NOTIFICATION_REG(game_comefromBack, SharkGameScene::comeFromBack, nullptr);
}


void SharkGameScene::onExit()
{
	Node::onExit();
	m_mainView = nullptr;
	This = nullptr;
	SimpleAudioEngine::getInstance()->stopAllEffects();


	HALL_NOTIFICATION_UNREG(game_comeToBack);
	HALL_NOTIFICATION_UNREG(game_comefromBack);

}

void SharkGameScene::comeToBack(Ref* ref)
{
	//time(&outTime);
	//���Ѻ��
	if (GameInfo::GetInstance()->getserverState() == gameState::BATTLE)
	{
		if (m_nowIndex != 0)
		{
			unschedule(schedule_selector(SharkGameScene::bridge));
		}
		
	}
	chat->removeFromParentAndCleanup(true);
	m_mainView->stakeManager.playerOut(nullptr);
	m_mainView->removeFromParentAndCleanup(true);
	m_mainView = nullptr;


	unschedule(schedule_selector(SharkGameScene::waitUpdate));
	unschedule(schedule_selector(SharkGameScene::updateSendHreat));
}

void SharkGameScene::comeFromBack(Ref* ref)
{
	///*---------------------------------*/
	//time_t currentTime;
	//time(&currentTime);
	//time_t disTime = currentTime - outTime;
	//log("------------disTime = %d-------", disTime);
	//if ((disTime / 60) > 5)
	//{
	//	Director::getInstance()->replaceScene(SelectRoom::createScene());
	//}
	//else
	//{
		auto gameInfo = GameInfo::GetInstance();
		HallGameServerItem* serverinfo = HallServerListData::shared()->getGameServerInfo(KIND_ID_SHARK, gameInfo->getroomIndex());

		SharkNetWorkKernel::getInstance()->setUrl(serverinfo->m_GameServer.szServerAddr, serverinfo->m_GameServer.wServerPort);
		//���ӷ�����
		SharkNetWorkKernel::getInstance()->start();
		SharkNetWorkKernel::getInstance()->SendLogonPacket();

		timeDelay = 0;
		schedule(schedule_selector(SharkGameScene::goMainScene), 0.1);
	//}	

}


void SharkGameScene::startTimer(float time)
{
	schedule(schedule_selector(SharkGameScene::waitUpdate), 0.1f);
}



void SharkGameScene::onEnterTransitionDidFinish()
{
	HALL_NOTIFICATION(shark_call_netWork,nullptr);
	

	timeDelay = 0;
	schedule(schedule_selector(SharkGameScene::goMainScene),0.1);

}



//������Ϸ������
void SharkGameScene::goMainScene(float t)
{
	timeDelay++;
	if (!GameInfo::GetInstance()->getstartGameFinish()) return;

	unschedule(schedule_selector(SharkGameScene::goMainScene));
	
	m_mainView = MainView::create();
	CC_ASSERT(m_mainView != nullptr);
	addChild(m_mainView);


	//����������
	updateSendHreat(0.f);
	schedule(schedule_selector(SharkGameScene::updateSendHreat), _HEART_TIME);

	auto gameInfo = GameInfo::GetInstance();
	m_nowState = gameInfo->getserverState();

	
	//��������
	gameInfo->selectMusicBack();
	m_nowIndex = 0;
	//����ѹ�ֽ���ʽ�
	m_mainView->stakeManager.setCaijin(gameInfo->getroomInfor()->lCJ);

	//������Ϸ����ʽ�����
	m_mainView->battleManager.setColorMoney(gameInfo->getroomInfor()->lCJ);

	//������ʱ�� �ȴ������֪ͨ��ʼ�µ�һ����Ϸ

	//��ʾ��¼��
	m_mainView->recordManager.updateBoard(1);
	
	
	if (m_nowState != gameState::STAKE)		//1 = Ѻע�׶�
	{
		m_mainView->battle_rootNode->setVisible(true);
		m_mainView->m_playerListManager.changeView(2);
		if (gameInfo->getLeftTime()>timeDelay)
		{
			waitNextGame(gameInfo->getLeftTime() - timeDelay);
			scheduleOnce(schedule_selector(SharkGameScene::startTimer),gameInfo->getLeftTime());
		}		
		
	}
	else
	{
		//���ٵȴ�����
		//m_mainView->waitNextManager.destory();
		//��ʼѺע
		m_mainView->stake_rootNode->setVisible(true);
		m_mainView->m_playerListManager.changeView(1);
		if (gameInfo->getLeftTime() > timeDelay)
		{
			gameStart(gameInfo->getLeftTime() - timeDelay);
		}	
		schedule(schedule_selector(SharkGameScene::waitUpdate), 0.1f);
	}
	
	//��������б�
	m_mainView->m_playerListManager.InitPlayerList();
	//���¸�����Ϣ
	m_mainView->m_playerListManager.updateInfo();

	/*----------------��������Ľڵ�----------------------*/

	chat = Node::create();
	this->addChild(chat,10);
	//��������
	HallGameScene::openChatSprite(chat);
	/*--------------------------------------------------------*/

	
}


void SharkGameScene::gameStart(int leftTime)
{
	//������ҶѶ�ʱ��
	m_mainView->stakeManager.startGoldheap();
	//�ı䵹��ʱ��λ��
	m_mainView->m_playerListManager.changeView(1);
	

	//��˸��ť�ϵĹ�Ȧ
	if (leftTime > 5)
	{
		m_mainView->stakeManager.showCircle();
	}
	
	auto gameInfo = GameInfo::GetInstance();

	//���òʽ�����
	m_mainView->battleManager.setColorMoney(gameInfo->getroomInfor()->lCJ);

	//����ѹ�ֽ���ʽ�
	m_mainView->stakeManager.setCaijin(gameInfo->getroomInfor()->lCJ);
	//���ñ���
	m_mainView->stakeManager.SetRate(gameInfo->getRate());
	//�������������Ѻ��
	m_mainView->m_playerListManager.cancelTotleNum();

	//���Ѻע��Ϣ
	m_mainView->stakeManager.ClearJetton();

	
	if (m_mainView->stakeManager.GetAI())
	{
			m_mainView->stakeManager.ContinuStake();
	}

	//��ʼ����ʱ
	m_mainView->sharkTimeCount.StartPlay(leftTime, 1);

	for (std::vector<UserInfo_Shark>::iterator itor = gameInfo->getroomInfor()->vec.begin(); itor != gameInfo->getroomInfor()->vec.end(); itor++)
	{
		(*itor).winscore = _ZERO;
	}
	m_mainView->m_playerListManager.InitPlayerList();
	

}

void SharkGameScene::waitUpdate(float t)
{
	//��ȡ����������
	gameState serverState = GameInfo::GetInstance()->getserverState();
	if (m_nowState == serverState) return;
	m_nowState = serverState;
	//����Ϣ������ �����Ǻõ�
	//m_bIslive = true;
	switch (m_nowState)
	{

	case gameState::STAKE:
		//��������
		GameInfo::GetInstance()->selectMusicBack();
		//�⿪Ѻ�ְ�ť�Ĵ�������
		m_mainView->stakeManager.setTouch(true);
		m_mainView->battle_rootNode->setVisible(false);
		m_mainView->stake_rootNode->setVisible(true);	
		//ˢ�����Ͻ���Ϣ
		m_mainView->m_playerListManager.updateInfo();
		gameStart(30);
		break;
	case gameState::BATTLE:
		//����Ѻ�ְ�ť�Ĵ�������
		m_mainView->stakeManager.setTouch(false);
		m_mainView->m_playerListManager.changeView(2);
		m_mainView->battle_rootNode->setVisible(true);
		m_mainView->stake_rootNode->setVisible(false);
		openPrize(30);
		break;
	default:
		break;
	}
}
//��ʼ��Ϸ
void SharkGameScene::openPrize(int leftTime)
{
	_STOP_MUSCI_SHARK;
	//��ԭ��Ҷ�
	m_mainView->stakeManager.hideGoldheap();
	log("=-------------time = %d",leftTime);
	m_mainView->sharkTimeCount.StartPlay(leftTime,2);
	scheduleOnce(schedule_selector(SharkGameScene::bridge), 0.05f);		//��ͣ��1��ȴ�ʱ����Ч����(slwh)
}

void SharkGameScene::bridge(float t)
{
	scheduleOnce(schedule_selector(SharkGameScene::gameRunning),0.05f);
}

//�ȴ���һ����Ϸ
void SharkGameScene::waitNextGame(int leftTime)
{

	log("----------timeCount = %d",leftTime);
	m_mainView->sharkTimeCount.StartPlay(leftTime, 2);
	//CheckNet(leftTime);
	//��ʱ��ɭ�����Ĵ���취
	m_mainView->wait_rootNode->setVisible(true);
	m_mainView->waitNextManager.StartPlay(leftTime);
	
}

void SharkGameScene::gameRunning(float t)
{
	//�ύ��Ϸѹ�����ݵ�������
	//�õ��������
	auto gameInfo = GameInfo::GetInstance();
	int index = gameInfo->getopenRelt().resultIndex[0];
	log("----------------resultIndex = %d", index);
	//��ͨ�����ж�
	if (index > -1 && index < 28)
	{
		for (int i = 0; i < 20; i++)
		{
			if (general[i] == index)
			{
				resultIndex = 1;
				break;
			}
		}
		//���迪���ж�
		for (int i = 0; i < 3; i++)
		{
			if (silverShark[i] == index)
			{
				resultIndex = 2;
				break;
			}
			
		}
		//���迪���ж�
		for (int i = 0; i < 3; i++)
		{
			if (goldShark[i] == index)
			{
				resultIndex = 3;				
				break;
			}
		}
		//�����㿪���ж�
		for (int i = 0; i < 2; i++)
		{
			if (color[i] == index)
			{
				resultIndex = 4;
				break;
			}
		}
	}
	
	//��¼ִ�е���һ����
	m_nowIndex++;
	log("------------nowIndex = %d--------------",m_nowIndex);
	log("-------------resultIndex = %d----------", resultIndex);
	switch (resultIndex)
	{
	case resultKind::GENERALRESULT :
		generalResult(m_nowIndex); break;
	case resultKind::SILVERRESULT:
		silverResult(m_nowIndex); break;
	case resultKind::SHARKRESULT :
		sharkResult(m_nowIndex); break;
	case resultKind::COLORRESULT:
		colorShark(m_nowIndex); break;
	default:
		break;
	}
}

//��ͨ����
void SharkGameScene::generalResult(int index)
{
	auto gameInfo = GameInfo::GetInstance();
	if (1 == index)
	{
		//�㿪ʼ�ζ�
		m_mainView->showFishLayer();
		//ͳ��Ѻ��
		m_mainView->stakeManager.SaveLastGame();
		m_mainView->m_playerListManager.showStakeInfo();
		scheduleOnce(schedule_selector(SharkGameScene::bridge), 0.05f);
	}
	else if (2 == index)
	{
		//���Ź�Ȧ�ܶ�����
		_PLAY_MUSCI_SHARK(TURN_MUSIC, true);
		int stopIndex = gameInfo->getcircleFlag();
		log("--------------stopIndex = %d----------", stopIndex);
		//���ù�Ȧ��ʼ��ֹͣ��λ��
		m_mainView->battleManager.setCirclePos(stopIndex, gameInfo->getopenRelt().resultIndex[0]);
		//��Ϸ�׶�
		m_mainView->battleManager.startPlay();
		this->scheduleOnce(schedule_selector(SharkGameScene::bridge), 10.5f);
	}
	else if (3 == index)
	{
		//������ζ�
		m_mainView->clearFishLayer();
		judgeResult(gameInfo->getopenRelt().resultIndex[0], false);
		//��ʾ����
		showRate();
		//ת����ͨ����
		//m_mainView->goldManager.startPlay();
		this->scheduleOnce(schedule_selector(SharkGameScene::bridge), 4.0f);
	}
	else if (4 == index)
	{
		//��Ȧ��ʾ
		m_mainView->battleManager.stopPlay();
		stopResult(gameInfo->getopenRelt().resultIndex[0], false);
		hideRate();
        
		battleResult();
		this->scheduleOnce(schedule_selector(SharkGameScene::bridge), 8.3f);
	}
	else if (5 == index)
	{
		changeView();
	}
	else if (6 == index)
	{

	}
	else if (7 == index)
	{

	}
}

//�����㿪��
void SharkGameScene::silverResult(int index)
{
	auto gameInfo = GameInfo::GetInstance();
	if (1 == index)
	{
		//�㿪ʼ�ζ�
		m_mainView->showFishLayer();
		//ͳ��Ѻ������
		m_mainView->stakeManager.SaveLastGame();
		m_mainView->m_playerListManager.showStakeInfo();

		//��ʾ������ת
		m_mainView->silverRotaManager.startPlay();
		scheduleOnce(schedule_selector(SharkGameScene::bridge), 4.0f);
	}
	else if (2 == index)
	{
		//���Ź�Ȧ�ܶ�����
		_PLAY_MUSCI_SHARK(TURN_MUSIC,true);
		//ֹͣ������ת
		m_mainView->silverRotaManager.stopPlay();
		int stopIndex = gameInfo->getcircleFlag();
		log("--------------stopIndex = %d----------", stopIndex);
		//���ù�Ȧ��ʼ��ֹͣ��λ��
		m_mainView->battleManager.setCirclePos(stopIndex, gameInfo->getopenRelt().resultIndex[0]);
		//��Ϸ�׶�
		m_mainView->battleManager.startPlay();
		this->scheduleOnce(schedule_selector(SharkGameScene::bridge), 10.5f);
	}
	else if (3 == index)
	{
		//������ζ�
		m_mainView->clearFishLayer();
		//ת�������㣬��ʾ����ʱ��Ч��
		judgeResult(gameInfo->getopenRelt().resultIndex[0], false);
		showRate();
		this->scheduleOnce(schedule_selector(SharkGameScene::bridge), 2.0f);
				
	}
	else if (4 == index)
	{
		//��ʾ��ǹЧ��3��
		m_mainView->colorManager.startShoot();
		this->scheduleOnce(schedule_selector(SharkGameScene::bridge), 4.0f);
	}
	else if (5 == index)
	{
		//��ʾ����ʱ��
		m_mainView->silverManager.startPlay();
		stopResult(5, false);
		hideRate();
		//���û�ת��λ��
		m_mainView->battleManager.setRotationPos(gameInfo->getopenRelt().resultIndex[1]);

		m_mainView->battleManager.rotation();
		this->scheduleOnce(schedule_selector(SharkGameScene::bridge), 1.0f);
		
	}
	else if (6 == index)
	{
		//��������
		judgeResult(gameInfo->getopenRelt().resultIndex[1], false);
		showRate();
		this->scheduleOnce(schedule_selector(SharkGameScene::bridge), 3.0f);

	}
	else if (7 == index)
	{
		hideRate();
		//�رյƹ�
		m_mainView->silverManager.stopPlay();
		stopResult(gameInfo->getopenRelt().resultIndex[0], false);
		stopResult(gameInfo->getopenRelt().resultIndex[1], false);
		m_mainView->battleManager.stopPlay();
		
		battleResult();
		this->scheduleOnce(schedule_selector(SharkGameScene::bridge), 7.35f);
	}
	else if (8 == index)
	{
		changeView();
	}
}
//�����㿪��
void SharkGameScene::sharkResult(int index)
{
	auto gameInfo = GameInfo::GetInstance();
	if (1 == index)
	{		
		//�㿪ʼ�ζ�
		m_mainView->showFishLayer();
		//ͳ��Ѻ������
		m_mainView->stakeManager.SaveLastGame();
		m_mainView->m_playerListManager.showStakeInfo();

		//��ʾ������ת
		m_mainView->sharkRotaManager.startPlay();
		scheduleOnce(schedule_selector(SharkGameScene::bridge), 4.0f);
	}
	else if (2 == index)
	{

		//���Ź�Ȧ�ܶ�����
		_PLAY_MUSCI_SHARK(TURN_MUSIC, true);
		//ֹͣ������ת
		m_mainView->sharkRotaManager.stopPlay();
		int stopIndex = gameInfo->getcircleFlag();
		log("--------------stopIndex = %d----------", stopIndex);
		//���ù�Ȧ��ʼ��ֹͣ��λ��
		m_mainView->battleManager.setCirclePos(stopIndex, gameInfo->getopenRelt().resultIndex[0]);
		//��Ϸ�׶�
		m_mainView->battleManager.startPlay();
		this->scheduleOnce(schedule_selector(SharkGameScene::bridge),10.5f);
	}
	else if (3 == index)
	{		
		//������ζ�
		m_mainView->clearFishLayer();
		//ת�����裬��ʾ������Ч
		judgeResult(gameInfo->getopenRelt().resultIndex[0], false);
		showRate();
		this->scheduleOnce(schedule_selector(SharkGameScene::bridge), 2.0f);
	}
	else if (4 == index)
	{
		//��ʾ��ǹЧ��3��
		m_mainView->colorManager.startShoot();
		this->scheduleOnce(schedule_selector(SharkGameScene::bridge), 4.0f);	
		
	}
	else if (5 == index)
	{
		//��ʾ������Ч
		m_mainView->goldManager.startPlay();

		stopResult(23, false);
		hideRate();
		//���û�ת��λ��
		m_mainView->battleManager.setRotationPos(gameInfo->getopenRelt().resultIndex[1]);
		m_mainView->battleManager.rotation();
		//�򿪻Ƶƣ�ת������
		this->scheduleOnce(schedule_selector(SharkGameScene::bridge), 1.0f);
		
	}
	else if (6 == index)
	{
		//��������
		judgeResult(gameInfo->getopenRelt().resultIndex[1], false);
		showRate();
		this->scheduleOnce(schedule_selector(SharkGameScene::bridge), 3.0f);	
	
	}
	else if (7 == index)
	{
		hideRate();
		m_mainView->goldManager.stopPlay();
		stopResult(gameInfo->getopenRelt().resultIndex[1], false);
		stopResult(gameInfo->getopenRelt().resultIndex[0], false);
		m_mainView->battleManager.stopPlay();		
		
		battleResult();
		this->scheduleOnce(schedule_selector(SharkGameScene::bridge), 7.35f);	

	}
	else if (8 == index)
	{
		changeView();
	}
}

//�����㿪��
void SharkGameScene::colorShark(int index)
{
	auto gameInfo = GameInfo::GetInstance();
	if (index == 1)
	{
		//�㿪ʼ�ζ�
		m_mainView->showFishLayer();
		//Ѻ�ֱ���
		m_mainView->stakeManager.SaveLastGame();
		m_mainView->m_playerListManager.showStakeInfo();

		//��ʾ������ת
		m_mainView->sharkRotaManager.startPlay();
		scheduleOnce(schedule_selector(SharkGameScene::bridge), 2.05f);
	}
	else if (index == 2)
	{
		//���Ź�Ȧ�ܶ�����
		_PLAY_MUSCI_SHARK(TURN_MUSIC, true);
		//ֹͣ������ת
		m_mainView->sharkRotaManager.stopPlay();

		int stopIndex = gameInfo->getcircleFlag();
		log("--------------stopIndex = %d----------", stopIndex);
		//���ù�Ȧ��ʼ��ֹͣ��λ��
		m_mainView->battleManager.setCirclePos(stopIndex, gameInfo->getopenRelt().resultIndex[0]);
		//��Ϸ�׶�
		m_mainView->battleManager.startPlay();
		this->scheduleOnce(schedule_selector(SharkGameScene::bridge), 10.0f);
	}
	else if (index == 3)
	{
		//������ζ�
		m_mainView->clearFishLayer();
		judgeResult(gameInfo->getopenRelt().resultIndex[0], false);
		showRate();
		this->scheduleOnce(schedule_selector(SharkGameScene::bridge), 3.0f);		
	}
	else if (index == 4)
	{
		//��ʾ��ǹЧ��3��
		m_mainView->colorManager.startColor();
		this->scheduleOnce(schedule_selector(SharkGameScene::bridge), 3.0f);

	}
	else if (index == 5)
	{
		//��ʾ�ʽ�ʱ��
		m_mainView->colorManager.startPlay();
		stopResult(gameInfo->getopenRelt().resultIndex[0], false);
		hideRate();
		//���ý���ͼƬ����
		//���û�ת��λ��
		m_mainView->battleManager.changeImg(gameInfo->getopenRelt().aType);
		m_mainView->battleManager.setRotationPos(gameInfo->getopenRelt().resultIndex[1]);
		m_mainView->battleManager.rotation();
		this->scheduleOnce(schedule_selector(SharkGameScene::bridge), 2.0f);

	}
	else if (index == 6)
	{
		judgeResult(gameInfo->getopenRelt().resultIndex[1], true);
		showRate();
		this->scheduleOnce(schedule_selector(SharkGameScene::bridge), 2.0f);

	}
	else if (index == 7)
	{
		stopResult(gameInfo->getopenRelt().resultIndex[1], true);
		hideRate();
		//�ٻ�תһ��
		m_mainView->battleManager.setRotationPos(gameInfo->getopenRelt().resultIndex[2]);
		m_mainView->battleManager.rotation();
		this->scheduleOnce(schedule_selector(SharkGameScene::bridge), 2.0f);
	
	}
	else if (index == 8)
	{
		judgeResult(gameInfo->getopenRelt().resultIndex[2], true);
		showRate();
		this->scheduleOnce(schedule_selector(SharkGameScene::bridge), 3.0f);

	}
	else if (index == 9)
	{
		stopResult(gameInfo->getopenRelt().resultIndex[2], true);
		hideRate();
		m_mainView->battleManager.stopPlay();
		//����ԭ����ͼƬ
		m_mainView->battleManager.reChangeImg();
		//�ʽ������رղʵ�
		m_mainView->colorManager.stopPlay();
		
		battleResult();
		this->scheduleOnce(schedule_selector(SharkGameScene::bridge), 3.15f);		
	}
	else if (9 == index)
	{
		changeView();
		
	}
}


bool SharkGameScene::AddGold(int g)
{
	auto info = Player::getInstance();
	if (g < _ZERO && info->getLogonData().lUserScore + g < _ZERO)
		return false;

	info->addCoin(g,false);

	return true;
}


//��ʾ�����ܽ����
void SharkGameScene::showResult()
{
	m_mainView->battle_rootNode->setVisible(false);
	m_mainView->stake_rootNode->setVisible(true);
	m_mainView->result_rootNode->setVisible(true);
	//m_mainView->sharkTimeCount.changeTimeCount();

	m_mainView->recordManager.updateBoard();
	showAnimal();
}
//�����������һ������
void SharkGameScene::battleResult()
{
	auto gameInfo = GameInfo::GetInstance();	
	//�˴��߼����Ͻ�������ҵ÷�Ϊ0���۲����н�ҹ���
	SharkGameScene::m_mainView->m_playerListManager.updatePlayerList(gameInfo->getroomInfor()->vec);
	SharkGameScene::m_mainView->m_playerListManager.updateInfo(1);
	m_mainView->resultBoardManager.startPlay();
	m_mainView->m_playerListManager.changeView(1);
	//����������ʾ
	showResult();
}

//������������л���Ѻ�ֽ���
void SharkGameScene::changeView()
{
	m_mainView->resultBoardManager.stopPlay();
	m_mainView->m_playerListManager.deleteOwnStake();
	m_mainView->result_rootNode->setVisible(false);
	m_nowIndex = _ZERO;

	stopResAnimal();
}

//�жϿ���������ʾ��һ������
void  SharkGameScene::showAnimal()
{
	auto gameInfo = GameInfo::GetInstance();

	int *newIndex = gameInfo->getResRecord();
	int i = newIndex[0];
	int index =0 ;
	if (i < 3)
	{
		index = i ;
	}
	else
	{
		index = i - 1;
	}
	switch (index)
	{
	case ANIMAL_GOLD:
		m_mainView->animalManager.startAnimal(2, ANIMAL_GOLD, PATH_GOLD);
		break;
	case ANIMAL_SILVER:
		m_mainView->animalManager.startAnimal(2, ANIMAL_SILVER, PATH_SILVER);
		break;
	case ANIMAL_SWALLOW:
		m_mainView->animalManager.startAnimal(2, ANIMAL_SWALLOW, PATH_SWALLOW);
		break;
	case ANIMAL_DOVE:
		m_mainView->animalManager.startAnimal(2, ANIMAL_DOVE, PATH_DOVE);
		break;
	case ANIMAL_PEACOCK:
		m_mainView->animalManager.startAnimal(2, ANIMAL_PEACOCK, PATH_PEACOCK);
		break;
	case ANIMAL_HAWK:
		m_mainView->animalManager.startAnimal(2, ANIMAL_HAWK, PATH_HAWK);
		break;
	case ANIMAL_LION:
		m_mainView->animalManager.startAnimal(2, ANIMAL_LION, PATH_LION);
		break;
	case ANIMAL_PANDA:
		m_mainView->animalManager.startAnimal(2, ANIMAL_PANDA, PATH_PANDA);
		break;
	case ANIMAL_MONKEY:
		m_mainView->animalManager.startAnimal(2, ANIMAL_MONKEY, PATH_MONKEY);
		break;
	case ANIMAL_RABBIT:
		m_mainView->animalManager.startAnimal(2, ANIMAL_RABBIT, PATH_RABBIT);
		break;
	default:
		break;
	}  
}

//������������Ķ���
void SharkGameScene::stopResAnimal()
{
	auto gameInfo = GameInfo::GetInstance();

	int *newIndex = gameInfo->getResRecord();
	int i = newIndex[0];
	int index = 0;
	if (i < 3)
	{
		index = i;
	}
	else
	{
		index = i - 1;
	}

	m_mainView->animalManager.stopAnimal(index);
}

//�ж�����һ������
void SharkGameScene::judgeResult(int index,bool colorsharFlag)
{
	
	auto gameInfo = GameInfo::GetInstance();	
	int* rate;
	rate = gameInfo->getRate();
	
	if (index == 0 || index == 1)
	{
		//��è
		//���ñ�����ʾ
		m_mainView->rateManager.setBeiLv(rate[9]);
		m_mainView->animalManager.startAnimal(1, ANIMAL_PANDA, PATH_PANDA);
		_PLAY_SOUND_SHARK(PANDA_MUSIC);

		//��ʾ����ͼƬ
		m_mainView->animalBgManager.startPlay();
	
	}
	else if (index == 3 || index == 4)
	{
		//����
		//���ñ�����ʾ
		m_mainView->rateManager.setBeiLv(rate[10]);
		m_mainView->animalManager.startAnimal(1, ANIMAL_MONKEY, PATH_MONKEY);
		_PLAY_SOUND_SHARK(MONKEY_MUSIC);

		//��ʾ����ͼƬ
		m_mainView->animalBgManager.startPlay();
	}
	
	else if (index == 6 || index == 7 || index == 8)
	{		
		//����
		//���ñ�����ʾ
		m_mainView->rateManager.setBeiLv(rate[11]);
		m_mainView->animalManager.startAnimal(1, ANIMAL_RABBIT, PATH_RABBIT);
		_PLAY_SOUND_SHARK(RABBIT_MUSIC);

		//��ʾ����ͼƬ
		m_mainView->animalBgManager.startPlay();
	}
	else if (index == 10 || index == 11 || index == 12)
	{		
		//����
		//���ñ�����ʾ
		m_mainView->rateManager.setBeiLv(rate[4]);
		m_mainView->animalManager.startAnimal(1, ANIMAL_SWALLOW,PATH_SWALLOW);
		_PLAY_SOUND_SHARK(SWALLOW_MUSIC);

		//��ʾ����ͼƬ
		m_mainView->animalBgManager.startPlay();
	}
	else if (index == 14 || index == 15)
	{
		//����
		//���ñ�����ʾ
		m_mainView->rateManager.setBeiLv(rate[5]);
		m_mainView->animalManager.startAnimal(1, ANIMAL_DOVE, PATH_DOVE);
		_PLAY_SOUND_SHARK(DOVE_MUSIC);

		//��ʾ����ͼƬ
		m_mainView->animalBgManager.startPlay();
	}
	else if (index == 17 || index == 18)
	{		
		//��ȸ
		//���ñ�����ʾ
		m_mainView->rateManager.setBeiLv(rate[6]);
		m_mainView->animalManager.startAnimal(1, ANIMAL_PEACOCK, PATH_PEACOCK);
		_PLAY_SOUND_SHARK(PEACOCK_MUSIC);

		//��ʾ����ͼƬ
		m_mainView->animalBgManager.startPlay();
	}
	else if (index == 20 || index == 21 || index == 22)
	{
		//��ӥ
		//���ñ�����ʾ
		m_mainView->rateManager.setBeiLv(rate[7]);
		m_mainView->animalManager.startAnimal(1, ANIMAL_HAWK, PATH_HAWK);
		_PLAY_SOUND_SHARK(EAGLE_MUSIC);

		//��ʾ����ͼƬ
		m_mainView->animalBgManager.startPlay();
	}
	else if (index == 24 || index == 25 || index == 26)
	{		
		//ʨ��
		//���ñ�����ʾ
		m_mainView->rateManager.setBeiLv(rate[8]);
		m_mainView->animalManager.startAnimal(1, ANIMAL_LION, PATH_LION);
		_PLAY_SOUND_SHARK(LION_MUSIC);

		//��ʾ����ͼƬ
		m_mainView->animalBgManager.startPlay();
	}
	else
	{
		if (!colorsharFlag)
		{
			//���迪��
			if (index == 2 || index == 9 || index == 16)
			{
				//������
				//���ñ�����ʾ
				m_mainView->rateManager.setBeiLv(rate[1]);
				m_mainView->animalManager.startAnimal(1, ANIMAL_GOLD, PATH_GOLD);
				_PLAY_SOUND_SHARK(GOLDSHARK_MUSIC);
			}
			//�����㿪��
			else if (index == 13 || index == 19 || index == 27)
			{
				//����
				//���ñ�����ʾ
				m_mainView->rateManager.setBeiLv(rate[2]);
				m_mainView->animalManager.startAnimal(1, ANIMAL_SILVER, PATH_SILVER);
				_PLAY_SOUND_SHARK(SILVERSHARK_MUSIC);
			}
			else if (index == 5)
			{
				m_mainView->animalManager.startAnimal(1, ANIMAL_SILVER, PATH_SILVER);
				m_mainView->rateManager.setBeiLv(rate[2]);
				_PLAY_SOUND_SHARK(SHARKTIME_MUSIC);
			
			}
			else if (index = 23)
			{
				m_mainView->animalManager.startAnimal(1, ANIMAL_GOLD, PATH_GOLD);
				m_mainView->rateManager.setBeiLv(rate[1]);
				_PLAY_SOUND_SHARK(SHARKTIME_MUSIC);
			}
		}
		else if (colorsharFlag)
		{
			if (index == 9 || index == 23)
			{
				//1Ԫ��  2��Ʊ  3�ʽ�
				m_mainView->colorManager.judgeResult(3); 				
				hideRate();
			}
			else
			{
				if (GameInfo::GetInstance()->getopenRelt().aType == 1)
				{
					m_mainView->colorManager.judgeResult(1);

					//1 Ԫ��  2��Ʊ
					int rate = countNum(1);
					m_mainView->rateManager.setBeiLv(rate);
				}
				else if (GameInfo::GetInstance()->getopenRelt().aType == 2)
				{
					if (index == 2 || index == 16)
					{
						m_mainView->colorManager.judgeResult(2);
						//1 Ԫ��  2��Ʊ
						int rate = countNum(2);
						m_mainView->rateManager.setBeiLv(rate);
					}
					else
					{
						m_mainView->colorManager.judgeResult(1);
						//1 Ԫ��  2��Ʊ
						int rate = countNum(1);
						m_mainView->rateManager.setBeiLv(rate);
					}

				}
			}
			
		}
	}
	
}

//�����õ�Ԫ�����ߴ�Ʊ�ĸ���
int  SharkGameScene::countNum(int index)
{
	auto gameInfo = GameInfo::GetInstance();
	int achiveNum = 0;
	int score = gameInfo->getownInfo().ownTotal; //��ȡѺע�����з���

	//100 ��Ԫ���Ļ��㵥λ��1000�Ǵ�Ʊ�Ļ��㵥λ
	if (index == 1)
	{
		int num = score / 100;
		if (num == 0)
		{
			achiveNum = 1;
		}
		else if (num != 0)
		{
			achiveNum = num;
		}
	}
	else if (index == 2)
	{
		int num = score / 1000;
		if (num == 0)
		{
			achiveNum = 1;
		}
		else if (num != 0)
		{
			achiveNum = num;
		}
	}

	return achiveNum;
}

//ֹͣս������Ķ�����ʾ
void SharkGameScene::stopResult(int index,bool colorsharFlag)
{	
	if (index == 0 || index == 1)
	{
		m_mainView->animalManager.stopAnimal(ANIMAL_PANDA);
		//���ر���ͼƬ
		m_mainView->animalBgManager.stopPlay();
	}
	else if (index == 3 || index == 4)
	{
		m_mainView->animalManager.stopAnimal(ANIMAL_MONKEY);
		//���ر���ͼƬ
		m_mainView->animalBgManager.stopPlay();
	}
	else if (index == 6 || index == 7 || index == 8)
	{
		m_mainView->animalManager.stopAnimal(ANIMAL_RABBIT);
		//���ر���ͼƬ
		m_mainView->animalBgManager.stopPlay();
	}
	else if (index == 10 || index == 11 || index == 12)
	{
		m_mainView->animalManager.stopAnimal(ANIMAL_SWALLOW);
		//���ر���ͼƬ
		m_mainView->animalBgManager.stopPlay();
	}
	else if (index == 14 || index == 15)
	{
		m_mainView->animalManager.stopAnimal(ANIMAL_DOVE);
		//���ر���ͼƬ
		m_mainView->animalBgManager.stopPlay();
	}
	else if (index == 17 || index == 18)
	{
		m_mainView->animalManager.stopAnimal(ANIMAL_PEACOCK);
		//���ر���ͼƬ
		m_mainView->animalBgManager.stopPlay();
	}
	else if (index == 20 || index == 21 || index == 22)
	{
		m_mainView->animalManager.stopAnimal(ANIMAL_HAWK);
		//���ر���ͼƬ
		m_mainView->animalBgManager.stopPlay();
	}
	else if (index == 24 || index == 25 || index == 26)
	{
		m_mainView->animalManager.stopAnimal(ANIMAL_LION);
		//���ر���ͼƬ
		m_mainView->animalBgManager.stopPlay();
	}
	

	else
	{
		if (!colorsharFlag)
		{
			//���迪��
			if (index == 2 || index == 9 || index == 16)
			{
				//������
				m_mainView->animalManager.stopAnimal(ANIMAL_GOLD);
			}
			//�����㿪��
			else if (index == 13 || index == 19 || index == 27)
			{
				//����
				m_mainView->animalManager.stopAnimal(ANIMAL_SILVER);
				 
			}
			else if (index == 5 || index == 23)
			{
				if (index == 5)
				{ 
					m_mainView->animalManager.stopAnimal(ANIMAL_SILVER);
				}
				else if (index == 23)
				{
					m_mainView->animalManager.stopAnimal(ANIMAL_GOLD);
				}
			}

		}
		else if (colorsharFlag)
		{
			m_mainView->colorManager.stopRotate();
		}
	}
}


//��ʾ��ȡ�ı���
void SharkGameScene::showRate()
{	
	m_mainView->rateManager.startPlay();
}

//���ر�����ʾ�Ľ���
void SharkGameScene::hideRate()
{
	m_mainView->rateManager.stopPlay();
}

void SharkGameScene::checkNetupdate(float t)
{

	if (!m_bIslive)
	{
		m_mainView->menuManager.goBack(-1);
		SharkNotice::openNotice(GETSTRING(1406), [](Ref*)
		{
			Director::getInstance()->replaceScene(CCTransitionFade::create(0.5f, HallGameScene::createScene()));
		});
	}
}

void SharkGameScene::CheckNet(float t)
{
	m_bIslive = false;
	unschedule(schedule_selector(SharkGameScene::checkNetupdate));
	scheduleOnce(schedule_selector(SharkGameScene::checkNetupdate), t + _CHECK_TIME);
}



void SharkGameScene::updateSendHreat(float t)
{
	SharkNetWorkKernel::getInstance()->SendHeartPak();
	//����Ƿ��л�Ӧ
	CheckNet(_ZERO);
}

void SharkGameScene::OnHreatPak()
{
	//�յ������� �����ߵ�
	m_bIslive = true;
}

