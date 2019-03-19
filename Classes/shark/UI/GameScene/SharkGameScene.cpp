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


static int general[20] = { 0, 1, 3, 4, 6, 7, 8, 10, 11, 12, 14, 15, 17, 18, 20, 21, 22, 24, 25, 26 }; //普通动物索引
static int goldShark[3] = { 2, 9, 16};                                                        //金鲨索引
static int silverShark[3] = { 13, 19, 27 };                                                   //银鲨索引
static int color[2] = { 5, 23 }; 

//10秒定时检测是否服务端有响应
#define _HEART_TIME		10	//心跳包时间
#define _CHECK_TIME		3  //彩色鲨鱼索引

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

	//注册进入后台和前端的处理事件
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
	//清除押分
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
		//连接服务器
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



//进入游戏主界面
void SharkGameScene::goMainScene(float t)
{
	timeDelay++;
	if (!GameInfo::GetInstance()->getstartGameFinish()) return;

	unschedule(schedule_selector(SharkGameScene::goMainScene));
	
	m_mainView = MainView::create();
	CC_ASSERT(m_mainView != nullptr);
	addChild(m_mainView);


	//开启心跳包
	updateSendHreat(0.f);
	schedule(schedule_selector(SharkGameScene::updateSendHreat), _HEART_TIME);

	auto gameInfo = GameInfo::GetInstance();
	m_nowState = gameInfo->getserverState();

	
	//播放音乐
	gameInfo->selectMusicBack();
	m_nowIndex = 0;
	//设置压分界面彩金
	m_mainView->stakeManager.setCaijin(gameInfo->getroomInfor()->lCJ);

	//设置游戏界面彩金总量
	m_mainView->battleManager.setColorMoney(gameInfo->getroomInfor()->lCJ);

	//启动定时器 等待服务端通知开始新的一局游戏

	//显示记录板
	m_mainView->recordManager.updateBoard(1);
	
	
	if (m_nowState != gameState::STAKE)		//1 = 押注阶段
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
		//销毁等待界面
		//m_mainView->waitNextManager.destory();
		//开始押注
		m_mainView->stake_rootNode->setVisible(true);
		m_mainView->m_playerListManager.changeView(1);
		if (gameInfo->getLeftTime() > timeDelay)
		{
			gameStart(gameInfo->getLeftTime() - timeDelay);
		}	
		schedule(schedule_selector(SharkGameScene::waitUpdate), 0.1f);
	}
	
	//更新玩家列表
	m_mainView->m_playerListManager.InitPlayerList();
	//更新个人信息
	m_mainView->m_playerListManager.updateInfo();

	/*----------------添加聊天框的节点----------------------*/

	chat = Node::create();
	this->addChild(chat,10);
	//开启聊天
	HallGameScene::openChatSprite(chat);
	/*--------------------------------------------------------*/

	
}


void SharkGameScene::gameStart(int leftTime)
{
	//开启金币堆定时器
	m_mainView->stakeManager.startGoldheap();
	//改变倒计时的位置
	m_mainView->m_playerListManager.changeView(1);
	

	//闪烁按钮上的光圈
	if (leftTime > 5)
	{
		m_mainView->stakeManager.showCircle();
	}
	
	auto gameInfo = GameInfo::GetInstance();

	//设置彩金总量
	m_mainView->battleManager.setColorMoney(gameInfo->getroomInfor()->lCJ);

	//设置压分界面彩金
	m_mainView->stakeManager.setCaijin(gameInfo->getroomInfor()->lCJ);
	//设置倍率
	m_mainView->stakeManager.SetRate(gameInfo->getRate());
	//设置所有玩家总押分
	m_mainView->m_playerListManager.cancelTotleNum();

	//清空押注信息
	m_mainView->stakeManager.ClearJetton();

	
	if (m_mainView->stakeManager.GetAI())
	{
			m_mainView->stakeManager.ContinuStake();
	}

	//开始倒计时
	m_mainView->sharkTimeCount.StartPlay(leftTime, 1);

	for (std::vector<UserInfo_Shark>::iterator itor = gameInfo->getroomInfor()->vec.begin(); itor != gameInfo->getroomInfor()->vec.end(); itor++)
	{
		(*itor).winscore = _ZERO;
	}
	m_mainView->m_playerListManager.InitPlayerList();
	

}

