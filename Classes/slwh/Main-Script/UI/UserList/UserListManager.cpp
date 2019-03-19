#include "UserListManager.h"
#include "slwh/Main-Script/UI/UIUserInfo.h"
#include "slwh/Network/NetworkKernel.h"


#define	_HEAD							"head"
#define	_HEAD_BK						"Image_frame"
#define _NAME							"text_name"
#define	_VIP_0							"image_vip0"
#define	_VIP_FRONT						"vip_front"
#define _VIP_NUM						"vip_degree"
#define _SCORE							"text_score"
#define	_ONLINE_NUM						"onlineNum"
#define	_TOTLE_USER_NUM					"totalbetBum"
#define _WIN_BK_0						"win_bk_0"
#define _WIN_NUM_0						"win_num_0"
#define _BET_GOLD						"bet_gold"

#define _LIST_VIEW						"ListView"
#define _LIST_VIEW_TEMPLATE				"bk_content"

#define _TAG_ADD						10000

#define _Head_Path						"images/shak/head/head_%d.png"
#define _Head_Path_Frame				"images/shak/vip/frame%d.png"

#define _Bet_Win						"game/game_scene/bet_win.png"
#define _Bet_Lose						"game/game_scene/bet_lost.png"
#define _Bet_Points						"game/game_scene/bet_points.png"

#define Hall_Message_Refresh_VIP		"Hall_Message_Refresh_VIP"			//刷新vip显示
#define Hall_Message_Refresh_Coin		"Hall_Message_Refresh_Coin"			//刷新金币显示

UserListManager::UserListManager():
m_head(nullptr),
m_headBk(nullptr),
m_name(nullptr),
m_vip0(nullptr),
m_vip(nullptr),
m_vipNum(nullptr),
m_scoreNum(nullptr),
m_userListView(nullptr),
m_winBk(nullptr),
m_winNum(nullptr),
m_betgold(nullptr),
m_actiontime(_ZERO),
m_targetGold(_ZERO)
{
	HALL_NOTIFICATION_REG(Hall_Message_Refresh_Coin, UserListManager::refreshPlayerCoin, nullptr);
	HALL_NOTIFICATION_REG(Hall_Message_Refresh_VIP, UserListManager::refreshPlayerVIP, nullptr);
}

UserListManager::~UserListManager()
{
	HALL_NOTIFICATION_UNREG(Hall_Message_Refresh_Coin);
	HALL_NOTIFICATION_UNREG(Hall_Message_Refresh_VIP);
	setTotelNum(_ZERO);
}

//初始化
void UserListManager::Start()
{
	scheduleUpdate();

	m_winBk = dynamic_cast<ImageView*>(m_node->getChildByName(_WIN_BK_0));
	CC_ASSERT(m_winBk != nullptr);

	m_betgold = dynamic_cast<Sprite*>(m_node->getChildByName(_BET_GOLD));
	CC_ASSERT(m_betgold != nullptr);

	m_winNum = dynamic_cast<Text*>(m_node->getChildByName(_WIN_NUM_0));
	CC_ASSERT(m_winNum != nullptr);

	m_head = dynamic_cast<Sprite*>(m_node->getChildByName(_HEAD));
	CC_ASSERT(m_head != nullptr);

	m_headBk = dynamic_cast<ImageView*>(m_node->getChildByName(_HEAD_BK));
	CC_ASSERT(m_headBk != nullptr);

	m_name = dynamic_cast<Text*>(m_node->getChildByName(_NAME));
	CC_ASSERT(m_name != nullptr);

	m_vip0 = dynamic_cast<ImageView*>(m_node->getChildByName(_VIP_0));
	CC_ASSERT(m_vip0 != nullptr);

	m_vip = dynamic_cast<ImageView*>(m_node->getChildByName(_VIP_FRONT));
	CC_ASSERT(m_vip != nullptr);

	m_vipNum = dynamic_cast<TextAtlas*>(m_node->getChildByName(_VIP_NUM));
	CC_ASSERT(m_vipNum != nullptr);

	m_scoreNum = dynamic_cast<TextAtlas*>(m_node->getChildByName(_SCORE));
	CC_ASSERT(m_scoreNum != nullptr);

	m_onlineNum = dynamic_cast<Text*>(m_node->getChildByName(_ONLINE_NUM));
	CC_ASSERT(m_onlineNum != nullptr);

	m_totelJetNum = dynamic_cast<Text*>(m_node->getChildByName(_TOTLE_USER_NUM));
	CC_ASSERT(m_totelJetNum != nullptr);

	m_userListView = dynamic_cast<ListView*>(m_node->getChildByName(_LIST_VIEW));
	CC_ASSERT(m_userListView != nullptr);



	UserListOne::initTemplate(dynamic_cast<ImageView*>(m_node->getChildByName(_LIST_VIEW_TEMPLATE)));

	setOnlineNum(_ZERO);
	setTotelNum(_ZERO);
	setWinScore(_ZERO,false);
}

