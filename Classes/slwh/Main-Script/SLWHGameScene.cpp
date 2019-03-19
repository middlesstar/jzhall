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

//定时检测是否服务端有响应
#define _HEART_TIME		10		//心跳包时间
#define _CHECK_TIME		3		//心跳包检查延迟时间

SLWHGameScene::SLWHGameScene()
{
	This = this;
	chat == nullptr;
	cj_root = nullptr;
	xycj_root = nullptr;
	timeDelay = 0;
	//outTime = 0;

	//注册进入后台和前端的处理事件
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
		//连接服务器
		NetworkKernel::getInstance()->start();
		NetworkKernel::getInstance()->SendLogonPacket();

		timeDelay = 0;
		schedule(schedule_selector(SLWHGameScene::goMainScene), 0.1);
	//}
	
	
}
void SLWHGameScene::comeToBack(Ref* ref)
{
	//time(&outTime);
	//清除押分
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

	//更新UI
	m_uiLayer->m_uiUserInfo.update();
	return true;
}

bool SLWHGameScene::AddYuanbao(int g)					//增加元宝
{
	auto info = Player::getInstance();
	info->addGold(g);
	//更新UI
	m_uiLayer->m_uiUserInfo.update();
	return true;
}

bool SLWHGameScene::AddTicket(int g)					//增加船票
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
	log("%s", a_u8("-------------------请等待下一局-----------------------"));


	auto gameinfo = OneGameInfo::GetInstance();

	m_uiLayer->m_resRecord.setVisible(false);

	m_uiLayer->m_jetandresManager.setVisible(false);
	//m_uiLayer->m_timeCount.StartPlay(_ZERO);
	//3D场景为押注等待界面
	m_uiLayer->SetEnble(false);
	//时间应急处理，这样处理不好，后期进行资源整合优化，过场加载界面会处理掉这里
	m_uiLayer->m_waitNextJet.StartPlay(lefttime);
	//CheckLive(lefttime);

	log("---------dais:WaitGames   over-------------");
	//TitleLabel::SetTitle(TitleLabel::Normal3);
}