void SharkGameScene::waitUpdate(float t)
{
	//获取服务器索引
	gameState serverState = GameInfo::GetInstance()->getserverState();
	if (m_nowState == serverState) return;
	m_nowState = serverState;
	//有消息过来了 网络是好的
	//m_bIslive = true;
	switch (m_nowState)
	{

	case gameState::STAKE:
		//播放音乐
		GameInfo::GetInstance()->selectMusicBack();
		//解开押分按钮的触摸屏蔽
		m_mainView->stakeManager.setTouch(true);
		m_mainView->battle_rootNode->setVisible(false);
		m_mainView->stake_rootNode->setVisible(true);	
		//刷新左上角信息
		m_mainView->m_playerListManager.updateInfo();
		gameStart(30);
		break;
	case gameState::BATTLE:
		//设置押分按钮的触摸屏蔽
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
//开始游戏
void SharkGameScene::openPrize(int leftTime)
{
	_STOP_MUSCI_SHARK;
	//还原金币堆
	m_mainView->stakeManager.hideGoldheap();
	log("=-------------time = %d",leftTime);
	m_mainView->sharkTimeCount.StartPlay(leftTime,2);
	scheduleOnce(schedule_selector(SharkGameScene::bridge), 0.05f);		//多停留1秒等待时间音效结束(slwh)
}

void SharkGameScene::bridge(float t)
{
	scheduleOnce(schedule_selector(SharkGameScene::gameRunning),0.05f);
}

//等待下一局游戏
void SharkGameScene::waitNextGame(int leftTime)
{

	log("----------timeCount = %d",leftTime);
	m_mainView->sharkTimeCount.StartPlay(leftTime, 2);
	//CheckNet(leftTime);
	//暂时用森林舞会的处理办法
	m_mainView->wait_rootNode->setVisible(true);
	m_mainView->waitNextManager.StartPlay(leftTime);
	
}

void SharkGameScene::gameRunning(float t)
{
	//提交游戏压分数据到服务器
	//得到结果数据
	auto gameInfo = GameInfo::GetInstance();
	int index = gameInfo->getopenRelt().resultIndex[0];
	log("----------------resultIndex = %d", index);
	//普通开奖判断
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
		//银鲨开奖判断
		for (int i = 0; i < 3; i++)
		{
			if (silverShark[i] == index)
			{
				resultIndex = 2;
				break;
			}
			
		}
		//金鲨开奖判断
		for (int i = 0; i < 3; i++)
		{
			if (goldShark[i] == index)
			{
				resultIndex = 3;				
				break;
			}
		}
		//彩鲨鱼开奖判断
		for (int i = 0; i < 2; i++)
		{
			if (color[i] == index)
			{
				resultIndex = 4;
				break;
			}
		}
	}
	
	//记录执行到哪一步了
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

//普通开奖
void SharkGameScene::generalResult(int index)
{
	auto gameInfo = GameInfo::GetInstance();
	if (1 == index)
	{
		//鱼开始游动
		m_mainView->showFishLayer();
		//统计押分
		m_mainView->stakeManager.SaveLastGame();
		m_mainView->m_playerListManager.showStakeInfo();
		scheduleOnce(schedule_selector(SharkGameScene::bridge), 0.05f);
	}
	else if (2 == index)
	{
		//播放光圈跑动音乐
		_PLAY_MUSCI_SHARK(TURN_MUSIC, true);
		int stopIndex = gameInfo->getcircleFlag();
		log("--------------stopIndex = %d----------", stopIndex);
		//设置光圈开始和停止的位置
		m_mainView->battleManager.setCirclePos(stopIndex, gameInfo->getopenRelt().resultIndex[0]);
		//游戏阶段
		m_mainView->battleManager.startPlay();
		this->scheduleOnce(schedule_selector(SharkGameScene::bridge), 10.5f);
	}
	else if (3 == index)
	{
		//清除鱼游动
		m_mainView->clearFishLayer();
		judgeResult(gameInfo->getopenRelt().resultIndex[0], false);
		//显示倍率
		showRate();
		//转到普通动物
		//m_mainView->goldManager.startPlay();
		this->scheduleOnce(schedule_selector(SharkGameScene::bridge), 4.0f);
	}
	else if (4 == index)
	{
		//光圈显示
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

//银鲨鱼开奖
void SharkGameScene::silverResult(int index)
{
	auto gameInfo = GameInfo::GetInstance();
	if (1 == index)
	{
		//鱼开始游动
		m_mainView->showFishLayer();
		//统计押分数据
		m_mainView->stakeManager.SaveLastGame();
		m_mainView->m_playerListManager.showStakeInfo();

		//显示鲨鱼旋转
		m_mainView->silverRotaManager.startPlay();
		scheduleOnce(schedule_selector(SharkGameScene::bridge), 4.0f);
	}
	else if (2 == index)
	{
		//播放光圈跑动音乐
		_PLAY_MUSCI_SHARK(TURN_MUSIC,true);
		//停止鲨鱼旋转
		m_mainView->silverRotaManager.stopPlay();
		int stopIndex = gameInfo->getcircleFlag();
		log("--------------stopIndex = %d----------", stopIndex);
		//设置光圈开始和停止的位置
		m_mainView->battleManager.setCirclePos(stopIndex, gameInfo->getopenRelt().resultIndex[0]);
		//游戏阶段
		m_mainView->battleManager.startPlay();
		this->scheduleOnce(schedule_selector(SharkGameScene::bridge), 10.5f);
	}
	else if (3 == index)
	{
		//清除鱼游动
		m_mainView->clearFishLayer();
		//转到银鲨鱼，显示幸运时刻效果
		judgeResult(gameInfo->getopenRelt().resultIndex[0], false);
		showRate();
		this->scheduleOnce(schedule_selector(SharkGameScene::bridge), 2.0f);
				
	}
	else if (4 == index)
	{
		//显示打枪效果3秒
		m_mainView->colorManager.startShoot();
		this->scheduleOnce(schedule_selector(SharkGameScene::bridge), 4.0f);
	}
	else if (5 == index)
	{
		//显示幸运时刻
		m_mainView->silverManager.startPlay();
		stopResult(5, false);
		hideRate();
		//设置回转的位置
		m_mainView->battleManager.setRotationPos(gameInfo->getopenRelt().resultIndex[1]);

		m_mainView->battleManager.rotation();
		this->scheduleOnce(schedule_selector(SharkGameScene::bridge), 1.0f);
		
	}
	else if (6 == index)
	{
		//动物跳舞
		judgeResult(gameInfo->getopenRelt().resultIndex[1], false);
		showRate();
		this->scheduleOnce(schedule_selector(SharkGameScene::bridge), 3.0f);

	}
	else if (7 == index)
	{
		hideRate();
		//关闭灯光
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
//金鲨鱼开奖
void SharkGameScene::sharkResult(int index)
{
	auto gameInfo = GameInfo::GetInstance();
	if (1 == index)
	{		
		//鱼开始游动
		m_mainView->showFishLayer();
		//统计押分数据
		m_mainView->stakeManager.SaveLastGame();
		m_mainView->m_playerListManager.showStakeInfo();

		//显示鲨鱼旋转
		m_mainView->sharkRotaManager.startPlay();
		scheduleOnce(schedule_selector(SharkGameScene::bridge), 4.0f);
	}
	else if (2 == index)
	{

		//播放光圈跑动音乐
		_PLAY_MUSCI_SHARK(TURN_MUSIC, true);
		//停止鲨鱼旋转
		m_mainView->sharkRotaManager.stopPlay();
		int stopIndex = gameInfo->getcircleFlag();
		log("--------------stopIndex = %d----------", stopIndex);
		//设置光圈开始和停止的位置
		m_mainView->battleManager.setCirclePos(stopIndex, gameInfo->getopenRelt().resultIndex[0]);
		//游戏阶段
		m_mainView->battleManager.startPlay();
		this->scheduleOnce(schedule_selector(SharkGameScene::bridge),10.5f);
	}
	else if (3 == index)
	{		
		//清除鱼游动
		m_mainView->clearFishLayer();
		//转到金鲨，显示爆灯特效
		judgeResult(gameInfo->getopenRelt().resultIndex[0], false);
		showRate();
		this->scheduleOnce(schedule_selector(SharkGameScene::bridge), 2.0f);
	}
	else if (4 == index)
	{
		//显示打枪效果3秒
		m_mainView->colorManager.startShoot();
		this->scheduleOnce(schedule_selector(SharkGameScene::bridge), 4.0f);	
		
	}
	else if (5 == index)
	{
		//显示爆灯特效
		m_mainView->goldManager.startPlay();

		stopResult(23, false);
		hideRate();
		//设置回转的位置
		m_mainView->battleManager.setRotationPos(gameInfo->getopenRelt().resultIndex[1]);
		m_mainView->battleManager.rotation();
		//打开黄灯，转到动物
		this->scheduleOnce(schedule_selector(SharkGameScene::bridge), 1.0f);
		
	}
	else if (6 == index)
	{
		//动物跳舞
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

//彩鲨鱼开奖
void SharkGameScene::colorShark(int index)
{
	auto gameInfo = GameInfo::GetInstance();
	if (index == 1)
	{
		//鱼开始游动
		m_mainView->showFishLayer();
		//押分保存
		m_mainView->stakeManager.SaveLastGame();
		m_mainView->m_playerListManager.showStakeInfo();

		//显示鲨鱼旋转
		m_mainView->sharkRotaManager.startPlay();
		scheduleOnce(schedule_selector(SharkGameScene::bridge), 2.05f);
	}
	else if (index == 2)
	{
		//播放光圈跑动音乐
		_PLAY_MUSCI_SHARK(TURN_MUSIC, true);
		//停止鲨鱼旋转
		m_mainView->sharkRotaManager.stopPlay();

		int stopIndex = gameInfo->getcircleFlag();
		log("--------------stopIndex = %d----------", stopIndex);
		//设置光圈开始和停止的位置
		m_mainView->battleManager.setCirclePos(stopIndex, gameInfo->getopenRelt().resultIndex[0]);
		//游戏阶段
		m_mainView->battleManager.startPlay();
		this->scheduleOnce(schedule_selector(SharkGameScene::bridge), 10.0f);
	}
	else if (index == 3)
	{
		//清除鱼游动
		m_mainView->clearFishLayer();
		judgeResult(gameInfo->getopenRelt().resultIndex[0], false);
		showRate();
		this->scheduleOnce(schedule_selector(SharkGameScene::bridge), 3.0f);		
	}
	else if (index == 4)
	{
		//显示打枪效果3秒
		m_mainView->colorManager.startColor();
		this->scheduleOnce(schedule_selector(SharkGameScene::bridge), 3.0f);

	}
	else if (index == 5)
	{
		//显示彩金时刻
		m_mainView->colorManager.startPlay();
		stopResult(gameInfo->getopenRelt().resultIndex[0], false);
		hideRate();
		//设置界面图片更换
		//设置回转的位置
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
		//再回转一次
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
		//换回原来的图片
		m_mainView->battleManager.reChangeImg();
		//彩金鲨界面关闭彩灯
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


//显示开奖总结界面
void SharkGameScene::showResult()
{
	m_mainView->battle_rootNode->setVisible(false);
	m_mainView->stake_rootNode->setVisible(true);
	m_mainView->result_rootNode->setVisible(true);
	//m_mainView->sharkTimeCount.changeTimeCount();

	m_mainView->recordManager.updateBoard();
	showAnimal();
}
//开奖界面最后一个环节
void SharkGameScene::battleResult()
{
	auto gameInfo = GameInfo::GetInstance();	
	//此处逻辑不严谨，若玩家得分为0，折不会有金币滚动
	SharkGameScene::m_mainView->m_playerListManager.updatePlayerList(gameInfo->getroomInfor()->vec);
	SharkGameScene::m_mainView->m_playerListManager.updateInfo(1);
	m_mainView->resultBoardManager.startPlay();
	m_mainView->m_playerListManager.changeView(1);
	//开奖界面显示
	showResult();
}

//开奖界面结束切换到押分界面
void SharkGameScene::changeView()
{
	m_mainView->resultBoardManager.stopPlay();
	m_mainView->m_playerListManager.deleteOwnStake();
	m_mainView->result_rootNode->setVisible(false);
	m_nowIndex = _ZERO;

	stopResAnimal();
}

//判断开奖界面显示哪一个动物
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

//消除开奖界面的动物
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

//判断是哪一个开奖
void SharkGameScene::judgeResult(int index,bool colorsharFlag)
{
	
	auto gameInfo = GameInfo::GetInstance();	
	int* rate;
	rate = gameInfo->getRate();
	
	if (index == 0 || index == 1)
	{
		//熊猫
		//设置倍率显示
		m_mainView->rateManager.setBeiLv(rate[9]);
		m_mainView->animalManager.startAnimal(1, ANIMAL_PANDA, PATH_PANDA);
		_PLAY_SOUND_SHARK(PANDA_MUSIC);

		//显示背景图片
		m_mainView->animalBgManager.startPlay();
	
	}
	else if (index == 3 || index == 4)
	{
		//猴子
		//设置倍率显示
		m_mainView->rateManager.setBeiLv(rate[10]);
		m_mainView->animalManager.startAnimal(1, ANIMAL_MONKEY, PATH_MONKEY);
		_PLAY_SOUND_SHARK(MONKEY_MUSIC);

		//显示背景图片
		m_mainView->animalBgManager.startPlay();
	}
	
	else if (index == 6 || index == 7 || index == 8)
	{		
		//兔子
		//设置倍率显示
		m_mainView->rateManager.setBeiLv(rate[11]);
		m_mainView->animalManager.startAnimal(1, ANIMAL_RABBIT, PATH_RABBIT);
		_PLAY_SOUND_SHARK(RABBIT_MUSIC);

		//显示背景图片
		m_mainView->animalBgManager.startPlay();
	}
	else if (index == 10 || index == 11 || index == 12)
	{		
		//燕子
		//设置倍率显示
		m_mainView->rateManager.setBeiLv(rate[4]);
		m_mainView->animalManager.startAnimal(1, ANIMAL_SWALLOW,PATH_SWALLOW);
		_PLAY_SOUND_SHARK(SWALLOW_MUSIC);

		//显示背景图片
		m_mainView->animalBgManager.startPlay();
	}
	else if (index == 14 || index == 15)
	{
		//鸽子
		//设置倍率显示
		m_mainView->rateManager.setBeiLv(rate[5]);
		m_mainView->animalManager.startAnimal(1, ANIMAL_DOVE, PATH_DOVE);
		_PLAY_SOUND_SHARK(DOVE_MUSIC);

		//显示背景图片
		m_mainView->animalBgManager.startPlay();
	}
	else if (index == 17 || index == 18)
	{		
		//孔雀
		//设置倍率显示
		m_mainView->rateManager.setBeiLv(rate[6]);
		m_mainView->animalManager.startAnimal(1, ANIMAL_PEACOCK, PATH_PEACOCK);
		_PLAY_SOUND_SHARK(PEACOCK_MUSIC);

		//显示背景图片
		m_mainView->animalBgManager.startPlay();
	}
	else if (index == 20 || index == 21 || index == 22)
	{
		//老鹰
		//设置倍率显示
		m_mainView->rateManager.setBeiLv(rate[7]);
		m_mainView->animalManager.startAnimal(1, ANIMAL_HAWK, PATH_HAWK);
		_PLAY_SOUND_SHARK(EAGLE_MUSIC);

		//显示背景图片
		m_mainView->animalBgManager.startPlay();
	}
	else if (index == 24 || index == 25 || index == 26)
	{		
		//狮子
		//设置倍率显示
		m_mainView->rateManager.setBeiLv(rate[8]);
		m_mainView->animalManager.startAnimal(1, ANIMAL_LION, PATH_LION);
		_PLAY_SOUND_SHARK(LION_MUSIC);

		//显示背景图片
		m_mainView->animalBgManager.startPlay();
	}
	else
	{
		if (!colorsharFlag)
		{
			//金鲨开奖
			if (index == 2 || index == 9 || index == 16)
			{
				//金鲨鱼
				//设置倍率显示
				m_mainView->rateManager.setBeiLv(rate[1]);
				m_mainView->animalManager.startAnimal(1, ANIMAL_GOLD, PATH_GOLD);
				_PLAY_SOUND_SHARK(GOLDSHARK_MUSIC);
			}
			//银鲨鱼开奖
			else if (index == 13 || index == 19 || index == 27)
			{
				//银鲨
				//设置倍率显示
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
				//1元宝  2船票  3彩金
				m_mainView->colorManager.judgeResult(3); 				
				hideRate();
			}
			else
			{
				if (GameInfo::GetInstance()->getopenRelt().aType == 1)
				{
					m_mainView->colorManager.judgeResult(1);

					//1 元宝  2船票
					int rate = countNum(1);
					m_mainView->rateManager.setBeiLv(rate);
				}
				else if (GameInfo::GetInstance()->getopenRelt().aType == 2)
				{
					if (index == 2 || index == 16)
					{
						m_mainView->colorManager.judgeResult(2);
						//1 元宝  2船票
						int rate = countNum(2);
						m_mainView->rateManager.setBeiLv(rate);
					}
					else
					{
						m_mainView->colorManager.judgeResult(1);
						//1 元宝  2船票
						int rate = countNum(1);
						m_mainView->rateManager.setBeiLv(rate);
					}

				}
			}
			
		}
	}
	
}

//计算获得的元宝或者船票的个数
int  SharkGameScene::countNum(int index)
{
	auto gameInfo = GameInfo::GetInstance();
	int achiveNum = 0;
	int score = gameInfo->getownInfo().ownTotal; //获取押注的所有分数

	//100 是元宝的换算单位，1000是船票的换算单位
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

//停止战斗界面的动物显示
void SharkGameScene::stopResult(int index,bool colorsharFlag)
{	
	if (index == 0 || index == 1)
	{
		m_mainView->animalManager.stopAnimal(ANIMAL_PANDA);
		//隐藏背景图片
		m_mainView->animalBgManager.stopPlay();
	}
	else if (index == 3 || index == 4)
	{
		m_mainView->animalManager.stopAnimal(ANIMAL_MONKEY);
		//隐藏背景图片
		m_mainView->animalBgManager.stopPlay();
	}
	else if (index == 6 || index == 7 || index == 8)
	{
		m_mainView->animalManager.stopAnimal(ANIMAL_RABBIT);
		//隐藏背景图片
		m_mainView->animalBgManager.stopPlay();
	}
	else if (index == 10 || index == 11 || index == 12)
	{
		m_mainView->animalManager.stopAnimal(ANIMAL_SWALLOW);
		//隐藏背景图片
		m_mainView->animalBgManager.stopPlay();
	}
	else if (index == 14 || index == 15)
	{
		m_mainView->animalManager.stopAnimal(ANIMAL_DOVE);
		//隐藏背景图片
		m_mainView->animalBgManager.stopPlay();
	}
	else if (index == 17 || index == 18)
	{
		m_mainView->animalManager.stopAnimal(ANIMAL_PEACOCK);
		//隐藏背景图片
		m_mainView->animalBgManager.stopPlay();
	}
	else if (index == 20 || index == 21 || index == 22)
	{
		m_mainView->animalManager.stopAnimal(ANIMAL_HAWK);
		//隐藏背景图片
		m_mainView->animalBgManager.stopPlay();
	}
	else if (index == 24 || index == 25 || index == 26)
	{
		m_mainView->animalManager.stopAnimal(ANIMAL_LION);
		//隐藏背景图片
		m_mainView->animalBgManager.stopPlay();
	}
	

	else
	{
		if (!colorsharFlag)
		{
			//金鲨开奖
			if (index == 2 || index == 9 || index == 16)
			{
				//金鲨鱼
				m_mainView->animalManager.stopAnimal(ANIMAL_GOLD);
			}
			//银鲨鱼开奖
			else if (index == 13 || index == 19 || index == 27)
			{
				//银鲨
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


//显示获取的倍率
void SharkGameScene::showRate()
{	
	m_mainView->rateManager.startPlay();
}

//隐藏倍率显示的界面
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
	//检查是否有回应
	CheckNet(_ZERO);
}

void SharkGameScene::OnHreatPak()
{
	//收到心跳包 是在线的
	m_bIslive = true;
}

