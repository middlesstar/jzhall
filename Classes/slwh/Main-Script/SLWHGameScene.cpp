#include "SLWHGameScene.h"
#include "slwh/ResourcePath.h"
#include "UI/TitleLabel.h"
#include "slwh/OneGameInfo.h"
#include "slwh/Network/NetworkKernel.h"
#include "slwh/Module/SLWH_CD.h"
#include "HallGameScene.h"
#include "slwh/Module/SLWHNotice.h"
#include "LanguageConfig.h"
#include "Window/chat/LogicChat.h"
#include "df/ServerListData.h"

#include "slwh/Hall-Script/getHouseIndex.h"
#include "slwh/Hall-Script/UILayerHall.h"


#define      game_comeToBack           "comeToBackground"
#define      game_comefromBack         "comeFromBackground"



Model3DLayer* SLWHGameScene::m_model3DLayer = nullptr;
UILayerMain*  SLWHGameScene::m_uiLayer = nullptr;
SLWHGameScene* SLWHGameScene::This = nullptr;

//��ʱ����Ƿ���������Ӧ
#define _HEART_TIME		10		//������ʱ��
#define _CHECK_TIME		3		//����������ӳ�ʱ��

SLWHGameScene::SLWHGameScene()
{
	This = this;
	chat == nullptr;
	cj_root = nullptr;
	xycj_root = nullptr;
	timeDelay = 0;
	//outTime = 0;

	//ע������̨��ǰ�˵Ĵ����¼�
	HALL_NOTIFICATION_REG(game_comeToBack, SLWHGameScene::comeToBack, nullptr);
	HALL_NOTIFICATION_REG(game_comefromBack, SLWHGameScene::comeFromBack, nullptr);
}

SLWHGameScene::~SLWHGameScene()
{
	This = nullptr;
	m_model3DLayer = nullptr;
	m_uiLayer = nullptr;
	chat = nullptr;
	cj_root = nullptr;
	xycj_root = nullptr;
	timeDelay = 0;
	//outTime = 0;

	HALL_NOTIFICATION_UNREG("comeToBackground");
	HALL_NOTIFICATION_UNREG("comeFromBackground");
}

bool SLWHGameScene::init()
{
	if (!Scene::init())
	{
		return false;
	}

	return true;
}


void SLWHGameScene::comeFromBack(Ref* ref) 
{
	//time_t currentTime;
	//time(&currentTime);
	//time_t disTime = currentTime - outTime;
	//log("------------disTime = %d-------", disTime);

	//if ((disTime / 60) > 5)
	//{
	//	Director::getInstance()->replaceScene(UILayerHall::createScene());
	//}
	//else
	//{
		loadroomData();
		auto gameInfo = OneGameInfo::GetInstance();
		HallGameServerItem* serverinfo = HallServerListData::shared()->getGameServerInfo(KIND_ID_SLWH, GetHouseIndex::getInstance()->getIndex());

		NetworkKernel::getInstance()->setUrl(serverinfo->m_GameServer.szServerAddr, serverinfo->m_GameServer.wServerPort);
		//���ӷ�����
		NetworkKernel::getInstance()->start();
		NetworkKernel::getInstance()->SendLogonPacket();

		timeDelay = 0;
		schedule(schedule_selector(SLWHGameScene::goMainScene), 0.1);
	//}
	
	
}
void SLWHGameScene::comeToBack(Ref* ref)
{
	//time(&outTime);
	//���Ѻ��
	if (OneGameInfo::GetInstance()->getServerState() == _Open)
	{
		if (m_nowIndex != 0)
		{
			unschedule(schedule_selector(SLWHGameScene::Bridge));
		}

	}
	this->removeAllChildren();

	m_model3DLayer = nullptr;
	m_uiLayer = nullptr;
	chat = nullptr;
	cj_root = nullptr;
	xycj_root = nullptr;
	
	unschedule(schedule_selector(SLWHGameScene::waitGameUpdate));
	unschedule(schedule_selector(SLWHGameScene::updateSendHreat));
	netDisconnect();


}
void SLWHGameScene::startTimer(float time)
{
	schedule(schedule_selector(SLWHGameScene::waitGameUpdate), 0.1f);
}

bool SLWHGameScene::AddGold(int g)
{
	auto info = Player::getInstance();
	if (g < _ZERO && info->getLogonData().lUserScore + g < _ZERO)
		return false;

	info->addCoin(g, false);

	//����UI
	m_uiLayer->m_uiUserInfo.update();
	return true;
}

bool SLWHGameScene::AddYuanbao(int g)					//����Ԫ��
{
	auto info = Player::getInstance();
	info->addGold(g);
	//����UI
	m_uiLayer->m_uiUserInfo.update();
	return true;
}

bool SLWHGameScene::AddTicket(int g)					//���Ӵ�Ʊ
{
	auto info = Player::getInstance();
	info->addTicket(g);
	return true;
}

void SLWHGameScene::WaitGames(int lefttime)
{
	if (lefttime < _ZERO) lefttime = _ZERO;


	log("---------dais:WaitGames-------------");
	_PLAY_MUSCI_BACK(JET_WAIT, true);
	log("%s", a_u8("-------------------��ȴ���һ��-----------------------"));


	auto gameinfo = OneGameInfo::GetInstance();

	m_uiLayer->m_resRecord.setVisible(false);

	m_uiLayer->m_jetandresManager.setVisible(false);
	//m_uiLayer->m_timeCount.StartPlay(_ZERO);
	//3D����ΪѺע�ȴ�����
	m_uiLayer->SetEnble(false);
	//ʱ��Ӧ���������������ã����ڽ�����Դ�����Ż����������ؽ���ᴦ�������
	m_uiLayer->m_waitNextJet.StartPlay(lefttime);
	//CheckLive(lefttime);

	log("---------dais:WaitGames   over-------------");
	//TitleLabel::SetTitle(TitleLabel::Normal3);
}

void SLWHGameScene::StartGames(int leftTime)
{
	if (leftTime < 0) leftTime = 0;

	_PLAY_MUSCI_BACK(JET_WAIT, true);
	//���������Ϸ������

	auto gameInfo = OneGameInfo::GetInstance();

	//���òʽ�����
	m_uiLayer->m_jcNum.SetNum(gameInfo->getroomInfo()->lCj);
	//����ѹ�ֽ���ʽ�
	m_uiLayer->m_myJetManager.setCaijin(gameInfo->getroomInfo()->lCj);
	//����̤����ɫ 
	m_model3DLayer->m_DownTo.ChangeColor((int*)gameInfo->getroomInfo()->dwColorIndex);
	//����ָ������ɫ����
	m_model3DLayer->m_sphere.SetColorIndex((int*)gameInfo->getroomInfo()->dwColorIndex);
	//���ñ���
	m_uiLayer->m_myJetManager.SetRate(gameInfo->getRate());

	m_uiLayer->m_resRecord.setVisible(true);
	m_uiLayer->m_myJetManager.OpenRecord();
	m_uiLayer->m_jetandresManager.setVisible(true);
	//���￪ʼ����
	m_model3DLayer->m_animal.OpenAniNorAc();

	log("%s", a_u8("-----------------------��ʼѺע--------------------"));

	m_uiLayer->SetEnble(true);


	//���Ѻע��Ϣ
	m_uiLayer->m_myJetManager.ClearJetton();
	m_uiLayer->m_userListManager.setTotelNum(_ZERO);

	auto roominfo = gameInfo->getroomInfo();
	for (std::vector<slwhPlayerInfo>::iterator itor = roominfo->vec.begin(); itor != roominfo->vec.end(); itor++)
	{
		itor->jet = _ZERO;
	}

	//�Ƿ����Զ�
	if (m_uiLayer->m_myJetManager.GetAI())
	{
		if (!AddGold(-m_uiLayer->m_myJetManager.GetLastJetAll()))
		{//Ǯ����

			m_uiLayer->m_myJetManager.SetAI(false);
		}
		else
		{
			m_uiLayer->m_myJetManager.ContinuJet();
		}
	}


	//�ӳ�3���ѹ�ֽ���
	if (leftTime > 3)
	{
		scheduleOnce(schedule_selector(SLWHGameScene::delaypopJet), 3.f);
	}

	//��ʼ����ʱ
	m_uiLayer->m_jetandresManager.StartPlay(leftTime);
	//CheckLive(leftTime);

	// ��ʾѹ�ֽ��棬���ؽ������
	m_uiLayer->m_myJetManager.StartPlay();
	m_uiLayer->m_resManager.StopPlay();
	//ˢ������б�
	for (std::vector<slwhPlayerInfo>::iterator itor = gameInfo->getroomInfo()->vec.begin(); itor != gameInfo->getroomInfo()->vec.end(); itor++)
	{
		(*itor).winscore = _ZERO;
	}
	m_uiLayer->m_userListManager.InitUserList();

	TitleLabel::SetTitle(TitleLabel::Normal1);

}