void SLWHGameScene::StartGames(int leftTime)
{
	if (leftTime < 0) leftTime = 0;

	_PLAY_MUSCI_BACK(JET_WAIT, true);
	//点击进入游戏后启动

	auto gameInfo = OneGameInfo::GetInstance();

	//设置彩金总量
	m_uiLayer->m_jcNum.SetNum(gameInfo->getroomInfo()->lCj);
	//设置压分界面彩金
	m_uiLayer->m_myJetManager.setCaijin(gameInfo->getroomInfo()->lCj);
	//设置踏板颜色 
	m_model3DLayer->m_DownTo.ChangeColor((int*)gameInfo->getroomInfo()->dwColorIndex);
	//设置指针球颜色缓存
	m_model3DLayer->m_sphere.SetColorIndex((int*)gameInfo->getroomInfo()->dwColorIndex);
	//设置倍率
	m_uiLayer->m_myJetManager.SetRate(gameInfo->getRate());

	m_uiLayer->m_resRecord.setVisible(true);
	m_uiLayer->m_myJetManager.OpenRecord();
	m_uiLayer->m_jetandresManager.setVisible(true);
	//动物开始跳舞
	m_model3DLayer->m_animal.OpenAniNorAc();

	log("%s", a_u8("-----------------------开始押注--------------------"));

	m_uiLayer->SetEnble(true);


	//清空押注信息
	m_uiLayer->m_myJetManager.ClearJetton();
	m_uiLayer->m_userListManager.setTotelNum(_ZERO);

	auto roominfo = gameInfo->getroomInfo();
	for (std::vector<slwhPlayerInfo>::iterator itor = roominfo->vec.begin(); itor != roominfo->vec.end(); itor++)
	{
		itor->jet = _ZERO;
	}

	//是否开了自动
	if (m_uiLayer->m_myJetManager.GetAI())
	{
		if (!AddGold(-m_uiLayer->m_myJetManager.GetLastJetAll()))
		{//钱不够

			m_uiLayer->m_myJetManager.SetAI(false);
		}
		else
		{
			m_uiLayer->m_myJetManager.ContinuJet();
		}
	}


	//延迟3秒打开压分界面
	if (leftTime > 3)
	{
		scheduleOnce(schedule_selector(SLWHGameScene::delaypopJet), 3.f);
	}

	//开始倒计时
	m_uiLayer->m_jetandresManager.StartPlay(leftTime);
	//CheckLive(leftTime);

	// 显示压分界面，隐藏结果界面
	m_uiLayer->m_myJetManager.StartPlay();
	m_uiLayer->m_resManager.StopPlay();
	//刷新玩家列表
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

//关闭进程时的断开连接的处理
void SLWHGameScene::netDisconnect()
{
	//如果是在压分阶段，清除玩家压分
	auto gameinfo = OneGameInfo::GetInstance();

	//用户信息
	SLWHUserInfo info = OneGameInfo::GetInstance()->getUserInfo();
	//发送起立
	NetworkKernel::getInstance()->SendStandUpPacket(info.wTableID, info.wChairID, TRUE);
	//断开连接
	NetworkKernel::getInstance()->relese();

	//清空信息
	gameinfo->relese();
}

void SLWHGameScene::loadroomData()
{
	slwhRoomInfo* info = OneGameInfo::GetInstance()->getroomInfo();
	info->wType = GetHouseIndex::getInstance()->getIndex();


	//客户端写死房间压分信息
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

	//D3Y初始化
	m_speD3Y.setPosition3D(Vec3(_ZERO, 65.f, _ZERO));
	auto d3yspr = Sprite3D::create();
	CC_ASSERT(d3yspr != nullptr);
	d3yspr->setScale(30.f);
	d3yspr->setCameraMask((int)CameraFlag::USER1);
	m_speD3Y.AttachSprite3D(d3yspr);
	m_speD3Y.AttachAnimalAndStage(&m_model3DLayer->m_animal, &m_model3DLayer->m_stage);
	addChild(&m_speD3Y);

	//D4X初始化
	m_model3DLayer->m_stage.addChild(&m_speD4X);

	//特殊开奖CJ界面
	cj_root = CSLoader::createNode(COCOS_STUDIO_CJ);
	CC_ASSERT(cj_root != nullptr);
	addChild(cj_root);
	m_speCJ.AttachNode(cj_root->getChildByName("Special_CJ"));
	addChild(&m_speCJ);

	//特殊开奖 XYCJ界面
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

	//初始化特殊开奖
	initSpe();


	//开启聊天
	chat = Node::create();
	this->addChild(chat, 10);
	HallGameScene::openChatSprite(chat);

	////初始化压分面板的位置
	//m_uiLayer->m_jetandresManager.initposition();
	//初始化一次压分按钮
	m_uiLayer->m_myJetManager.SetJetNum(OneOfJetton::SetAddJet(1));

	auto gameInfo = OneGameInfo::GetInstance();
	//初始化当前环节
	m_nowIndex = _ZERO;
	//设置彩金总量
	m_uiLayer->m_jcNum.SetNum(gameInfo->getroomInfo()->lCj);
	//设置压分界面彩金
	m_uiLayer->m_myJetManager.setCaijin(gameInfo->getroomInfo()->lCj);
	//设置记录板数据
	m_uiLayer->m_resRecord.InitResList(gameInfo->getResRecord());
	m_uiLayer->m_myJetManager.InitResList(gameInfo->getResRecord());
	//启动定时器 等待服务端通知开始新的一局游戏

	log("-------------LeftTime:%d----------------------", (int)gameInfo->getLeftTime());
	m_nowState = gameInfo->getServerState();
	if (m_nowState != enumServerState::_Jet)		// 不是押注阶段 1  = 押注阶段 
	{
		WaitGames((int)gameInfo->getLeftTime() - 5);
	}
	else
	{
		//开始押注
		m_uiLayer->m_waitNextJet.destory();
		//时间应急处理，这样处理不好，后期进行资源整合优化，过场加载界面会处理掉这里
		StartGames((int)gameInfo->getLeftTime() - 5);
	}
	//更新玩家列表
	m_uiLayer->m_userListManager.InitUserList();

	m_uiLayer->m_userListManager.updateInfo();

	//监听服务端状态变化
	schedule(schedule_selector(SLWHGameScene::waitGameUpdate), 0.1f);

	//开启心跳包 	发送一次心跳包
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
	//有消息过来了 网络是好的
	m_bIslive = true;
	switch (m_nowState)
	{
	case enumServerState::_Jet:
		//下注阶段
		StartGames(33);
		m_uiLayer->m_userListManager.updateInfo();
		break;
	case enumServerState::_Open:
		//开奖结果
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

	//初始化特殊开奖
	initSpe();
	////初始化压分面板的位置
	//m_uiLayer->m_jetandresManager.initposition();
	//初始化一次压分按钮
	m_uiLayer->m_myJetManager.SetJetNum(OneOfJetton::SetAddJet(1));

	auto gameInfo = OneGameInfo::GetInstance();
	//初始化当前环节
	m_nowIndex = _ZERO;
	//设置彩金总量
	m_uiLayer->m_jcNum.SetNum(gameInfo->getroomInfo()->lCj);
	//设置压分界面彩金
	m_uiLayer->m_myJetManager.setCaijin(gameInfo->getroomInfo()->lCj);
	//设置记录板数据
	m_uiLayer->m_resRecord.InitResList(gameInfo->getResRecord());
	m_uiLayer->m_myJetManager.InitResList(gameInfo->getResRecord());
	//启动定时器 等待服务端通知开始新的一局游戏

	log("-------------LeftTime:%d----------------------", (int)gameInfo->getLeftTime());
	m_nowState = gameInfo->getServerState();
	if (m_nowState != enumServerState::_Jet)		// 不是押注阶段 1  = 押注阶段 
	{
		WaitGames((int)gameInfo->getLeftTime() - timeDelay);
	}
	else
	{
		//开始押注
		m_uiLayer->m_waitNextJet.destory();
		//时间应急处理，这样处理不好，后期进行资源整合优化，过场加载界面会处理掉这里
		StartGames((int)gameInfo->getLeftTime() - timeDelay);
	}
	//更新玩家列表
	m_uiLayer->m_userListManager.InitUserList();

	m_uiLayer->m_userListManager.updateInfo();

	//监听服务端状态变化
	schedule(schedule_selector(SLWHGameScene::waitGameUpdate), 0.1f);

	//开启心跳包 	发送一次心跳包
	updateSendHreat(0.f);
	schedule(schedule_selector(SLWHGameScene::updateSendHreat), _HEART_TIME);

	
}

void SLWHGameScene::delaypopJet(float t)
{
	m_uiLayer->m_jetandresManager.MyJettonOut(1);
}

void SLWHGameScene::OpenGames()
{
	scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 1);		//多停留1秒等待时间音效结束
}

void SLWHGameScene::GameRunning(float t)
{
	//提交游戏压分数据到服务器
	//得到结果数据

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
		log("%s", a_u8("-------------开奖结果出错------------"));
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

		//保存压分数据
		m_uiLayer->m_myJetManager.SaveLastGame();
		//压分界面不可用
		//m_uiLayer->m_myJetManager.enable(false);
		m_uiLayer->m_jetandresManager.MyJettonOut(2);
		//此过程持续时间为2S
		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 2);

	}
	else if (index == 2)
	{

		_PLAY_MUSCI_BACK(ZP_RUN, true);

		m_model3DLayer->m_stage.StartPlay(openRlt.colorindex);
		m_model3DLayer->m_animal.StartPlay(openRlt.animalindex, openRlt.colorindex);
		m_uiLayer->m_zhxControl.StartPlay(openRlt.res.e4);


		//此过程持续时间为14S
		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 14);
	}
	else if (index == 3)
	{

		AnimalResult res = openRlt.res;
		//播放音效
		PlayAnimalMusic(res.e1, res.e3, res.e2);

		//开启闪灯
		enumAnimalColor e = m_model3DLayer->m_sphere.OpenLight();
		m_model3DLayer->m_lightAll.OpenLight(e);

		//此过程持续时间为1S
		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 1);
	}
	else if (index == 4)
	{
		_STOP_MUSCI_BACK;
		//相机聚焦
		m_model3DLayer->m_camera3D.Close2Animal();
		//添加结果
		m_uiLayer->m_resRecord.Add(openRlt.res);
		m_uiLayer->m_myJetManager.Add(openRlt.res);
		//打开球动画
		m_model3DLayer->m_stage.OpenSphere(true);

		m_model3DLayer->m_animal.ResPlay(openRlt.colorindex, openRlt.res.e2);

		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 8);
	}
	else if (index == 5)
	{

		_PLAY_MUSCI_BACK(_KJJS, false);

		//下拉开奖界面	
		gameinfo->setbCountScore(true);
		m_uiLayer->m_jetandresManager.MyJettonOut(1);
		m_uiLayer->m_jetandresManager.setVisible(false);
		int getGold = m_uiLayer->m_resManager.StartPlay(
			openRlt.res,
			gameinfo->getRate(),
			m_uiLayer->m_myJetManager.GetRecord()
			);

		//隐藏压分界面
		m_uiLayer->m_myJetManager.StopPlay();
		//增加玩家金币
		AddGold(getGold);
		//更新列表
		m_uiLayer->m_userListManager.updateUserList(gameinfo->getroomInfo()->vec);
		m_uiLayer->m_userListManager.updateInfo(1);
		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 8);
	}
	else if (index == 6)
	{
		//拉回开奖界面
		m_uiLayer->m_jetandresManager.MyJettonOut(2);
		//动物还原位置
		m_model3DLayer->m_animal.ResStop();
		//镜头拉远
		m_model3DLayer->m_camera3D.FarAway();

		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 1);

	}
	else if (index == 7)
	{
		//关闭球
		m_model3DLayer->m_stage.OpenSphere(false);
		//关闭场景大灯
		m_model3DLayer->m_lightAll.CloseLight();
		//关闭球闪灯
		m_model3DLayer->m_sphere.CloseLight();
		//索引初始化
		m_nowIndex = _ZERO;
		//初始化开奖
		gameinfo->setopenRlt(OpenRes());
		//此过程4秒（预留1秒）
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

		//保存压分数据
		m_uiLayer->m_myJetManager.SaveLastGame();
		//压分界面不可用
		m_uiLayer->m_jetandresManager.MyJettonOut(2);

		//打开球动画
		//m_model3DLayer->m_stage.OpenSphere(true);

		//相机聚焦
		m_model3DLayer->m_camera3D.Close2AnimalMid();

		//此过程持续时间为2S
		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 2);

	}
	else if (index == 2)
	{

		_PLAY_MUSCI_BACK(HI_TIME, true);

		m_model3DLayer->m_stage.StartPlay(openRlt.colorindex);
		m_model3DLayer->m_animal.StartPlay(openRlt.animalindex, openRlt.colorindex);
		m_uiLayer->m_zhxControl.StartPlay(openRlt.res.e4);

		//开启闪灯
		m_model3DLayer->m_lightAll.OpenLight(enumAnimalColor::ColorMax);

		//此过程持续时间为14S
		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 14);
	}
	else if (index == 3)
	{

		//播放音效
		PlayAnimalMusic(openRlt.res.e1, openRlt.res.e3, openRlt.res.e2);

		//此过程持续时间为1S
		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 1);
	}
	else if (index == 4)
	{
		_STOP_MUSCI_BACK;
		//相机聚焦
		m_model3DLayer->m_camera3D.Close2Animal();

		//打开球动画
		m_model3DLayer->m_stage.OpenSphere(true);

		//添加结果
		m_uiLayer->m_resRecord.Add(openRlt.res);
		m_uiLayer->m_myJetManager.Add(openRlt.res);

		m_model3DLayer->m_animal.ResPlay(openRlt.colorindex, openRlt.res.e2);

		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 8);
	}
	else if (index == 5)
	{
		_PLAY_MUSCI_BACK(_KJJS, false);

		//下拉开奖界面	
		gameinfo->setbCountScore(true);
		m_uiLayer->m_jetandresManager.MyJettonOut(1);
		m_uiLayer->m_jetandresManager.setVisible(false);
		int getGold = m_uiLayer->m_resManager.StartPlay(
			openRlt.res,
			gameinfo->getRate(),
			m_uiLayer->m_myJetManager.GetRecord()
			);
		//隐藏压分界面
		m_uiLayer->m_myJetManager.StopPlay();
		//增加玩家金币
		AddGold(getGold);
		//更新列表
		m_uiLayer->m_userListManager.updateUserList(gameinfo->getroomInfo()->vec);
		m_uiLayer->m_userListManager.updateInfo(1);
		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 8);
	}
	else if (index == 6)
	{


		//拉回开奖界面
		m_uiLayer->m_jetandresManager.MyJettonOut(2);
		//动物还原位置
		m_model3DLayer->m_animal.ResStop();
		//镜头拉远
		m_model3DLayer->m_camera3D.FarAway();

		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 1);

		//D3Y消失
		//m_speD3Y.StopPlay();

		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 1);

	}
	else if (index == 7)
	{
		//关闭球
		m_model3DLayer->m_stage.OpenSphere(false);
		//关闭场景大灯
		m_model3DLayer->m_lightAll.CloseLight();
		//关闭球闪灯
		m_model3DLayer->m_sphere.CloseLight();
		//复位
		m_nowIndex = _ZERO;
		//初始化开奖
		gameinfo->setopenRlt(OpenRes());
		//此过程4秒（预留1秒）
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

		//保存压分数据
		m_uiLayer->m_myJetManager.SaveLastGame();
		//拉回压分界面
		m_uiLayer->m_jetandresManager.MyJettonOut(2);
		//相机聚焦
		m_model3DLayer->m_camera3D.Close2AnimalMid();
		//此过程持续时间为2S
		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 2);
	}
	else if (index == 2)
	{

		_PLAY_MUSCI_BACK(HI_TIME, true);


		m_model3DLayer->m_stage.StartPlay(openRlt.colorindex);
		m_model3DLayer->m_animal.StartPlay(openRlt.animalindex, openRlt.colorindex);
		m_uiLayer->m_zhxControl.StartPlay(openRlt.res.e4);

		//开启闪灯
		m_model3DLayer->m_lightAll.OpenLight(enumAnimalColor::ColorMax);

		//此过程持续时间为14S
		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 14);
	}
	else if (index == 3)
	{

		AnimalResult res = openRlt.res;
		//播放音效
		PlayAnimalMusic(res.e1, res.e3, res.e2);

		//此过程持续时间为1S
		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 1);
	}
	else if (index == 4)
	{
		//相机聚焦
		m_model3DLayer->m_camera3D.Close2Animal();

		//_STOP_MUSCI_BACK;
		//添加结果
		m_uiLayer->m_resRecord.Add(openRlt.res);
		m_uiLayer->m_myJetManager.Add(openRlt.res);

		//打开球
		m_model3DLayer->m_stage.OpenSphere(true);

		//D4X特殊动画逻辑
		m_speD4X.StartPlay();

		//动物向中间移动
		//m_model3DLayer->m_animal.ResPlay(openRlt.colorindex, openRlt.res.e2);

		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 8);
	}
	else if (index == 5)
	{

		_PLAY_MUSCI_BACK(_KJJS, false);

		//下拉开奖界面	
		gameinfo->setbCountScore(true);
		m_uiLayer->m_jetandresManager.MyJettonOut(1);
		m_uiLayer->m_jetandresManager.setVisible(false);
		int getGold = m_uiLayer->m_resManager.StartPlay(
			openRlt.res,
			gameinfo->getRate(),
			m_uiLayer->m_myJetManager.GetRecord()
			);
		//隐藏压分界面
		m_uiLayer->m_myJetManager.StopPlay();
		//增加玩家金币
		AddGold(getGold);
		//更新列表
		m_uiLayer->m_userListManager.updateUserList(gameinfo->getroomInfo()->vec);
		m_uiLayer->m_userListManager.updateInfo(1);
		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 8);
	}
	else if (index == 6)
	{

		//拉回开奖界面
		m_uiLayer->m_jetandresManager.MyJettonOut(2);
		//动物还原位置
		//m_model3DLayer->m_animal.ResStop();
		//镜头拉远
		m_model3DLayer->m_camera3D.FarAway();
		//D3Y消失
		//m_speD3Y.StopPlay();
		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 1);

	}
	else if (index == 7)
	{
		//关闭球
		m_model3DLayer->m_stage.OpenSphere(false);

		m_speD4X.StopPlay();
		//关闭场景大灯
		m_model3DLayer->m_lightAll.CloseLight();

		//关闭球闪灯
		m_model3DLayer->m_sphere.CloseLight();
		//还原
		m_nowIndex = _ZERO;
		//初始化开奖
		gameinfo->setopenRlt(OpenRes());
		//此过程4秒
		//CheckLive(4);
	}
}

