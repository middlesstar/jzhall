#include "PlayerListManager.h"
#include "shark/NetWork/SharkNetworkKernel.h"
#include "ui/UILayout.h"


#define _WIN_BK_0						"win_bk_0"
#define _WIN_NUM_0						"win_num_0"
#define _BET_GOLD						"bet_gold"

#define	_HEAD							"head"
#define	_HEAD_BK						"Image_frame"
#define _NAME							"text_name"
#define	_VIP_0							"image_vip0"
#define	_VIP_FRONT						"vip_front"
#define _VIP_NUM						"vip_degree"
#define _SCORE							"text_score"
#define	_ONLINE_NUM						"onlineNum"
#define	_TOTLE_USER_NUM					"totalbetBum"

#define _LIST_VIEW						"ListView"
#define _LIST_VIEW_TEMPLATE				"bk_content"

#define _TAG_ADD						10000


#define _Head_Path						"images/shak/head/head_%d.png"
#define _Head_Path_Frame				"images/shak/vip/frame%d.png"


#define _Bet_Win						"images/shak/bet/bet_win.png"
#define _Bet_Lose						"images/shak/bet/bet_lost.png"
#define _Bet_Points						"images/shak/bet/bet_points.png"

//玩家押分的列表
#define  PLAYERINFO                      "playerInfo"
#define  STAKENUM                        "totalbetNum"
#define  STAKEINFO                       "stakeInfo"
#define  STAKELIST                       "listView"
#define  STAKELISTITEM                   "item"


#define Hall_Message_Refresh_VIP			"Hall_Message_Refresh_VIP"			//刷新vip显示
#define Hall_Message_Refresh_Coin			"Hall_Message_Refresh_Coin"			//刷新金币显示

PlayerListManager::PlayerListManager() :
m_head(nullptr),
m_headBk(nullptr),
m_name(nullptr),
m_vip0(nullptr),
m_vip(nullptr),
m_vipNum(nullptr),
m_scoreNum(nullptr),
m_winBk(nullptr),
m_winNum(nullptr),
m_betgold(nullptr),
m_playerListView(nullptr),
playerInfo(nullptr),
stakeInfo(nullptr),
totalStake(nullptr),
stakeListView(nullptr),
m_targetGold(_ZERO),
countStake(_ZERO)
{
	HALL_NOTIFICATION_REG(Hall_Message_Refresh_Coin, PlayerListManager::refreshPlayerCoin, nullptr);
	HALL_NOTIFICATION_REG(Hall_Message_Refresh_VIP, PlayerListManager::refreshPlayerVIP, nullptr);
}
PlayerListManager::~PlayerListManager()
{
	HALL_NOTIFICATION_UNREG(Hall_Message_Refresh_Coin);
	HALL_NOTIFICATION_UNREG(Hall_Message_Refresh_VIP);

	//清除存储的每个按钮的押分信息
	auto gameInfo = GameInfo::GetInstance();
	if (gameInfo->getserverState() == BATTLE)
	{
		deleteOwnStake();
	}
}