void SLWHGameScene::Bridge(float t)
{
	scheduleOnce(schedule_selector(SLWHGameScene::GameRunning), 0.01f);
}

void SLWHGameScene::onEnter()
{
	//t = clock();
	Scene::onEnter();
}

void SLWHGameScene::onExit()
{
	Scene::onExit();
	netDisconnect();
	SimpleAudioEngine::getInstance()->stopBackgroundMusic(false);
	SimpleAudioEngine::getInstance()->stopAllEffects();

}

//�رս���ʱ�ĶϿ����ӵĴ���
void SLWHGameScene::netDisconnect()
{
	//�������ѹ�ֽ׶Σ�������ѹ��
	auto gameinfo = OneGameInfo::GetInstance();

	//�û���Ϣ
	SLWHUserInfo info = OneGameInfo::GetInstance()->getUserInfo();
	//��������
	NetworkKernel::getInstance()->SendStandUpPacket(info.wTableID, info.wChairID, TRUE);
	//�Ͽ�����
	NetworkKernel::getInstance()->relese();

	//�����Ϣ
	gameinfo->relese();
}

void SLWHGameScene::loadroomData()
{
	slwhRoomInfo* info = OneGameInfo::GetInstance()->getroomInfo();
	info->wType = GetHouseIndex::getInstance()->getIndex();


	//�ͻ���д������ѹ����Ϣ
	switch (info->wType)
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
}

void SLWHGameScene::initSpe()
{

	//D3Y��ʼ��
	m_speD3Y.setPosition3D(Vec3(_ZERO, 65.f, _ZERO));
	auto d3yspr = Sprite3D::create();
	CC_ASSERT(d3yspr != nullptr);
	d3yspr->setScale(30.f);
	d3yspr->setCameraMask((int)CameraFlag::USER1);
	m_speD3Y.AttachSprite3D(d3yspr);
	m_speD3Y.AttachAnimalAndStage(&m_model3DLayer->m_animal, &m_model3DLayer->m_stage);
	addChild(&m_speD3Y);

	//D4X��ʼ��
	m_model3DLayer->m_stage.addChild(&m_speD4X);

	//���⿪��CJ����
	cj_root = CSLoader::createNode(COCOS_STUDIO_CJ);
	CC_ASSERT(cj_root != nullptr);
	addChild(cj_root);
	m_speCJ.AttachNode(cj_root->getChildByName("Special_CJ"));
	addChild(&m_speCJ);

	//���⿪�� XYCJ����
	xycj_root = CSLoader::createNode(COCOS_STUDIO_XYCJ);
	CC_ASSERT(xycj_root != nullptr);
	addChild(xycj_root);
	m_speXYCJ.AttachNode(xycj_root->getChildByName("XYCJ_Back"));
	addChild(&m_speXYCJ);

	//DMG
	//auto dmgspr = Sprite3D::create(Path_DMG);
	//dmgspr->setScale(20.f);
	//dmgspr->setRotation3D(Vec3(180, _ZERO, _ZERO));
	//dmgspr->setPosition3D(Vec3(_ZERO, -310, _ZERO));
	//dmgspr->setCameraMask((int)CameraFlag::USER1);
	//m_speDmg.AttachSprite3D(dmgspr);
	addChild(&m_speDmg);
}

void SLWHGameScene::onEnterTransitionDidFinish()
{
	m_model3DLayer = Model3DLayer::create();
	CC_ASSERT(NULL != m_model3DLayer);
	addChild(m_model3DLayer);

	m_uiLayer = UILayerMain::create();
	CC_ASSERT(NULL != m_uiLayer);
	addChild(m_uiLayer);

	//��ʼ�����⿪��
	initSpe();


	//��������
	chat = Node::create();
	this->addChild(chat, 10);
	HallGameScene::openChatSprite(chat);

	////��ʼ��ѹ������λ��
	//m_uiLayer->m_jetandresManager.initposition();
	//��ʼ��һ��ѹ�ְ�ť
	m_uiLayer->m_myJetManager.SetJetNum(OneOfJetton::SetAddJet(1));

	auto gameInfo = OneGameInfo::GetInstance();
	//��ʼ����ǰ����
	m_nowIndex = _ZERO;
	//���òʽ�����
	m_uiLayer->m_jcNum.SetNum(gameInfo->getroomInfo()->lCj);
	//����ѹ�ֽ���ʽ�
	m_uiLayer->m_myJetManager.setCaijin(gameInfo->getroomInfo()->lCj);
	//���ü�¼������
	m_uiLayer->m_resRecord.InitResList(gameInfo->getResRecord());
	m_uiLayer->m_myJetManager.InitResList(gameInfo->getResRecord());
	//������ʱ�� �ȴ������֪ͨ��ʼ�µ�һ����Ϸ

	log("-------------LeftTime:%d----------------------", (int)gameInfo->getLeftTime());
	m_nowState = gameInfo->getServerState();
	if (m_nowState != enumServerState::_Jet)		// ����Ѻע�׶� 1  = Ѻע�׶� 
	{
		WaitGames((int)gameInfo->getLeftTime() - 5);
	}
	else
	{
		//��ʼѺע
		m_uiLayer->m_waitNextJet.destory();
		//ʱ��Ӧ���������������ã����ڽ�����Դ�����Ż����������ؽ���ᴦ�������
		StartGames((int)gameInfo->getLeftTime() - 5);
	}
	//��������б�
	m_uiLayer->m_userListManager.InitUserList();

	m_uiLayer->m_userListManager.updateInfo();

	//���������״̬�仯
	schedule(schedule_selector(SLWHGameScene::waitGameUpdate), 0.1f);

	//���������� 	����һ��������
	updateSendHreat(0.f);
	schedule(schedule_selector(SLWHGameScene::updateSendHreat), _HEART_TIME);
}