void SLWHGameScene::_4LF(int index)
{
	//得到4连发结果
	auto gameInfo = OneGameInfo::GetInstance();
	Open4LF open4LF = gameInfo->getopen4LFRlt();

	if (index == 1)
	{
		//大奖开始音效
		_PLAY_MUSCI_BACK(_DJKS, false);
		//4连发标题+特效
		TitleLabel::SetTitle(TitleLabel::_4lian);
		//保存压分数据
		m_uiLayer->m_myJetManager.SaveLastGame();
		//压分界面缩回
		m_uiLayer->m_jetandresManager.MyJettonOut(2);
		//此过程持续时间为2S
		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 2);
	}
	else if (index == 2)
	{

		//庄和闲开始转
		m_uiLayer->m_zhxControl.StartPlay(gameInfo->getopenRlt().res.e4);
		//开启全闪灯
		m_model3DLayer->m_lightAll.OpenLight(enumAnimalColor::ColorMax);
		//此过程为2阶段的过渡
		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 0.01);
	}
	else if (index == 3)
	{
		//比较嗨的背景音乐响起
		_PLAY_MUSCI_BACK(HI_TIME, true);
		//动物转盘开始转
		m_model3DLayer->m_stage.StartPlay(open4LF.colorindex[4 - open4LF.leftIndex]);
		//指针开始转
		m_model3DLayer->m_animal.StartPlay(open4LF.animalindex[4 - open4LF.leftIndex], open4LF.colorindex[4 - open4LF.leftIndex]);
		//此过程持续时间为14S
		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 14);
	}
	else if (index == 4)
	{
		//得到4连发结果
		AnimalResult res = open4LF.res[4 - open4LF.leftIndex];
		//播放音效
		PlayAnimalMusic(enumSpecialRs::_4LF, open4LF.res[4 - open4LF.leftIndex].e3, open4LF.res[4 - open4LF.leftIndex].e2);
		//此过程持续时间为1S
		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 1);
	}
	else if (index == 5)
	{
		//停止背景音效
		_STOP_MUSCI_BACK;
		//打开球动画
		m_model3DLayer->m_stage.OpenSphere(true);
		//相机聚焦
		m_model3DLayer->m_camera3D.Close2Animal();
		//添加结果记录
		m_uiLayer->m_resRecord.Add(open4LF.res[4 - open4LF.leftIndex]);
		m_uiLayer->m_myJetManager.Add(open4LF.res[4 - open4LF.leftIndex]);
		//动物在中间展示
		m_model3DLayer->m_animal.ResPlay(open4LF.colorindex[4 - open4LF.leftIndex], open4LF.res[4 - open4LF.leftIndex].e2);
		//此过程持续时间为
		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 8);

	}
	else if (index == 6)
	{
		//最后一轮时候跳过此阶段
		if (--open4LF.leftIndex > _ZERO)
		{
			//动物还原位置
			m_model3DLayer->m_animal.ResStop();
			//镜头拉远
			m_model3DLayer->m_camera3D.FarAway();
			//此阶段1秒
			scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 1);
		}
		else
		{
			scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 0.01);
		}

	}
	else if (index == 7)
	{
		//4连发重复3~7阶段 4次

		//最后一轮跳过此阶段
		if (--open4LF.leftIndex > _ZERO)
		{
			//此阶段过了会++
			m_nowIndex = 2;
			//关闭球
			m_model3DLayer->m_stage.OpenSphere(false);
			//此阶段3秒
			scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 3);
		}
		else
		{
			scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 0.01);
		}
		//刷新4lf结果
		gameInfo->setopen4LFRlt(open4LF);
	}
	else if (index == 8)
	{
		_PLAY_MUSCI_BACK(_KJJS, false);
		gameInfo->setbCountScore(true);
		//下拉开奖界面	弹出界面
		m_uiLayer->m_jetandresManager.MyJettonOut(1);
		//隐藏切换按钮, 不允许玩家进行缩回操作
		m_uiLayer->m_jetandresManager.setVisible(false);
		//隐藏压分界面
		m_uiLayer->m_myJetManager.StopPlay();
		//计算得分
		int getGold = m_uiLayer->m_resManager.StartPlay(
			gameInfo->getopenRlt().res,
			gameInfo->getRate(),
			m_uiLayer->m_myJetManager.GetRecord(),
			open4LF.res
			);
		//增加玩家金币
		AddGold(getGold);
		//更新列表
		m_uiLayer->m_userListManager.updateUserList(OneGameInfo::GetInstance()->getroomInfo()->vec);
		//更新自己
		m_uiLayer->m_userListManager.updateInfo(1);
		//此过程8秒
		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 8);
	}
	else if (index == 9)
	{

		//拉回开奖界面
		m_uiLayer->m_jetandresManager.MyJettonOut(2);
		//动物还原位置
		m_model3DLayer->m_animal.ResStop();
		//镜头拉远
		m_model3DLayer->m_camera3D.FarAway();
		//此过程1秒
		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 1);

	}
	else if (index == 10)
	{
		//关闭球
		m_model3DLayer->m_stage.OpenSphere(false);
		//关闭场景大灯
		m_model3DLayer->m_lightAll.CloseLight();
		//关闭球闪灯
		m_model3DLayer->m_sphere.CloseLight();
		//复位
		m_nowIndex = _ZERO;
		//初始化开奖
		gameInfo->setopenRlt(OpenRes());
		//初始化特殊开奖
		gameInfo->setopen4LFRlt(Open4LF());
		//此过程4秒
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
		//保存压分数据
		m_uiLayer->m_myJetManager.SaveLastGame();
		m_uiLayer->m_jetandresManager.MyJettonOut(2);
		//压分界面不可用
		//m_uiLayer->m_myJetManager.enable(false);
		//此过程持续时间为2S
		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 2);

	}
	else if (index == 2)
	{

		_PLAY_MUSCI_BACK(HI_TIME, true);

		m_model3DLayer->m_stage.StartPlay(openRlt.colorindex);
		m_model3DLayer->m_animal.StartPlay(openRlt.animalindex, openRlt.colorindex);
		m_uiLayer->m_zhxControl.StartPlay(openRlt.res.e4);

		//开启闪灯
		m_model3DLayer->m_lightAll.OpenLight(enumAnimalColor::ColorMax);

		//此过程持续时间为14S
		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 14);
	}
	else if (index == 3)
	{

		AnimalResult res = openRlt.res;
		//播放音效
		PlayAnimalMusic(res.e1, res.e3, res.e2);

		//此过程持续时间为1S
		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 1);
	}
	else if (index == 4)
	{
		_STOP_MUSCI_BACK;
		//相机聚焦
		m_model3DLayer->m_camera3D.Close2Animal();
		//添加结果
		m_uiLayer->m_resRecord.Add(gameinfo->getopenRlt().res);

		m_uiLayer->m_myJetManager.Add(gameinfo->getopenRlt().res);

		//打开球动画
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


		//下拉开奖界面	
		gameinfo->setbCountScore(true);
		m_uiLayer->m_jetandresManager.MyJettonOut(1);
		m_uiLayer->m_jetandresManager.setVisible(false);
		int getGold = m_uiLayer->m_resManager.StartPlay(
			gameinfo->getopenRlt().res,
			gameinfo->getRate(),
			m_uiLayer->m_myJetManager.GetRecord(),
			cj_get_num);
		//隐藏压分界面
		m_uiLayer->m_myJetManager.StopPlay();
		//增加玩家金币
		AddGold(getGold);
		//更新列表
		m_uiLayer->m_userListManager.updateUserList(gameinfo->getroomInfo()->vec);
		m_uiLayer->m_userListManager.updateInfo(1);
		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 8);
	}
	else if (index == 7)
	{
		SLWH_CD::StopPlay();
		//拉回开奖界面
		//m_uiLayer->m_resManager.StopPlay();
		//动物还原位置
		m_model3DLayer->m_animal.ResStop();
		//镜头拉远
		m_model3DLayer->m_camera3D.FarAway();

		m_uiLayer->m_jetandresManager.MyJettonOut(2);
		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 1);

	}
	else if (index == 8)
	{
		//关闭球
		m_model3DLayer->m_stage.OpenSphere(false);
		//关闭场景大灯
		m_model3DLayer->m_lightAll.CloseLight();
		//关闭球闪灯
		m_model3DLayer->m_sphere.CloseLight();
		//还原
		m_nowIndex = _ZERO;
		//初始化开奖
		gameinfo->setopenRlt(OpenRes());
		//初始化特殊开奖
		gameinfo->setopenCJRlt(OpenCJ());
		//此过程4秒
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
		//保存压分数据
		m_uiLayer->m_myJetManager.SaveLastGame();
		m_uiLayer->m_jetandresManager.MyJettonOut(2);
		//压分界面不可用
		//m_uiLayer->m_myJetManager.enable(false);
		//相机聚焦
		//m_model3DLayer->m_camera3D.Close2AnimalMid();
		//此过程持续时间为2S
		//开启XYCJ特效
		m_speXYCJ.StartPlayEff();

		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 2);

	}
	else if (index == 2)
	{

		_PLAY_MUSCI_BACK(HI_TIME, true);

		m_model3DLayer->m_stage.StartPlay(openRlt.colorindex);
		m_model3DLayer->m_animal.StartPlay(openRlt.animalindex, openRlt.colorindex);
		m_uiLayer->m_zhxControl.StartPlay(openRlt.res.e4);

		//开启闪灯
		m_model3DLayer->m_lightAll.OpenLight(enumAnimalColor::ColorMax);

		//此过程持续时间为14S
		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 14);
	}
	else if (index == 3)
	{

		//播放音效
		PlayAnimalMusic(openRlt.res.e1, openRlt.res.e3, openRlt.res.e2);

		//此过程持续时间为1S
		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 1);
	}
	else if (index == 4)
	{
		_STOP_MUSCI_BACK;
		//打开球动画
		m_model3DLayer->m_stage.OpenSphere(true);
		//相机聚焦
		m_model3DLayer->m_camera3D.Close2Animal();
		//添加结果
		m_uiLayer->m_resRecord.Add(openRlt.res);
		m_uiLayer->m_myJetManager.Add(openRlt.res);
		//动物移动
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

		//下拉开奖界面	
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
		//隐藏压分界面
		m_uiLayer->m_myJetManager.StopPlay();
		//增加玩家金币
		AddGold(getGold);
		//增加玩家船票
		AddYuanbao(m_uiLayer->m_resManager.getyuanbao());
		//增加玩家元宝
		AddTicket(m_uiLayer->m_resManager.getticket());
		//更新列表
		m_uiLayer->m_userListManager.updateUserList(gameinfo->getroomInfo()->vec);
		m_uiLayer->m_userListManager.updateInfo(1);
		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 8);
	}
	else if (index == 7)
	{

		//拉回开奖界面
		m_uiLayer->m_jetandresManager.MyJettonOut(2);
		//动物还原位置
		m_model3DLayer->m_animal.ResStop();
		//镜头拉远
		m_model3DLayer->m_camera3D.FarAway();

		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 1);

	}
	else if (index == 8)
	{
		//关闭XYCJ 特效
		m_speXYCJ.StopPlayEff();
		//关闭球
		m_model3DLayer->m_stage.OpenSphere(false);
		//关闭场景大灯
		m_model3DLayer->m_lightAll.CloseLight();
		//关闭球闪灯
		m_model3DLayer->m_sphere.CloseLight();
		//还原
		m_nowIndex = _ZERO;
		//初始化开奖
		gameinfo->setopenRlt(OpenRes());
		//初始化特殊开奖
		gameinfo->setopenXYCJRlt(OpenXYCJ());
		//此过程4秒
		//CheckLive(4);
	}
}