//初始化
void PlayerListManager::setContent()
{

	m_winBk = dynamic_cast<ImageView*>(m_pNode->getChildByName(_WIN_BK_0));
	CC_ASSERT(m_winBk != nullptr);

	m_betgold = dynamic_cast<Sprite*>(m_pNode->getChildByName(_BET_GOLD));
	CC_ASSERT(m_betgold != nullptr);

	m_winNum = dynamic_cast<Text*>(m_pNode->getChildByName(_WIN_NUM_0));
	CC_ASSERT(m_winNum != nullptr);
	//玩家列表
	m_head = dynamic_cast<Sprite*>(m_pNode->getChildByName(_HEAD));
	CC_ASSERT(m_head != nullptr);

	m_headBk = dynamic_cast<ImageView*>(m_pNode->getChildByName(_HEAD_BK));
	CC_ASSERT(m_headBk != nullptr);

	m_name = dynamic_cast<Text*>(m_pNode->getChildByName(_NAME));
	CC_ASSERT(m_name != nullptr);

	m_vip0 = dynamic_cast<ImageView*>(m_pNode->getChildByName(_VIP_0));
	CC_ASSERT(m_vip0 != nullptr);

	m_vip = dynamic_cast<ImageView*>(m_pNode->getChildByName(_VIP_FRONT));
	CC_ASSERT(m_vip != nullptr);

	m_vipNum = dynamic_cast<TextAtlas*>(m_pNode->getChildByName(_VIP_NUM));
	CC_ASSERT(m_vipNum != nullptr);

	m_scoreNum = dynamic_cast<Text*>(m_pNode->getChildByName(_SCORE));
	CC_ASSERT(m_scoreNum != nullptr);

	m_onlineNum = dynamic_cast<Text*>(_seekNodeByName(m_pNode, _ONLINE_NUM));
	CC_ASSERT(m_onlineNum != nullptr);

	m_totelJetNum = dynamic_cast<Text*>(_seekNodeByName(m_pNode, _TOTLE_USER_NUM));
	CC_ASSERT(m_totelJetNum != nullptr);

	m_playerListView = dynamic_cast<ListView*>(_seekNodeByName(m_pNode, _LIST_VIEW));
	CC_ASSERT(m_playerListView != nullptr);

	PlayerListOne::initTemplate(dynamic_cast<ImageView*>(_seekNodeByName(m_pNode,_LIST_VIEW_TEMPLATE)));

	
	//玩家信息节点
	playerInfo = dynamic_cast<Node*>(_seekNodeByName(m_pNode,PLAYERINFO));
	CC_ASSERT(playerInfo != nullptr);
	playerInfo->setVisible(false);

	stakeInfo = dynamic_cast<Node*>(_seekNodeByName(m_pNode, STAKEINFO));
	CC_ASSERT(stakeInfo != nullptr);
	stakeInfo->setVisible(false);

	//用户押分列表
	totalStake = dynamic_cast<Text*>(_seekNodeByName(m_pNode, STAKENUM));
	CC_ASSERT(totalStake != nullptr);

	stakeListView = dynamic_cast<ListView*>(_seekNodeByName(m_pNode, STAKELIST));
	CC_ASSERT(stakeListView != nullptr);

	StakeItem::initTemplate(dynamic_cast<ImageView*>(_seekNodeByName(m_pNode, STAKELISTITEM)));
	

	setOnlineNum(_ZERO);
	setTotelNum(_ZERO);
	setTotalStake(_ZERO);
	setWinScore(_ZERO, STAKE);
}


//清空上一局的总押分信息
void PlayerListManager::cancelTotleNum()
{
	//更新总压分
	SCORE s = GameInfo::GetInstance()->getroomInfor()->alljetinfo.lAllBet;
	setTotelNum(s);
}


void PlayerListManager::setWinScore(SCORE score, gameState type)
{
	char num[30];
	if (type == STAKE)
	{
		m_winBk->loadTexture(_Bet_Points, TextureResType::PLIST);
	}
	else
	{		
		if (score >= _ZERO)
		{
			//赢
			m_winBk->loadTexture(_Bet_Win, TextureResType::PLIST);

		}
		else
		{
			//输
			m_winBk->loadTexture(_Bet_Lose, TextureResType::PLIST);
		}
	}	
	
	
	SCORE userscore = abs(score);
	if (userscore > 1000)
	{
		auto gameInfo = GameInfo::GetInstance();
		int roomNum = gameInfo->getroomIndex();
		if (roomNum == 2)
		{
			sprintf(num, "%ldk", userscore / 1000);
		}
		else if (roomNum == 1)
		{
			sprintf(num, "%.1fK", 1.f * userscore / 1000);
		}
		else if (roomNum == 0)
		{
			sprintf(num, "%.2fK", 1.f * userscore / 1000);
		}
		
	}
	else
	{
		sprintf(num, "%ld", userscore);
	}
	m_winNum->setString(num);

}

//初始化玩家列表
void PlayerListManager::InitPlayerList()
{
	auto roominfo = GameInfo::GetInstance()->getroomInfor();
	////进行排序 vip等级高在最上面  vip一样 比目前金币数
	std::sort(roominfo->vec.begin(), roominfo->vec.end(), UserInfo_Shark::LessComp);


	m_playerListView->removeAllChildren();
	for (std::vector<PlayerListOne*>::iterator itor = m_playerlistinfo.begin(); itor != m_playerlistinfo.end(); itor++)
	{
		delete (*itor);
	}
	m_playerlistinfo.clear();

	for (std::vector<UserInfo_Shark>::iterator itor = roominfo->vec.begin(); itor != roominfo->vec.end(); itor++)
	{
		addPlayerElement(*itor);
	}

	int online = m_playerlistinfo.size();
	setOnlineNum(online);


}
//转换列表视图
void PlayerListManager::changeView(int index)
{
	if (index == 1)
	{
		playerInfo->setVisible(true);
		stakeInfo->setVisible(false);

	}
	else if (index == 2)
	{
		playerInfo->setVisible(false);
		stakeInfo->setVisible(true);
	}

}
//添加玩家
void PlayerListManager::addPlayerElement(const UserInfo_Shark &info)
{
	if (nullptr == m_playerListView)
	{
		return;
	}

	Vector<Node*> vec = m_playerListView->getChildren();
	for (Vector<Node*>::iterator itor = vec.begin(); itor != vec.end(); itor++)
	{
		if (info.userid + _TAG_ADD == (*itor)->getTag())
		{
			return;
		}
	}

	PlayerListOne* user =  PlayerListOne::create();
	if (user == nullptr) return;

	m_playerlistinfo.push_back(user);
	user->setuserinfo(info);
	user->getRootNdoe()->setTag(_TAG_ADD + info.userid);
	m_playerListView->pushBackCustomItem(user->getRootNdoe());


	int online = m_playerlistinfo.size();
	setOnlineNum(online);

}