void SLWHGameScene::waitGameUpdate(float t)
{
	if (m_uiLayer->m_jetandresManager.GetleftTime() < _ZERO)
	{
		m_uiLayer->SetEnble(false);
	}
	enumServerState serverState = OneGameInfo::GetInstance()->getServerState();
	if (m_nowState == serverState) return;
	m_nowState = serverState; 
	//����Ϣ������ �����Ǻõ�
	m_bIslive = true;
	switch (m_nowState)
	{
	case enumServerState::_Jet:
		//��ע�׶�
		StartGames(33);
		m_uiLayer->m_userListManager.updateInfo();
		break;
	case enumServerState::_Open:
		//�������
		OpenGames();
		//m_uiLayer->m_userListManager.updateInfo();
		break;
	default:
		break;
	}
}

void SLWHGameScene::goMainScene(float t)
{
	timeDelay++;
	if (!OneGameInfo::GetInstance()->getStartGameFinish()) return;

	unschedule(schedule_selector(SLWHGameScene::goMainScene));

	m_model3DLayer = Model3DLayer::create();
	CC_ASSERT(NULL != m_model3DLayer);
	addChild(m_model3DLayer);

	m_uiLayer = UILayerMain::create();
	CC_ASSERT(NULL != m_uiLayer);
	addChild(m_uiLayer);

	//��ʼ�����⿪��
	initSpe();
	////��ʼ��ѹ������λ��
	//m_uiLayer->m_jetandresManager.initposition();
	//��ʼ��һ��ѹ�ְ�ť
	m_uiLayer->m_myJetManager.SetJetNum(OneOfJetton::SetAddJet(1));

	auto gameInfo = OneGameInfo::GetInstance();
	//��ʼ����ǰ����
	m_nowIndex = _ZERO;
	//���òʽ�����
	m_uiLayer->m_jcNum.SetNum(gameInfo->getroomInfo()->lCj);
	//����ѹ�ֽ���ʽ�
	m_uiLayer->m_myJetManager.setCaijin(gameInfo->getroomInfo()->lCj);
	//���ü�¼������
	m_uiLayer->m_resRecord.InitResList(gameInfo->getResRecord());
	m_uiLayer->m_myJetManager.InitResList(gameInfo->getResRecord());
	//������ʱ�� �ȴ������֪ͨ��ʼ�µ�һ����Ϸ

	log("-------------LeftTime:%d----------------------", (int)gameInfo->getLeftTime());
	m_nowState = gameInfo->getServerState();
	if (m_nowState != enumServerState::_Jet)		// ����Ѻע�׶� 1  = Ѻע�׶� 
	{
		WaitGames((int)gameInfo->getLeftTime() - timeDelay);
	}
	else
	{
		//��ʼѺע
		m_uiLayer->m_waitNextJet.destory();
		//ʱ��Ӧ���������������ã����ڽ�����Դ�����Ż����������ؽ���ᴦ�������
		StartGames((int)gameInfo->getLeftTime() - timeDelay);
	}
	//��������б�
	m_uiLayer->m_userListManager.InitUserList();

	m_uiLayer->m_userListManager.updateInfo();

	//���������״̬�仯
	schedule(schedule_selector(SLWHGameScene::waitGameUpdate), 0.1f);

	//���������� 	����һ��������
	updateSendHreat(0.f);
	schedule(schedule_selector(SLWHGameScene::updateSendHreat), _HEART_TIME);

	
}

void SLWHGameScene::delaypopJet(float t)
{
	m_uiLayer->m_jetandresManager.MyJettonOut(1);
}

void SLWHGameScene::OpenGames()
{
	scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 1);		//��ͣ��1��ȴ�ʱ����Ч����
}

void SLWHGameScene::GameRunning(float t)
{
	//�ύ��Ϸѹ�����ݵ�������
	//�õ��������

	m_nowIndex++;
	log("--------dais:spe:%d,Index:%d--------", OneGameInfo::GetInstance()->getopenRlt().res.e1, m_nowIndex);
	switch (OneGameInfo::GetInstance()->getopenRlt().res.e1)
	{
	case enumSpecialRs::Normal:
		Normal(m_nowIndex);
		break;
	case enumSpecialRs::D3Y:
		D3Y(m_nowIndex);
		break;
	case enumSpecialRs::D4X:
		D4X(m_nowIndex);
		break;
	case enumSpecialRs::_4LF:
		_4LF(m_nowIndex);
		break;
	case enumSpecialRs::XYCJ:
		XYCJ(m_nowIndex);
		break;
	case enumSpecialRs::DMG:
		DMG(m_nowIndex);
		break;
	case enumSpecialRs::CJ:
		CJ(m_nowIndex);
		break;
	default:
		log("%s", a_u8("-------------�����������------------"));
		break;
	}
}

void SLWHGameScene::Normal(int index)
{

	auto gameinfo = OneGameInfo::GetInstance();
	auto openRlt = gameinfo->getopenRlt();

	if (index == 1)
	{

		_PLAY_MUSCI_BACK(ZP_START, false);

		//����ѹ������
		m_uiLayer->m_myJetManager.SaveLastGame();
		//ѹ�ֽ��治����
		//m_uiLayer->m_myJetManager.enable(false);
		m_uiLayer->m_jetandresManager.MyJettonOut(2);
		//�˹��̳���ʱ��Ϊ2S
		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 2);

	}
	else if (index == 2)
	{

		_PLAY_MUSCI_BACK(ZP_RUN, true);

		m_model3DLayer->m_stage.StartPlay(openRlt.colorindex);
		m_model3DLayer->m_animal.StartPlay(openRlt.animalindex, openRlt.colorindex);
		m_uiLayer->m_zhxControl.StartPlay(openRlt.res.e4);


		//�˹��̳���ʱ��Ϊ14S
		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 14);
	}
	else if (index == 3)
	{

		AnimalResult res = openRlt.res;
		//������Ч
		PlayAnimalMusic(res.e1, res.e3, res.e2);

		//��������
		enumAnimalColor e = m_model3DLayer->m_sphere.OpenLight();
		m_model3DLayer->m_lightAll.OpenLight(e);

		//�˹��̳���ʱ��Ϊ1S
		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 1);
	}
	else if (index == 4)
	{
		_STOP_MUSCI_BACK;
		//����۽�
		m_model3DLayer->m_camera3D.Close2Animal();
		//��ӽ��
		m_uiLayer->m_resRecord.Add(openRlt.res);
		m_uiLayer->m_myJetManager.Add(openRlt.res);
		//���򶯻�
		m_model3DLayer->m_stage.OpenSphere(true);

		m_model3DLayer->m_animal.ResPlay(openRlt.colorindex, openRlt.res.e2);

		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 8);
	}
	else if (index == 5)
	{

		_PLAY_MUSCI_BACK(_KJJS, false);

		//������������	
		gameinfo->setbCountScore(true);
		m_uiLayer->m_jetandresManager.MyJettonOut(1);
		m_uiLayer->m_jetandresManager.setVisible(false);
		int getGold = m_uiLayer->m_resManager.StartPlay(
			openRlt.res,
			gameinfo->getRate(),
			m_uiLayer->m_myJetManager.GetRecord()
			);

		//����ѹ�ֽ���
		m_uiLayer->m_myJetManager.StopPlay();
		//������ҽ��
		AddGold(getGold);
		//�����б�
		m_uiLayer->m_userListManager.updateUserList(gameinfo->getroomInfo()->vec);
		m_uiLayer->m_userListManager.updateInfo(1);
		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 8);
	}
	else if (index == 6)
	{
		//���ؿ�������
		m_uiLayer->m_jetandresManager.MyJettonOut(2);
		//���ﻹԭλ��
		m_model3DLayer->m_animal.ResStop();
		//��ͷ��Զ
		m_model3DLayer->m_camera3D.FarAway();

		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 1);

	}
	else if (index == 7)
	{
		//�ر���
		m_model3DLayer->m_stage.OpenSphere(false);
		//�رճ������
		m_model3DLayer->m_lightAll.CloseLight();
		//�ر�������
		m_model3DLayer->m_sphere.CloseLight();
		//������ʼ��
		m_nowIndex = _ZERO;
		//��ʼ������
		gameinfo->setopenRlt(OpenRes());
		//�˹���4�루Ԥ��1�룩
		//CheckLive(4);
	}

}