void SLWHGameScene::DMG(int index)
{
	//得到DMG连发结果
	auto gameinfo = OneGameInfo::GetInstance();
	auto openRlt = gameinfo->getopenRlt();
	auto openDMG = gameinfo->getopenDMGRlt();

	if (index == 1)
	{
		_PLAY_MUSCI_BACK(_DJKS, false);

		//保存压分数据
		m_uiLayer->m_myJetManager.SaveLastGame();
		//压分界面不可用
		//m_uiLayer->m_myJetManager.enable(false);
		//DMG标题+特效
		TitleLabel::SetTitle(TitleLabel::DMG);
		//压分界面缩回
		m_uiLayer->m_jetandresManager.MyJettonOut(2);
		//此过程持续时间为2S
		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 2);

	}
	else if (index == 2)
	{

		_PLAY_MUSCI_BACK(HI_TIME, true);

		//开启闪灯
		m_model3DLayer->m_lightAll.OpenLight(enumAnimalColor::ColorMax);

		m_model3DLayer->m_stage.StartPlay(openRlt.colorindex);
		m_model3DLayer->m_animal.StartPlay(openRlt.animalindex, openRlt.colorindex);
		m_uiLayer->m_zhxControl.StartPlay(openRlt.res.e4);


		//此过程持续时间为14S
		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 14);
	}
	else if (index == 3)
	{

		AnimalResult res = openRlt.res;
		//播放音效
		PlayAnimalMusic(res.e1, res.e3, res.e2);

		//开启闪灯
		//enumAnimalColor e = m_model3DLayer->m_sphere.OpenLight();
		//m_model3DLayer->m_lightAll.OpenLight(e);
		m_model3DLayer->m_sphere.OpenLight();

		//此过程持续时间为1S
		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 1);
	}
	else if (index == 4)
	{
		_STOP_MUSCI_BACK;
		//相机聚焦
		m_model3DLayer->m_camera3D.Close2AnimalMid();
		//添加结果
		m_uiLayer->m_resRecord.Add(openRlt.res);
		m_uiLayer->m_myJetManager.Add(openRlt.res);
		//打开球动画
		m_model3DLayer->m_stage.OpenSphere(true);

		m_speDmg.StartPlay(openRlt.colorindex);

		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 8);
	}
	else if (index == 5)
	{

		//添加结果记录
		m_uiLayer->m_resRecord.Add(openDMG.res[_ZERO]);
		m_uiLayer->m_myJetManager.Add(openDMG.res[_ZERO]);

		_PLAY_MUSCI_BACK(_KJJS, false);
		//下拉开奖界面	弹出界面
		gameinfo->setbCountScore(true);
		m_uiLayer->m_jetandresManager.MyJettonOut(1);
		//隐藏切换按钮, 不允许玩家进行缩回操作
		m_uiLayer->m_jetandresManager.setVisible(false);
		//隐藏压分界面
		m_uiLayer->m_myJetManager.StopPlay();
		//计算得分
		int getGold = m_uiLayer->m_resManager.StartPlay(
			openDMG.res[_ZERO],
			gameinfo->getRate(),
			m_uiLayer->m_myJetManager.GetRecord()
			);
		//增加玩家金币
		AddGold(getGold);
		//更新列表
		m_uiLayer->m_userListManager.updateUserList(OneGameInfo::GetInstance()->getroomInfo()->vec);
		//更新自己
		m_uiLayer->m_userListManager.updateInfo(1);
		//此过程8秒
		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 8);

	}
	else if (index == 6)
	{
		//拉回开奖界面
		m_uiLayer->m_jetandresManager.MyJettonOut(2);
		//结束大满贯
		m_speDmg.StopPlay();
		//镜头拉远
		m_model3DLayer->m_camera3D.FarAway();

		scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 7);

	}
	else if (index == 7)
	{
		//DMG消失
		//m_speDmg.StopPlay();
		//关闭球
		m_model3DLayer->m_stage.OpenSphere(false);
		//关闭场景大灯
		m_model3DLayer->m_lightAll.CloseLight();
		//关闭球闪灯
		m_model3DLayer->m_sphere.CloseLight();
		//还原
		m_nowIndex = _ZERO;
		//初始化开奖
		gameinfo->setopenRlt(OpenRes());
		//初始化特殊开奖
		gameinfo->setopenDMGRlt(OpenDMG());
		//此过程4秒

	}

	//转12次DMG逻辑
	//auto gameInfo = OneGameInfo::GetInstance();
	//auto openDMG = gameInfo->getopenDMGRlt();

	//if (index == 1)
	//{
	//	//大奖开始音效
	//	_PLAY_MUSCI_BACK(_DJKS, false);
	//	//打开球动画
	//	m_model3DLayer->m_stage.OpenSphere(true);
	//	//出现DMG
	//	m_speDmg.StartPlay();
	//	//DMG标题+特效
	//	TitleLabel::SetTitle(TitleLabel::DMG);
	//	//保存压分数据
	//	m_uiLayer->m_myJetManager.SaveLastGame();
	//	//压分界面缩回
	//	m_uiLayer->m_jetandresManager.MyJettonOut(2);
	//	//此过程持续时间为2S
	//	scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 2);
	//}
	//else if (index == 2)
	//{
	//	//庄和闲开始转
	//	m_uiLayer->m_zhxControl.StartPlay(openDMG.res[_ZERO].e4);
	//	//开启全闪灯
	//	m_model3DLayer->m_lightAll.OpenLight(enumAnimalColor::ColorMax);
	//	//此过程为2阶段的过渡
	//	scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 0.01);
	//}
	//else if (index == 3)
	//{
	//	//比较嗨的背景音乐响起
	//	_PLAY_MUSCI_BACK(HI_TIME, true);
	//	//设置DMG第几轮
	//	m_speDmg.setDMGIndex(12 - openDMG.leftIndex);
	//	//动物转盘开始转
	//	m_model3DLayer->m_stage.StartPlay(openDMG.colorindex[12 - openDMG.leftIndex]);
	//	//指针开始转
	//	m_model3DLayer->m_animal.StartPlay(openDMG.animalindex[12 - openDMG.leftIndex], openDMG.colorindex[12 - openDMG.leftIndex]);
	//	//此过程持续时间为14S
	//	scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 14);
	//}
	//else if (index == 4)
	//{
	//	//得到DMG结果
	//	AnimalResult res = openDMG.res[12 - openDMG.leftIndex];
	//	//播放音效
	//	PlayAnimalMusic(enumSpecialRs::DMG, res.e3, res.e2);
	//	//此过程持续时间为1S
	//	scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 1);
	//}
	//else if (index == 5)
	//{
	//	//停止背景音效
	//	_STOP_MUSCI_BACK;
	//	//动物跳舞
	//	m_model3DLayer->m_animal.ResDance(openDMG.colorindex[12 - openDMG.leftIndex]);
	//	//此过程持续时间为
	//	scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 5);
	//}
	//else if (index == 6)
	//{
	//	//DMG重复3~6阶段 12次
	//	if (--openDMG.leftIndex > _ZERO)
	//	{
	//		//此阶段过了会++
	//		m_nowIndex = 2;
	//	}
	//	gameInfo->setopenDMGRlt(openDMG);
	//	//此阶段1秒
	//	scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 1);
	//}
	//else if (index == 7)
	//{
	//	//添加结果记录
	//	m_uiLayer->m_resRecord.Add(openDMG.res[_ZERO]);
	//	m_uiLayer->m_myJetManager.Add(openDMG.res[_ZERO]);

	//	_PLAY_MUSCI_BACK(_KJJS, false);
	//	//下拉开奖界面	弹出界面
	//	gameInfo->setbCountScore(true);
	//	m_uiLayer->m_jetandresManager.MyJettonOut(1);
	//	//隐藏切换按钮, 不允许玩家进行缩回操作
	//	m_uiLayer->m_jetandresManager.setVisible(false);
	//	//隐藏压分界面
	//	m_uiLayer->m_myJetManager.StopPlay();
	//	//计算得分
	//	int getGold = m_uiLayer->m_resManager.StartPlay(
	//		openDMG.res[_ZERO],
	//		gameInfo->getRate(),
	//		m_uiLayer->m_myJetManager.GetRecord()
	//		);
	//	//增加玩家金币
	//	AddGold(getGold);
	//	//更新列表
	//	m_uiLayer->m_userListManager.updateUserList(OneGameInfo::GetInstance()->getroomInfo()->vec);
	//	//更新自己
	//	m_uiLayer->m_userListManager.updateInfo(1);
	//	//此过程8秒
	//	scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 8);
	//}
	//else if (index == 8)
	//{
	//	//拉回开奖界面
	//	m_uiLayer->m_jetandresManager.MyJettonOut(2);
	//	//此过程1秒
	//	scheduleOnce(schedule_selector(SLWHGameScene::Bridge), 1);
	//}
	//else if (index == 9)
	//{
	//	//DMG消失
	//	m_speDmg.StopPlay();
	//	//关闭球
	//	m_model3DLayer->m_stage.OpenSphere(false);
	//	//关闭场景大灯
	//	m_model3DLayer->m_lightAll.CloseLight();
	//	//关闭球闪灯
	//	m_model3DLayer->m_sphere.CloseLight();
	//	//还原
	//	m_nowIndex = _ZERO;
	//	//初始化开奖
	//	gameInfo->setopenRlt(OpenRes());
	//	//初始化特殊开奖
	//	gameInfo->setopenDMGRlt(OpenDMG());
	//	//此过程4秒
	//	//CheckLive(4);
	//}
}

//播放开奖结果音效
void SLWHGameScene::PlayAnimalMusic(enumSpecialRs eRs, enumAnimalType eType, enumAnimalColor eColor)
{

	/*
		服务端传过来的特殊开奖 都是按照普通的动物类型和颜色传递过来
		在这里需要进行转换下
		*/

	//此特殊开奖播放普通开奖音效
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
		//普通开奖    
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
		//大满贯打酱油
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

			//返回大厅
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
	//检查是否有回应
	CheckLive(_ZERO);
}

void SLWHGameScene::OnHreatPak()
{
	//收到心跳包 是在线的
	m_bIslive = true;
}