//更新玩家列表
void PlayerListManager::updatePlayerList(std::vector<UserInfo_Shark> vec)
{
	//进行排序
	std::sort(vec.begin(), vec.end(), UserInfo_Shark::GreatComp);


	auto roominfo = GameInfo::GetInstance()->getroomInfor();
	m_playerListView->removeAllChildren();
	for (std::vector<PlayerListOne*>::iterator itor = m_playerlistinfo.begin(); itor != m_playerlistinfo.end(); itor++)
	{
		delete (*itor);
	}
	m_playerlistinfo.clear();
	roominfo->vec.clear();

	int i = 0;
	for (std::vector<UserInfo_Shark>::iterator itor = vec.begin(); itor != vec.end(); itor++)
	{
		if (i < 10)
		{
			if ((*itor).userid == Player::getInstance()->getLogonData().dwUserID)
			{
				GameInfo::GetInstance()->settophead(true);				
			}
			i++;
		}
		addPlayerElement(*itor);

		roominfo->vec.push_back(*itor);
		if ((*itor).userid == Player::getInstance()->getLogonData().dwUserID)
		{
			setWinScore((*itor).winscore, BATTLE);
		}
	}


	int online = m_playerlistinfo.size();
	setOnlineNum(online);
	
}


//删除玩家
void PlayerListManager::deleteUserElement(int userid)
{
	m_playerListView->removeChildByTag(_TAG_ADD + userid, true);

	for (std::vector<PlayerListOne*>::iterator itor = m_playerlistinfo.begin(); itor != m_playerlistinfo.end(); itor++)
	{
		if ((*itor)->getuserinfo().userid == userid)
		{
			delete (*itor);
			m_playerlistinfo.erase(itor);
			break;
		}
	}
	//更新在线玩家数
	auto roomInfo = GameInfo::GetInstance()->getroomInfor();
	int online = m_playerlistinfo.size();
	setOnlineNum(online);
}



//其他玩家压分操作
void PlayerListManager::otherJet(int userid, long jetadd, SCORE nowscore, dword animalindex)
{

	auto roominfo = GameInfo::GetInstance()->getroomInfor();
	for (std::vector<UserInfo_Shark>::iterator itor = roominfo->vec.begin(); itor != roominfo->vec.end(); itor++)
	{
		if (itor->userid == userid)
		{
			itor->gold = nowscore;
			itor->jet += jetadd;
		}
	}



	for (std::vector<PlayerListOne*>::iterator itor = m_playerlistinfo.begin(); itor != m_playerlistinfo.end(); itor++)
	{
		UserInfo_Shark info = (*itor)->getuserinfo();
		if (info.userid == userid)
		{
			info.gold = nowscore;
			info.jet += jetadd;
			(*itor)->setuserinfo(info);
			//tosl

			//金币飞过去(自己不飞)
			if (info.userid != Player::getInstance()->getLogonData().dwUserID)
			{
				(*itor)->GoldFly(animalindex);
			}			

			if (Player::getInstance()->getLogonData().dwUserID == userid)
			{
				//更新列表上边金币界面
				setScore(nowscore);
				setWinScore(info.jet, STAKE);
				log("--------------info.jet = %ld--------------",info.jet);
			}
			break;
		}
	}

	//更新总压分
	SCORE s = GameInfo::GetInstance()->getroomInfor()->alljetinfo.lAllBet;
	setTotelNum(s);
	
}