void SLWHGameScene::D3Y(int index)
{
	auto gameinfo = OneGameInfo::GetInstance();
	auto openRlt = gameinfo->getopenRlt();

	if (index == 1)
	{
		_PLAY_MUSCI_BACK(_DJKS, false);

		TitleLabel::SetTitle(TitleLabel::D3Y);

		//m_speD3Y.StartPlay();

		//����ѹ������
		m_uiLayer->m_myJetManager.SaveLastGame();
		//ѹ�ֽ��治����
		m_uiLayer->m_jetandresManager.MyJettonOut(2);

		//���򶯻�
		//m_model3DLayer->m_stage.OpenSphere(true);

		//����۽�
		m_model3DLayer->m_camera3D.Close2AnimalMid();

		//�˹��̳���ʱ��Ϊ2S
		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 2);

	}
	else if (index == 2)
	{

		_PLAY_MUSCI_BACK(HI_TIME, true);

		m_model3DLayer->m_stage.StartPlay(openRlt.colorindex);
		m_model3DLayer->m_animal.StartPlay(openRlt.animalindex, openRlt.colorindex);
		m_uiLayer->m_zhxControl.StartPlay(openRlt.res.e4);

		//��������
		m_model3DLayer->m_lightAll.OpenLight(enumAnimalColor::ColorMax);

		//�˹��̳���ʱ��Ϊ14S
		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 14);
	}
	else if (index == 3)
	{

		//������Ч
		PlayAnimalMusic(openRlt.res.e1, openRlt.res.e3, openRlt.res.e2);

		//�˹��̳���ʱ��Ϊ1S
		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 1);
	}
	else if (index == 4)
	{
		_STOP_MUSCI_BACK;
		//����۽�
		m_model3DLayer->m_camera3D.Close2Animal();

		//���򶯻�
		m_model3DLayer->m_stage.OpenSphere(true);

		//��ӽ��
		m_uiLayer->m_resRecord.Add(openRlt.res);
		m_uiLayer->m_myJetManager.Add(openRlt.res);

		m_model3DLayer->m_animal.ResPlay(openRlt.colorindex, openRlt.res.e2);

		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 8);
	}
	else if (index == 5)
	{
		_PLAY_MUSCI_BACK(_KJJS, false);

		//������������	
		gameinfo->setbCountScore(true);
		m_uiLayer->m_jetandresManager.MyJettonOut(1);
		m_uiLayer->m_jetandresManager.setVisible(false);
		int getGold = m_uiLayer->m_resManager.StartPlay(
			openRlt.res,
			gameinfo->getRate(),
			m_uiLayer->m_myJetManager.GetRecord()
			);
		//����ѹ�ֽ���
		m_uiLayer->m_myJetManager.StopPlay();
		//������ҽ��
		AddGold(getGold);
		//�����б�
		m_uiLayer->m_userListManager.updateUserList(gameinfo->getroomInfo()->vec);
		m_uiLayer->m_userListManager.updateInfo(1);
		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 8);
	}
	else if (index == 6)
	{


		//���ؿ�������
		m_uiLayer->m_jetandresManager.MyJettonOut(2);
		//���ﻹԭλ��
		m_model3DLayer->m_animal.ResStop();
		//��ͷ��Զ
		m_model3DLayer->m_camera3D.FarAway();

		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 1);

		//D3Y��ʧ
		//m_speD3Y.StopPlay();

		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 1);

	}
	else if (index == 7)
	{
		//�ر���
		m_model3DLayer->m_stage.OpenSphere(false);
		//�رճ������
		m_model3DLayer->m_lightAll.CloseLight();
		//�ر�������
		m_model3DLayer->m_sphere.CloseLight();
		//��λ
		m_nowIndex = _ZERO;
		//��ʼ������
		gameinfo->setopenRlt(OpenRes());
		//�˹���4�루Ԥ��1�룩
		//CheckLive(4);
	}
}

void SLWHGameScene::D4X(int index)
{
	auto gameinfo = OneGameInfo::GetInstance();
	auto openRlt = gameinfo->getopenRlt();

	if (index == 1)
	{
		_PLAY_MUSCI_BACK(_DJKS, false);

		TitleLabel::SetTitle(TitleLabel::D4X);

		//����ѹ������
		m_uiLayer->m_myJetManager.SaveLastGame();
		//����ѹ�ֽ���
		m_uiLayer->m_jetandresManager.MyJettonOut(2);
		//����۽�
		m_model3DLayer->m_camera3D.Close2AnimalMid();
		//�˹��̳���ʱ��Ϊ2S
		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 2);
	}
	else if (index == 2)
	{

		_PLAY_MUSCI_BACK(HI_TIME, true);


		m_model3DLayer->m_stage.StartPlay(openRlt.colorindex);
		m_model3DLayer->m_animal.StartPlay(openRlt.animalindex, openRlt.colorindex);
		m_uiLayer->m_zhxControl.StartPlay(openRlt.res.e4);

		//��������
		m_model3DLayer->m_lightAll.OpenLight(enumAnimalColor::ColorMax);

		//�˹��̳���ʱ��Ϊ14S
		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 14);
	}
	else if (index == 3)
	{

		AnimalResult res = openRlt.res;
		//������Ч
		PlayAnimalMusic(res.e1, res.e3, res.e2);

		//�˹��̳���ʱ��Ϊ1S
		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 1);
	}
	else if (index == 4)
	{
		//����۽�
		m_model3DLayer->m_camera3D.Close2Animal();

		//_STOP_MUSCI_BACK;
		//��ӽ��
		m_uiLayer->m_resRecord.Add(openRlt.res);
		m_uiLayer->m_myJetManager.Add(openRlt.res);

		//����
		m_model3DLayer->m_stage.OpenSphere(true);

		//D4X���⶯���߼�
		m_speD4X.StartPlay();

		//�������м��ƶ�
		//m_model3DLayer->m_animal.ResPlay(openRlt.colorindex, openRlt.res.e2);

		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 8);
	}
	else if (index == 5)
	{

		_PLAY_MUSCI_BACK(_KJJS, false);

		//������������	
		gameinfo->setbCountScore(true);
		m_uiLayer->m_jetandresManager.MyJettonOut(1);
		m_uiLayer->m_jetandresManager.setVisible(false);
		int getGold = m_uiLayer->m_resManager.StartPlay(
			openRlt.res,
			gameinfo->getRate(),
			m_uiLayer->m_myJetManager.GetRecord()
			);
		//����ѹ�ֽ���
		m_uiLayer->m_myJetManager.StopPlay();
		//������ҽ��
		AddGold(getGold);
		//�����б�
		m_uiLayer->m_userListManager.updateUserList(gameinfo->getroomInfo()->vec);
		m_uiLayer->m_userListManager.updateInfo(1);
		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 8);
	}
	else if (index == 6)
	{

		//���ؿ�������
		m_uiLayer->m_jetandresManager.MyJettonOut(2);
		//���ﻹԭλ��
		//m_model3DLayer->m_animal.ResStop();
		//��ͷ��Զ
		m_model3DLayer->m_camera3D.FarAway();
		//D3Y��ʧ
		//m_speD3Y.StopPlay();
		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 1);

	}
	else if (index == 7)
	{
		//�ر���
		m_model3DLayer->m_stage.OpenSphere(false);

		m_speD4X.StopPlay();
		//�رճ������
		m_model3DLayer->m_lightAll.CloseLight();

		//�ر�������
		m_model3DLayer->m_sphere.CloseLight();
		//��ԭ
		m_nowIndex = _ZERO;
		//��ʼ������
		gameinfo->setopenRlt(OpenRes());
		//�˹���4��
		//CheckLive(4);
	}
}