void UserListManager::setWinScore(SCORE score, bool biscount)
{
	if (biscount == false)
	{
		m_winBk->loadTexture(_Bet_Points, TextureResType::PLIST);
	}
	else{
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

	char num[30];
	SCORE userscore = abs(score);
	if (userscore > 1000)
	{
		slwhRoomInfo* info = OneGameInfo::GetInstance()->getroomInfo();
		switch (info->wType)
		{
		case 0:
			sprintf(num, "%.2fK", 1.f * userscore / 1000);
			break;
		case 1:
			sprintf(num, "%.1fK", 1.f * userscore / 1000);
			break;
		case 2:
			sprintf(num, "%.0fK", 1.f * userscore / 1000);
			break;
		default:
			break;
		}
	}
	else
	{
		sprintf(num, "%lld", userscore);
	}
	m_winNum->setString(num);
}

//添加玩家
void UserListManager::addUserElement(const slwhPlayerInfo &info)
{
	if (nullptr == m_userListView)
	{
		return;
	}

	Vector<Node*> vec = m_userListView->getChildren();
	for (Vector<Node*>::iterator itor = vec.begin(); itor != vec.end(); itor++)
	{
		if (info.userid + _TAG_ADD == (*itor)->getTag())
		{
			//已经有该玩家了
			return;
		}
	}

	UserListOne* user = UserListOne::create();
	if (user == nullptr) return;

	m_userlistinfo.push_back(user);
	user->setuserinfo(info);
	user->getRootNdoe()->setTag(_TAG_ADD + info.userid);
	m_userListView->pushBackCustomItem(user->getRootNdoe());
	//m_userListView->addChild(user->getRootNdoe());
	//更新在线玩家数
	//int online = OneGameInfo::GetInstance()->getroomInfo()->dwOnline;
	int online = m_userlistinfo.size();
	setOnlineNum(online);

	//OneGameInfo::GetInstance()->getroomInfo()->vec.push_back(info);
}

void UserListManager::update(float t)
{
	//log("-------dais: %.2f , %.2f -------", m_userListView->getInnerContainerPosition().x, m_userListView->getInnerContainerPosition().y);

}

//删除玩家
void UserListManager::deleteUserElement(int userid)
{
	m_userListView->removeChildByTag(_TAG_ADD + userid, true);

	for (std::vector<UserListOne*>::iterator itor = m_userlistinfo.begin(); itor != m_userlistinfo.end(); itor++)
	{
		if ((*itor)->getuserinfo().userid == userid)
		{
			delete (*itor);
			m_userlistinfo.erase(itor);
			break;
		}
	}
	

	//更新在线玩家数
	auto roominfo = OneGameInfo::GetInstance()->getroomInfo();
	int online = m_userlistinfo.size();
	setOnlineNum(online);


}

//初始化玩家列表
void UserListManager::InitUserList()
{
	auto roominfo = OneGameInfo::GetInstance()->getroomInfo();
	
	//进行排序 vip等级高在最上面  vip一样 比目前金币数
	std::sort(roominfo->vec.begin(), roominfo->vec.end(), slwhPlayerInfo::LessComp);

	m_userListView->removeAllChildren();
	for (std::vector<UserListOne*>::iterator itor = m_userlistinfo.begin(); itor != m_userlistinfo.end(); itor++)
	{
		delete (*itor);
	}
	m_userlistinfo.clear();

	for (std::vector<slwhPlayerInfo>::iterator itor = roominfo->vec.begin(); itor != roominfo->vec.end(); itor++)
	{
		addUserElement(*itor);
	}

	//更新在线玩家数
	int online = m_userlistinfo.size();
	setOnlineNum(online);

}

//更新玩家列表 显示输赢
void UserListManager::updateUserList(std::vector<slwhPlayerInfo> vec)
{
	//进行排序
	std::sort(vec.begin(), vec.end(), slwhPlayerInfo::GreatComp);

	auto roominfo = OneGameInfo::GetInstance()->getroomInfo();
	m_userListView->removeAllChildren();
	for (std::vector<UserListOne*>::iterator itor = m_userlistinfo.begin(); itor != m_userlistinfo.end(); itor++)
	{
		delete (*itor);
	}
	m_userlistinfo.clear();
	roominfo->vec.clear();
	for (std::vector<slwhPlayerInfo>::iterator itor = vec.begin(); itor != vec.end(); itor++)
	{
		addUserElement(*itor);
		roominfo->vec.push_back(*itor);
		if ((*itor).userid == Player::getInstance()->getLogonData().dwUserID)
		{
			setWinScore((*itor).winscore, true);
		}
	}

	//更新在线玩家数
	int online = m_userlistinfo.size();
	setOnlineNum(online);


}

//其他玩家压分操作
void UserListManager::otherJet(int userid, long jetadd, SCORE nowscore, dword animalindex)
{
	auto roominfo = OneGameInfo::GetInstance()->getroomInfo();
	for (std::vector<slwhPlayerInfo>::iterator itor = roominfo->vec.begin(); itor != roominfo->vec.end(); itor++)
	{
		if (itor->userid == userid)
		{
			itor->gold = nowscore;
			itor->jet += jetadd;
		}
	}


	for (std::vector<UserListOne*>::iterator itor = m_userlistinfo.begin(); itor != m_userlistinfo.end(); itor++)
	{
		slwhPlayerInfo info = (*itor)->getuserinfo();

		if (info.userid == userid)
		{
			info.gold = nowscore;
			info.jet += jetadd;
			(*itor)->setuserinfo(info);
			//金币飞过去(自己不飞)
			if (info.userid != Player::getInstance()->getLogonData().dwUserID)
			{
				(*itor)->GoldFlyStartPlay(animalindex);
			}
		
			
			if (Player::getInstance()->getLogonData().dwUserID == userid)
			{
				//更新列表上边金币界面
				setScore(nowscore);
				setWinScore(info.jet, false);
			}
			break;
		}
	}

	//更新总压分
	SCORE s = OneGameInfo::GetInstance()->getroomInfo()->alljetinfo.lAllBet;
	setTotelNum(s);
	
}

//玩家取消压分
void UserListManager::othercancelJet(int userid, SCORE jetadd)
{
	for (std::vector<UserListOne*>::iterator itor = m_userlistinfo.begin(); itor != m_userlistinfo.end(); itor++)
	{
		slwhPlayerInfo info = (*itor)->getuserinfo();
		
		if (info.userid == userid)
		{
			info.gold += jetadd;
			info.jet = _ZERO;
			(*itor)->setuserinfo(info);
		
			if (info.userid == Player::getInstance()->getLogonData().dwUserID)
			{
				setWinScore(_ZERO, false);
			}
			(*itor)->setuserinfo(info);

			//如果是自己
			if (Player::getInstance()->getLogonData().dwUserID == userid)
			{
				//更新列表上边金币界面
				setScore(info.gold);
				setWinScore(info.jet, false);
			}
			break;
		}
	}

	//更新总压分
	SCORE s = OneGameInfo::GetInstance()->getroomInfo()->alljetinfo.lAllBet;
	setTotelNum(s);
}

void UserListManager::setOnlineNum(int i)
{
	std::stringstream str;
	str << i;
	m_onlineNum->setString(str.str());
}

void UserListManager::setTotelNum(SCORE score)
{
	char buf[256];
	if (score >= 1000)
	{
		slwhRoomInfo* info = OneGameInfo::GetInstance()->getroomInfo();
		switch (info->wType)
		{
		case 0:
			sprintf(buf, "%.2fK", (float)score / 1000);
			break;
		case 1:
			sprintf(buf, "%.1fK", (float)score / 1000);
			break;
		case 2:
			sprintf(buf, "%.0fK", (float)score / 1000);
			break;
		default:
			break;
		}
	}
	else
	{
		sprintf(buf, "%lld", score);
	}
	m_totelJetNum->setString(buf);
}

void UserListManager::setName(const std::string &name)
{
	m_name->setText(name);
}

void UserListManager::updateInfo(int code /*= 0*/)
{
	//找到自己
	int userid = Player::getInstance()->getLogonData().dwUserID;
	for (std::vector<UserListOne*>::iterator itor = m_userlistinfo.begin(); itor != m_userlistinfo.end(); itor++)
	{
		slwhPlayerInfo info = (*itor)->getuserinfo();
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
		
			setName(info.nickname);
			(*itor)->setuserinfo(info);
			setWinScore(info.winscore, OneGameInfo::GetInstance()->getbCountScore());


			auto scoreData = Player::getInstance()->getLogonData();
			scoreData.lUserScore = info.gold;
			Player::getInstance()->setLogonData(scoreData);

			break;
		}
	}
}