//玩家取消压分
void PlayerListManager::othercancelJet(int userid, SCORE jetadd)
{
	for (std::vector<PlayerListOne*>::iterator itor = m_playerlistinfo.begin(); itor != m_playerlistinfo.end(); itor++)
	{
		UserInfo_Shark info = (*itor)->getuserinfo();
		if (info.userid == userid)
		{
			info.gold += jetadd;
			info.jet = _ZERO;
			//(*itor)->setuserinfo(info);

			if (info.userid == Player::getInstance()->getLogonData().dwUserID)
			{
				setWinScore(_ZERO, STAKE);
			}
			(*itor)->setuserinfo(info);

			//如果是自己
			if (Player::getInstance()->getLogonData().dwUserID == userid)
			{
				//更新列表上边金币界面
				setScore(info.gold);
				setWinScore(info.jet, STAKE);
			}
			break;
		}
	}

	//更新总压分
	SCORE s = GameInfo::GetInstance()->getroomInfor()->alljetinfo.lAllBet;
	setTotelNum(s);

}


void PlayerListManager::setOnlineNum(int i)
{
	std::stringstream str;
	str << i;
	m_onlineNum->setString(str.str());
}

void PlayerListManager::setTotelNum(SCORE score)
{
	char buf[256];
	if (score >= 1000)
	{
		sprintf(buf, "%.2fK", 1.f * score / 1000);
	}
	else
	{
		sprintf(buf, "%ld", score);
	}
	m_totelJetNum->setString(buf);
}

void PlayerListManager::setName(const std::string &name)
{
	m_name->setText(name);
}



void PlayerListManager::updateInfo(int code)
{
	//找到自己
	int userid = Player::getInstance()->getLogonData().dwUserID;
	for (std::vector<PlayerListOne*>::iterator itor = m_playerlistinfo.begin(); itor != m_playerlistinfo.end(); itor++)
	{
		UserInfo_Shark info = (*itor)->getuserinfo();
		if (info.userid == userid)
		{
			setHead(info.headimgid);
			setVipOrHradBk(info.viplevel);

			if (code != _ZERO)
			{
				//金币滚动
				goldRollPlay(info.gold);
			}
			else
			{
				//金币直接刷上
				setScore(info.gold);
			}

			//setScore(info.gold);
			setName(info.nickname);
			(*itor)->setuserinfo(info);

			if (GameInfo::GetInstance()->getserverState() == STAKE)
			{
				setWinScore(info.jet, GameInfo::GetInstance()->getserverState());
			}
			else
			{
				setWinScore(info.winscore, GameInfo::GetInstance()->getserverState());
			}			

			auto scoreData = Player::getInstance()->getLogonData();
			scoreData.lUserScore = info.gold;
			Player::getInstance()->setLogonData(scoreData);
			break;
		}
	}
}


void PlayerListManager::goldRollPlay(SCORE targetnum)
{
	//计算差
	SCORE nowNum;
	std::stringstream str(m_scoreNum->getString());
	str >> nowNum;
	m_targetGold = targetnum;
	m_getnum = targetnum - nowNum;
	if (m_getnum > _ZERO)
	{
		scheduleOnce(schedule_selector(PlayerListManager::goldRolldelay), 3.0f);
	}
}

void PlayerListManager::goldRolldelay(float  t)
{
	schedule(schedule_selector(PlayerListManager::goldRollfunc), 0.1f);
}

void PlayerListManager::goldRollfunc(float t)
{
	//几次滚动完成
	static int index = _ZERO;
	if (index < 30)		//持续3秒
	{
		setScore((SCORE)(m_targetGold - m_getnum + ((1 + index) * m_getnum) / 30));
		m_betgold->runAction(
			Sequence::create(
			ScaleTo::create(0.05f, 1.2f),
			ScaleTo::create(0.05f, 1.f),
			NULL));
		index++;
	}
	else
	{
		setScore(m_targetGold);
		index = _ZERO;
		unschedule(schedule_selector(PlayerListManager::goldRollfunc));
	}

}

void PlayerListManager::setHead(int index)
{
	char path[256];
	sprintf(path, _Head_Path, index);
	m_head->setTexture(path);
}

void PlayerListManager::setVipOrHradBk(int vip)
{
	if (vip == _ZERO)
	{
		m_vip0->setVisible(true);
		m_vip->setVisible(false);
		m_vipNum->setVisible(false);
	}
	else
	{
		m_vip0->setVisible(false);
		m_vip->setVisible(true);
		m_vipNum->setVisible(true);
		std::stringstream str;
		str << vip;
		m_vipNum->setString(str.str());
	}

	//VIP框子图片不够10张 暂且这么设定
	int vipFrame = vip;
	if (vipFrame >= 7)
	{
		vipFrame = 7;
	}

	//加载炫酷头像框
	char path[256];
	sprintf(path, _Head_Path_Frame, vipFrame);
	m_headBk->loadTexture(path, TextureResType::PLIST);

}