void SLWHGameScene::_4LF(int index)
{
	//�õ�4�������
	auto gameInfo = OneGameInfo::GetInstance();
	Open4LF open4LF = gameInfo->getopen4LFRlt();

	if (index == 1)
	{
		//�󽱿�ʼ��Ч
		_PLAY_MUSCI_BACK(_DJKS, false);
		//4��������+��Ч
		TitleLabel::SetTitle(TitleLabel::_4lian);
		//����ѹ������
		m_uiLayer->m_myJetManager.SaveLastGame();
		//ѹ�ֽ�������
		m_uiLayer->m_jetandresManager.MyJettonOut(2);
		//�˹��̳���ʱ��Ϊ2S
		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 2);
	}
	else if (index == 2)
	{

		//ׯ���п�ʼת
		m_uiLayer->m_zhxControl.StartPlay(gameInfo->getopenRlt().res.e4);
		//����ȫ����
		m_model3DLayer->m_lightAll.OpenLight(enumAnimalColor::ColorMax);
		//�˹���Ϊ2�׶εĹ���
		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 0.01);
	}
	else if (index == 3)
	{
		//�Ƚ��˵ı�����������
		_PLAY_MUSCI_BACK(HI_TIME, true);
		//����ת�̿�ʼת
		m_model3DLayer->m_stage.StartPlay(open4LF.colorindex[4 - open4LF.leftIndex]);
		//ָ�뿪ʼת
		m_model3DLayer->m_animal.StartPlay(open4LF.animalindex[4 - open4LF.leftIndex], open4LF.colorindex[4 - open4LF.leftIndex]);
		//�˹��̳���ʱ��Ϊ14S
		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 14);
	}
	else if (index == 4)
	{
		//�õ�4�������
		AnimalResult res = open4LF.res[4 - open4LF.leftIndex];
		//������Ч
		PlayAnimalMusic(enumSpecialRs::_4LF, open4LF.res[4 - open4LF.leftIndex].e3, open4LF.res[4 - open4LF.leftIndex].e2);
		//�˹��̳���ʱ��Ϊ1S
		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 1);
	}
	else if (index == 5)
	{
		//ֹͣ������Ч
		_STOP_MUSCI_BACK;
		//���򶯻�
		m_model3DLayer->m_stage.OpenSphere(true);
		//����۽�
		m_model3DLayer->m_camera3D.Close2Animal();
		//��ӽ����¼
		m_uiLayer->m_resRecord.Add(open4LF.res[4 - open4LF.leftIndex]);
		m_uiLayer->m_myJetManager.Add(open4LF.res[4 - open4LF.leftIndex]);
		//�������м�չʾ
		m_model3DLayer->m_animal.ResPlay(open4LF.colorindex[4 - open4LF.leftIndex], open4LF.res[4 - open4LF.leftIndex].e2);
		//�˹��̳���ʱ��Ϊ
		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 8);

	}
	else if (index == 6)
	{
		//���һ��ʱ�������˽׶�
		if (--open4LF.leftIndex > _ZERO)
		{
			//���ﻹԭλ��
			m_model3DLayer->m_animal.ResStop();
			//��ͷ��Զ
			m_model3DLayer->m_camera3D.FarAway();
			//�˽׶�1��
			scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 1);
		}
		else
		{
			scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 0.01);
		}

	}
	else if (index == 7)
	{
		//4�����ظ�3~7�׶� 4��

		//���һ�������˽׶�
		if (--open4LF.leftIndex > _ZERO)
		{
			//�˽׶ι��˻�++
			m_nowIndex = 2;
			//�ر���
			m_model3DLayer->m_stage.OpenSphere(false);
			//�˽׶�3��
			scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 3);
		}
		else
		{
			scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 0.01);
		}
		//ˢ��4lf���
		gameInfo->setopen4LFRlt(open4LF);
	}
	else if (index == 8)
	{
		_PLAY_MUSCI_BACK(_KJJS, false);
		gameInfo->setbCountScore(true);
		//������������	��������
		m_uiLayer->m_jetandresManager.MyJettonOut(1);
		//�����л���ť, ��������ҽ������ز���
		m_uiLayer->m_jetandresManager.setVisible(false);
		//����ѹ�ֽ���
		m_uiLayer->m_myJetManager.StopPlay();
		//����÷�
		int getGold = m_uiLayer->m_resManager.StartPlay(
			gameInfo->getopenRlt().res,
			gameInfo->getRate(),
			m_uiLayer->m_myJetManager.GetRecord(),
			open4LF.res
			);
		//������ҽ��
		AddGold(getGold);
		//�����б�
		m_uiLayer->m_userListManager.updateUserList(OneGameInfo::GetInstance()->getroomInfo()->vec);
		//�����Լ�
		m_uiLayer->m_userListManager.updateInfo(1);
		//�˹���8��
		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 8);
	}
	else if (index == 9)
	{

		//���ؿ�������
		m_uiLayer->m_jetandresManager.MyJettonOut(2);
		//���ﻹԭλ��
		m_model3DLayer->m_animal.ResStop();
		//��ͷ��Զ
		m_model3DLayer->m_camera3D.FarAway();
		//�˹���1��
		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 1);

	}
	else if (index == 10)
	{
		//�ر���
		m_model3DLayer->m_stage.OpenSphere(false);
		//�رճ������
		m_model3DLayer->m_lightAll.CloseLight();
		//�ر�������
		m_model3DLayer->m_sphere.CloseLight();
		//��λ
		m_nowIndex = _ZERO;
		//��ʼ������
		gameInfo->setopenRlt(OpenRes());
		//��ʼ�����⿪��
		gameInfo->setopen4LFRlt(Open4LF());
		//�˹���4��
		//CheckLive(4);
	}
}