void UserListManager::setHead(int index)
{
	char path[256];
	sprintf(path, _Head_Path, index);
	m_head->setTexture(path);
}

void UserListManager::setVipOrHradBk(int vip)
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

void UserListManager::setScore(SCORE score)
{
	std::stringstream str;
	str << score;
	m_scoreNum->setString(str.str());
}

void UserListManager::goldRollPlay(SCORE targetnum)
{
	//计算差
	SCORE nowNum;
	std::stringstream str(m_scoreNum->getString());
	str >> nowNum;
	m_targetGold = targetnum;
	m_getnum = targetnum - nowNum;
	if (m_getnum > _ZERO)
	{
		scheduleOnce(schedule_selector(UserListManager::goldRolldelay), 3.3f);
	}
}

void UserListManager::goldRolldelay(float  t)
{
	schedule(schedule_selector(UserListManager::goldRollfunc), 0.1f);
}

void UserListManager::goldRollfunc(float t)
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
		unschedule(schedule_selector(UserListManager::goldRollfunc));
	}
}

//刷新用户的分数
void UserListManager::refreshPlayerCoin(Ref* ref)
{
	auto roomInfo = OneGameInfo::GetInstance()->getroomInfo();
	for (std::vector<slwhPlayerInfo>::iterator itor = roomInfo->vec.begin(); itor != roomInfo->vec.end(); itor++)
	{
		if ((*itor).userid == Player::getInstance()->getLogonData().dwUserID)
		{
			(*itor).gold = Player::getInstance()->getLogonData().lUserScore;
			setScore((*itor).gold);
		}
	}
	for (std::vector<UserListOne*>::iterator itor = m_userlistinfo.begin(); itor != m_userlistinfo.end(); itor++)
	{
		slwhPlayerInfo info = (*itor)->getuserinfo();
		if (info.userid == Player::getInstance()->getLogonData().dwUserID)
		{			
			info.gold = Player::getInstance()->getLogonData().lUserScore;	

			/*-------此处更新金币的多少------*/
			auto gameInfo = OneGameInfo::GetInstance();
			int coinScore = gameInfo->getscoreChange();
			coinScore = info.gold;
			gameInfo->setscoreChange(coinScore);
			/*----------------------------*/
			(*itor)->setuserinfo(info);
			break;
		}
	}
}

//刷新vip等级
void UserListManager::refreshPlayerVIP(Ref* ref)
{
	auto roomInfo = OneGameInfo::GetInstance()->getroomInfo();
	for (std::vector<slwhPlayerInfo>::iterator itor = roomInfo->vec.begin(); itor != roomInfo->vec.end(); itor++)
	{
		if ((*itor).userid == Player::getInstance()->getLogonData().dwUserID)
		{
			(*itor).viplevel = Player::getInstance()->getHallData().dwVipLevel;
			setVipOrHradBk((*itor).viplevel);
		}
	}

	for (std::vector<UserListOne*>::iterator itor = m_userlistinfo.begin(); itor != m_userlistinfo.end(); itor++)
	{
		slwhPlayerInfo info = (*itor)->getuserinfo();
		if (info.userid == Player::getInstance()->getLogonData().dwUserID)
		{
			info.viplevel = Player::getInstance()->getHallData().dwVipLevel;					
			(*itor)->setuserinfo(info);

			break;
		}
	}

	InitUserList();
}