void PlayerListManager::setScore(SCORE score)
{
	std::stringstream str;
	str << score;
	m_scoreNum->setString(str.str());
}

//显示押分信息
void PlayerListManager::showStakeInfo()
{
	
	auto gameInfo = GameInfo::GetInstance();

	int* rate;
	rate = gameInfo->getRate();
	if (nullptr == stakeListView)
	{
	return;
	}

	setTotalStake(gameInfo->getownInfo().ownTotal);	
	
	for (int i = 0; i < 12; i++)
	{
		StakeItem* stake = StakeItem::create();
		if (stake == nullptr) return;
		stakeListInfo.push_back(stake);
		
		if (gameInfo->getownInfo().ownStake[i] != 0)
		{
			stake->setLogo(i);
			stake->setRate(rate[i]);
			stake->setStakeNum(gameInfo->getownInfo().ownStake[i]);
			//stakeListView->addChild(stake->getRootNdoe());
			stakeListView->pushBackCustomItem(stake->getRootNdoe());
			countStake++;
		}
	
	}
}

//删除自己的押分列表信息
void PlayerListManager::deleteOwnStake()
{
	stakeListView->removeAllChildren();
	for (std::vector<StakeItem*>::iterator itor = stakeListInfo.begin(); itor != stakeListInfo.end(); )
	{
		delete (*itor);
		itor = stakeListInfo.erase(itor);
	}
	stakeListInfo.clear();
	countStake = 0;
	setTotalStake(_ZERO);
}

//设置总押分数
void PlayerListManager::setTotalStake(int num)
{
	char buf[256];
	sprintf(buf, "%d",num);
	totalStake->setString(buf);
}


//刷新用户的分数
void PlayerListManager::refreshPlayerCoin(Ref* ref)
{
	//改变存储在刷新列表的信息
	auto roomInfo = GameInfo::GetInstance()->getroomInfor();
	for (std::vector<UserInfo_Shark>::iterator itor = roomInfo->vec.begin(); itor != roomInfo->vec.end(); itor++)
	{
		if ((*itor).userid == Player::getInstance()->getLogonData().dwUserID)
		{
			(*itor).gold = Player::getInstance()->getLogonData().lUserScore;
			setScore((*itor).gold);
			break;
		}
	}

	for (std::vector<PlayerListOne*>::iterator itor = m_playerlistinfo.begin(); itor != m_playerlistinfo.end(); itor++)
	{
		UserInfo_Shark info = (*itor)->getuserinfo();
		if (info.userid == Player::getInstance()->getLogonData().dwUserID)
		{			
			info.gold = Player::getInstance()->getLogonData().lUserScore;
			/*-------此处更新金币的多少------*/
			auto gameInfo = GameInfo::GetInstance();
			int coinScore = gameInfo->getchangeScore();
			//int yuanBaoScore = gameinfo->getchangeYuanBao();
			coinScore = info.gold;
			gameInfo->setchangeScore(coinScore);
			/*----------------------------*/
			
			(*itor)->setuserinfo(info);
			break;
		}
	}
	//updatePlayerList(gameInfo->getroomInfor()->vec);    //用户金币数排序
}
//刷新vip等级

void PlayerListManager::refreshPlayerVIP(Ref* ref)
{
	//改变存储在刷新列表里边的信息
	auto roomInfo = GameInfo::GetInstance()->getroomInfor();
	for (std::vector<UserInfo_Shark>::iterator itor = roomInfo->vec.begin(); itor != roomInfo->vec.end(); itor++)
	{
		if ((*itor).userid == Player::getInstance()->getLogonData().dwUserID)
		{
			(*itor).viplevel = Player::getInstance()->getHallData().dwVipLevel;
			setVipOrHradBk((*itor).viplevel);
			break;
		}
	}

	for (std::vector<PlayerListOne*>::iterator itor = m_playerlistinfo.begin(); itor != m_playerlistinfo.end(); itor++)
	{
		UserInfo_Shark info = (*itor)->getuserinfo();
		if (info.userid == Player::getInstance()->getLogonData().dwUserID)
		{			
			info.viplevel = Player::getInstance()->getHallData().dwVipLevel;			
			(*itor)->setuserinfo(info);
			break;
		}
	}
	//InitPlayerList();  //vip等级排序
}