void SLWHGameScene::CJ(int index)
{
	auto gameinfo = OneGameInfo::GetInstance();
	auto openRlt = gameinfo->getopenRlt();
	OpenCJ cj = gameinfo->getopenCJRlt();
	if (index == 1)
	{

		_PLAY_MUSCI_BACK(_DJKS, false);

		TitleLabel::SetTitle(TitleLabel::CJ);
		//����ѹ������
		m_uiLayer->m_myJetManager.SaveLastGame();
		m_uiLayer->m_jetandresManager.MyJettonOut(2);
		//ѹ�ֽ��治����
		//m_uiLayer->m_myJetManager.enable(false);
		//�˹��̳���ʱ��Ϊ2S
		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 2);

	}
	else if (index == 2)
	{

		_PLAY_MUSCI_BACK(HI_TIME, true);

		m_model3DLayer->m_stage.StartPlay(openRlt.colorindex);
		m_model3DLayer->m_animal.StartPlay(openRlt.animalindex, openRlt.colorindex);
		m_uiLayer->m_zhxControl.StartPlay(openRlt.res.e4);

		//��������
		m_model3DLayer->m_lightAll.OpenLight(enumAnimalColor::ColorMax);

		//�˹��̳���ʱ��Ϊ14S
		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 14);
	}
	else if (index == 3)
	{

		AnimalResult res = openRlt.res;
		//������Ч
		PlayAnimalMusic(res.e1, res.e3, res.e2);

		//�˹��̳���ʱ��Ϊ1S
		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 1);
	}
	else if (index == 4)
	{
		_STOP_MUSCI_BACK;
		//����۽�
		m_model3DLayer->m_camera3D.Close2Animal();
		//��ӽ��
		m_uiLayer->m_resRecord.Add(gameinfo->getopenRlt().res);

		m_uiLayer->m_myJetManager.Add(gameinfo->getopenRlt().res);

		//���򶯻�
		m_model3DLayer->m_stage.OpenSphere(true);

		m_model3DLayer->m_animal.ResPlay(gameinfo->getopenRlt().colorindex, gameinfo->getopenRlt().res.e2);

		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 8);
	}
	else if (index == 5)
	{
		std::string namestr[3] = { cj.name[0], cj.name[1], cj.name[2] };
		m_speCJ.StartPlay(cj.num, namestr, cj.score);
		SLWH_CD::StartPlay();
		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 5);
	}
	else if (index == 6)
	{

		_PLAY_MUSCI_BACK(_KJJS, false);
		m_speCJ.StopPlay();
		SLWH_CD::StopPlay();

		int cj_get_num = 0;
		for (size_t i = 0; i < 3; i++)
		{
			if (cj.userid[i] == Player::getInstance()->getLogonData().dwUserID)
			{
				cj_get_num = cj.score[i];
				break;
			}
		}


		//������������	
		gameinfo->setbCountScore(true);
		m_uiLayer->m_jetandresManager.MyJettonOut(1);
		m_uiLayer->m_jetandresManager.setVisible(false);
		int getGold = m_uiLayer->m_resManager.StartPlay(
			gameinfo->getopenRlt().res,
			gameinfo->getRate(),
			m_uiLayer->m_myJetManager.GetRecord(),
			cj_get_num);
		//����ѹ�ֽ���
		m_uiLayer->m_myJetManager.StopPlay();
		//������ҽ��
		AddGold(getGold);
		//�����б�
		m_uiLayer->m_userListManager.updateUserList(gameinfo->getroomInfo()->vec);
		m_uiLayer->m_userListManager.updateInfo(1);
		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 8);
	}
	else if (index == 7)
	{
		SLWH_CD::StopPlay();
		//���ؿ�������
		//m_uiLayer->m_resManager.StopPlay();
		//���ﻹԭλ��
		m_model3DLayer->m_animal.ResStop();
		//��ͷ��Զ
		m_model3DLayer->m_camera3D.FarAway();

		m_uiLayer->m_jetandresManager.MyJettonOut(2);
		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 1);

	}
	else if (index == 8)
	{
		//�ر���
		m_model3DLayer->m_stage.OpenSphere(false);
		//�رճ������
		m_model3DLayer->m_lightAll.CloseLight();
		//�ر�������
		m_model3DLayer->m_sphere.CloseLight();
		//��ԭ
		m_nowIndex = _ZERO;
		//��ʼ������
		gameinfo->setopenRlt(OpenRes());
		//��ʼ�����⿪��
		gameinfo->setopenCJRlt(OpenCJ());
		//�˹���4��
		//CheckLive(4);
	}
}

void SLWHGameScene::XYCJ(int index)
{
	auto gameinfo = OneGameInfo::GetInstance();
	auto openRlt = gameinfo->getopenRlt();
	auto xycjRlt = gameinfo->getopenXYCJRlt();
	if (index == 1)
	{
		_PLAY_MUSCI_BACK(_DJKS, false);

		TitleLabel::SetTitle(TitleLabel::XYCJ);
		//����ѹ������
		m_uiLayer->m_myJetManager.SaveLastGame();
		m_uiLayer->m_jetandresManager.MyJettonOut(2);
		//ѹ�ֽ��治����
		//m_uiLayer->m_myJetManager.enable(false);
		//����۽�
		//m_model3DLayer->m_camera3D.Close2AnimalMid();
		//�˹��̳���ʱ��Ϊ2S
		//����XYCJ��Ч
		m_speXYCJ.StartPlayEff();

		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 2);

	}
	else if (index == 2)
	{

		_PLAY_MUSCI_BACK(HI_TIME, true);

		m_model3DLayer->m_stage.StartPlay(openRlt.colorindex);
		m_model3DLayer->m_animal.StartPlay(openRlt.animalindex, openRlt.colorindex);
		m_uiLayer->m_zhxControl.StartPlay(openRlt.res.e4);

		//��������
		m_model3DLayer->m_lightAll.OpenLight(enumAnimalColor::ColorMax);

		//�˹��̳���ʱ��Ϊ14S
		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 14);
	}
	else if (index == 3)
	{

		//������Ч
		PlayAnimalMusic(openRlt.res.e1, openRlt.res.e3, openRlt.res.e2);

		//�˹��̳���ʱ��Ϊ1S
		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 1);
	}
	else if (index == 4)
	{
		_STOP_MUSCI_BACK;
		//���򶯻�
		m_model3DLayer->m_stage.OpenSphere(true);
		//����۽�
		m_model3DLayer->m_camera3D.Close2Animal();
		//��ӽ��
		m_uiLayer->m_resRecord.Add(openRlt.res);
		m_uiLayer->m_myJetManager.Add(openRlt.res);
		//�����ƶ�
		m_model3DLayer->m_animal.ResPlay(openRlt.colorindex, openRlt.res.e2);

		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 8);
	}
	else if (index == 5)
	{
		_PLAY_MUSCI_BACK(XYCJ_HI_TIME2, false);
		m_speXYCJ.StartPlay(xycjRlt.Atype,xycjRlt.AValue, xycjRlt.iNotes);
		m_model3DLayer->setVisible(false);
		if (OneGameInfo::GetInstance()->getopenXYCJRlt().iNotes[0] != 1)
		{
			scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 15);
		}
		else
		{
			scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 53);
		}
	}
	else if (index == 6)
	{

		_PLAY_MUSCI_BACK(_KJJS, false);

		m_speXYCJ.StopPlay();
		m_model3DLayer->setVisible(true);

		//������������	
		gameinfo->setbCountScore(true);
		m_uiLayer->m_jetandresManager.MyJettonOut(1);
		m_uiLayer->m_jetandresManager.setVisible(false);
		int getGold = m_uiLayer->m_resManager.StartPlay(
			openRlt.res,
			gameinfo->getRate(),
			m_uiLayer->m_myJetManager.GetRecord(),
			xycjRlt.Atype,
			xycjRlt.iNotes,
			m_speXYCJ.getScoreBl(),
			m_speXYCJ.getyuanbaoBl(),
			m_speXYCJ.getcpBl()
			);
		//����ѹ�ֽ���
		m_uiLayer->m_myJetManager.StopPlay();
		//������ҽ��
		AddGold(getGold);
		//������Ҵ�Ʊ
		AddYuanbao(m_uiLayer->m_resManager.getyuanbao());
		//�������Ԫ��
		AddTicket(m_uiLayer->m_resManager.getticket());
		//�����б�
		m_uiLayer->m_userListManager.updateUserList(gameinfo->getroomInfo()->vec);
		m_uiLayer->m_userListManager.updateInfo(1);
		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 8);
	}
	else if (index == 7)
	{

		//���ؿ�������
		m_uiLayer->m_jetandresManager.MyJettonOut(2);
		//���ﻹԭλ��
		m_model3DLayer->m_animal.ResStop();
		//��ͷ��Զ
		m_model3DLayer->m_camera3D.FarAway();

		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 1);

	}
	else if (index == 8)
	{
		//�ر�XYCJ ��Ч
		m_speXYCJ.StopPlayEff();
		//�ر���
		m_model3DLayer->m_stage.OpenSphere(false);
		//�رճ������
		m_model3DLayer->m_lightAll.CloseLight();
		//�ر�������
		m_model3DLayer->m_sphere.CloseLight();
		//��ԭ
		m_nowIndex = _ZERO;
		//��ʼ������
		gameinfo->setopenRlt(OpenRes());
		//��ʼ�����⿪��
		gameinfo->setopenXYCJRlt(OpenXYCJ());
		//�˹���4��
		//CheckLive(4);
	}
}

void SLWHGameScene::DMG(int index)
{
	//�õ�DMG�������
	auto gameinfo = OneGameInfo::GetInstance();
	auto openRlt = gameinfo->getopenRlt();
	auto openDMG = gameinfo->getopenDMGRlt();

	if (index == 1)
	{
		_PLAY_MUSCI_BACK(_DJKS, false);

		//����ѹ������
		m_uiLayer->m_myJetManager.SaveLastGame();
		//ѹ�ֽ��治����
		//m_uiLayer->m_myJetManager.enable(false);
		//DMG����+��Ч
		TitleLabel::SetTitle(TitleLabel::DMG);
		//ѹ�ֽ�������
		m_uiLayer->m_jetandresManager.MyJettonOut(2);
		//�˹��̳���ʱ��Ϊ2S
		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 2);

	}
	else if (index == 2)
	{

		_PLAY_MUSCI_BACK(HI_TIME, true);

		//��������
		m_model3DLayer->m_lightAll.OpenLight(enumAnimalColor::ColorMax);

		m_model3DLayer->m_stage.StartPlay(openRlt.colorindex);
		m_model3DLayer->m_animal.StartPlay(openRlt.animalindex, openRlt.colorindex);
		m_uiLayer->m_zhxControl.StartPlay(openRlt.res.e4);


		//�˹��̳���ʱ��Ϊ14S
		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 14);
	}
	else if (index == 3)
	{

		AnimalResult res = openRlt.res;
		//������Ч
		PlayAnimalMusic(res.e1, res.e3, res.e2);

		//��������
		//enumAnimalColor e = m_model3DLayer->m_sphere.OpenLight();
		//m_model3DLayer->m_lightAll.OpenLight(e);
		m_model3DLayer->m_sphere.OpenLight();

		//�˹��̳���ʱ��Ϊ1S
		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 1);
	}
	else if (index == 4)
	{
		_STOP_MUSCI_BACK;
		//����۽�
		m_model3DLayer->m_camera3D.Close2AnimalMid();
		//��ӽ��
		m_uiLayer->m_resRecord.Add(openRlt.res);
		m_uiLayer->m_myJetManager.Add(openRlt.res);
		//���򶯻�
		m_model3DLayer->m_stage.OpenSphere(true);

		m_speDmg.StartPlay(openRlt.colorindex);

		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 8);
	}
	else if (index == 5)
	{

		//��ӽ����¼
		m_uiLayer->m_resRecord.Add(openDMG.res[_ZERO]);
		m_uiLayer->m_myJetManager.Add(openDMG.res[_ZERO]);

		_PLAY_MUSCI_BACK(_KJJS, false);
		//������������	��������
		gameinfo->setbCountScore(true);
		m_uiLayer->m_jetandresManager.MyJettonOut(1);
		//�����л���ť, ��������ҽ������ز���
		m_uiLayer->m_jetandresManager.setVisible(false);
		//����ѹ�ֽ���
		m_uiLayer->m_myJetManager.StopPlay();
		//����÷�
		int getGold = m_uiLayer->m_resManager.StartPlay(
			openDMG.res[_ZERO],
			gameinfo->getRate(),
			m_uiLayer->m_myJetManager.GetRecord()
			);
		//������ҽ��
		AddGold(getGold);
		//�����б�
		m_uiLayer->m_userListManager.updateUserList(OneGameInfo::GetInstance()->getroomInfo()->vec);
		//�����Լ�
		m_uiLayer->m_userListManager.updateInfo(1);
		//�˹���8��
		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 8);

	}
	else if (index == 6)
	{
		//���ؿ�������
		m_uiLayer->m_jetandresManager.MyJettonOut(2);
		//����������
		m_speDmg.StopPlay();
		//��ͷ��Զ
		m_model3DLayer->m_camera3D.FarAway();

		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 7);

	}
	else if (index == 7)
	{
		//DMG��ʧ
		//m_speDmg.StopPlay();
		//�ر���
		m_model3DLayer->m_stage.OpenSphere(false);
		//�رճ������
		m_model3DLayer->m_lightAll.CloseLight();
		//�ر�������
		m_model3DLayer->m_sphere.CloseLight();
		//��ԭ
		m_nowIndex = _ZERO;
		//��ʼ������
		gameinfo->setopenRlt(OpenRes());
		//��ʼ�����⿪��
		gameinfo->setopenDMGRlt(OpenDMG());
		//�˹���4��

	}

	//ת12��DMG�߼�
	//auto gameInfo = OneGameInfo::GetInstance();
	//auto openDMG = gameInfo->getopenDMGRlt();

	//if (index == 1)
	//{
	//	//�󽱿�ʼ��Ч
	//	_PLAY_MUSCI_BACK(_DJKS, false);
	//	//���򶯻�
	//	m_model3DLayer->m_stage.OpenSphere(true);
	//	//����DMG
	//	m_speDmg.StartPlay();
	//	//DMG����+��Ч
	//	TitleLabel::SetTitle(TitleLabel::DMG);
	//	//����ѹ������
	//	m_uiLayer->m_myJetManager.SaveLastGame();
	//	//ѹ�ֽ�������
	//	m_uiLayer->m_jetandresManager.MyJettonOut(2);
	//	//�˹��̳���ʱ��Ϊ2S
	//	scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 2);
	//}
	//else if (index == 2)
	//{
	//	//ׯ���п�ʼת
	//	m_uiLayer->m_zhxControl.StartPlay(openDMG.res[_ZERO].e4);
	//	//����ȫ����
	//	m_model3DLayer->m_lightAll.OpenLight(enumAnimalColor::ColorMax);
	//	//�˹���Ϊ2�׶εĹ���
	//	scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 0.01);
	//}
	//else if (index == 3)
	//{
	//	//�Ƚ��˵ı�����������
	//	_PLAY_MUSCI_BACK(HI_TIME, true);
	//	//����DMG�ڼ���
	//	m_speDmg.setDMGIndex(12 - openDMG.leftIndex);
	//	//����ת�̿�ʼת
	//	m_model3DLayer->m_stage.StartPlay(openDMG.colorindex[12 - openDMG.leftIndex]);
	//	//ָ�뿪ʼת
	//	m_model3DLayer->m_animal.StartPlay(openDMG.animalindex[12 - openDMG.leftIndex], openDMG.colorindex[12 - openDMG.leftIndex]);
	//	//�˹��̳���ʱ��Ϊ14S
	//	scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 14);
	//}
	//else if (index == 4)
	//{
	//	//�õ�DMG���
	//	AnimalResult res = openDMG.res[12 - openDMG.leftIndex];
	//	//������Ч
	//	PlayAnimalMusic(enumSpecialRs::DMG, res.e3, res.e2);
	//	//�˹��̳���ʱ��Ϊ1S
	//	scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 1);
	//}
	//else if (index == 5)
	//{
	//	//ֹͣ������Ч
	//	_STOP_MUSCI_BACK;
	//	//��������
	//	m_model3DLayer->m_animal.ResDance(openDMG.colorindex[12 - openDMG.leftIndex]);
	//	//�˹��̳���ʱ��Ϊ
	//	scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 5);
	//}
	//else if (index == 6)
	//{
	//	//DMG�ظ�3~6�׶� 12��
	//	if (--openDMG.leftIndex > _ZERO)
	//	{
	//		//�˽׶ι��˻�++
	//		m_nowIndex = 2;
	//	}
	//	gameInfo->setopenDMGRlt(openDMG);
	//	//�˽׶�1��
	//	scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 1);
	//}
	//else if (index == 7)
	//{
	//	//��ӽ����¼
	//	m_uiLayer->m_resRecord.Add(openDMG.res[_ZERO]);
	//	m_uiLayer->m_myJetManager.Add(openDMG.res[_ZERO]);

	//	_PLAY_MUSCI_BACK(_KJJS, false);
	//	//������������	��������
	//	gameInfo->setbCountScore(true);
	//	m_uiLayer->m_jetandresManager.MyJettonOut(1);
	//	//�����л���ť, ��������ҽ������ز���
	//	m_uiLayer->m_jetandresManager.setVisible(false);
	//	//����ѹ�ֽ���
	//	m_uiLayer->m_myJetManager.StopPlay();
	//	//����÷�
	//	int getGold = m_uiLayer->m_resManager.StartPlay(
	//		openDMG.res[_ZERO],
	//		gameInfo->getRate(),
	//		m_uiLayer->m_myJetManager.GetRecord()
	//		);
	//	//������ҽ��
	//	AddGold(getGold);
	//	//�����б�
	//	m_uiLayer->m_userListManager.updateUserList(OneGameInfo::GetInstance()->getroomInfo()->vec);
	//	//�����Լ�
	//	m_uiLayer->m_userListManager.updateInfo(1);
	//	//�˹���8��
	//	scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 8);
	//}
	//else if (index == 8)
	//{
	//	//���ؿ�������
	//	m_uiLayer->m_jetandresManager.MyJettonOut(2);
	//	//�˹���1��
	//	scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 1);
	//}
	//else if (index == 9)
	//{
	//	//DMG��ʧ
	//	m_speDmg.StopPlay();
	//	//�ر���
	//	m_model3DLayer->m_stage.OpenSphere(false);
	//	//�رճ������
	//	m_model3DLayer->m_lightAll.CloseLight();
	//	//�ر�������
	//	m_model3DLayer->m_sphere.CloseLight();
	//	//��ԭ
	//	m_nowIndex = _ZERO;
	//	//��ʼ������
	//	gameInfo->setopenRlt(OpenRes());
	//	//��ʼ�����⿪��
	//	gameInfo->setopenDMGRlt(OpenDMG());
	//	//�˹���4��
	//	//CheckLive(4);
	//}
}

//���ſ��������Ч
void SLWHGameScene::PlayAnimalMusic(enumSpecialRs eRs, enumAnimalType eType, enumAnimalColor eColor)
{

	/*
		����˴����������⿪�� ���ǰ�����ͨ�Ķ������ͺ���ɫ���ݹ���
		��������Ҫ����ת����
		*/

	//�����⿪��������ͨ������Ч
	if (eRs == enumSpecialRs::CJ || eRs == enumSpecialRs::XYCJ || eRs == enumSpecialRs::_4LF || eRs == enumSpecialRs::DMG)
	{
		eRs = enumSpecialRs::Normal;
	}
	else if (eRs == enumSpecialRs::D3Y)
	{
		eColor = enumAnimalColor::ColorMax;
	}
	else if (eRs == enumSpecialRs::D4X)
	{
		eType = enumAnimalType::AnimalMax;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////


	int x = static_cast<int>(eRs)* 100 + static_cast<int>(eType)* 10 + static_cast<int>(eColor);
	switch (x)
	{
		//��ͨ����    
	case 100:
		_PLAY_MUSCI_EFF(RED_RABBIT);
		break;
	case 101:
		_PLAY_MUSCI_EFF(GREEN_RABBIT);
		break;
	case 102:
		_PLAY_MUSCI_EFF(YELLOW_RABBIT);
		break;
	case 110:
		_PLAY_MUSCI_EFF(RED_MONKEY);
		break;
	case 111:
		_PLAY_MUSCI_EFF(GREEN_MONKEY);
		break;
	case 112:
		_PLAY_MUSCI_EFF(YELLOW_MONKEY);
		break;
	case 120:
		_PLAY_MUSCI_EFF(RED_PANDA);
		break;
	case 121:
		_PLAY_MUSCI_EFF(GREEN_PANDA);
		break;
	case 122:
		_PLAY_MUSCI_EFF(YELLOW_PANDA);
		break;
	case 130:
		_PLAY_MUSCI_EFF(RED_LION);
		break;
	case 131:
		_PLAY_MUSCI_EFF(GREEN_LION);
		break;
	case 132:
		_PLAY_MUSCI_EFF(YELLOW_LION);
		break;
	case 303:
		_PLAY_MUSCI_EFF(LOTTERYTYPE_RABBIT);
		break;
	case 313:
		_PLAY_MUSCI_EFF(LOTTERYTYPE_MONKEY);
		break;
	case 323:
		_PLAY_MUSCI_EFF(LOTTERYTYPE_PANDA);
		break;
	case 333:
		_PLAY_MUSCI_EFF(LOTTERYTYPE_LION);
		break;
	case 440:
		_PLAY_MUSCI_EFF(LOTTERYTYPE_RED);
		break;
	case 441:
		_PLAY_MUSCI_EFF(LOTTERYTYPE_GREEN);
		break;
	case 442:
		_PLAY_MUSCI_EFF(LOTTERYTYPE_YELLOW);
		break;
	case 543:
		//���������
		break;
	default:
		break;
	}

}

void SLWHGameScene::checkLiveupdate(float t)
{
	if (!m_bIslive)
	{
		SLWHGameScene::m_uiLayer->m_uiMenu.exit(-1);
		SLWHNotice::openNotice(GETSTRING(1406), [](Ref*)
		{

			//���ش���
			Director::getInstance()->replaceScene(CCTransitionFade::create(0.5f, HallGameScene::createScene()));
		});
	}
}


void SLWHGameScene::CheckLive(float t)
{
	m_bIslive = false;
	unschedule(schedule_selector(SLWHGameScene::checkLiveupdate));
	scheduleOnce(schedule_selector(SLWHGameScene::checkLiveupdate), t + _CHECK_TIME);
}

void SLWHGameScene::updateSendHreat(float t)
{
	NetworkKernel::getInstance()->SendHeartPak();
	//����Ƿ��л�Ӧ
	CheckLive(_ZERO);
}

void SLWHGameScene::OnHreatPak()
{
	//�յ������� �����ߵ�
	m_bIslive = true;
}

