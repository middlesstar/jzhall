#include "OptionParameter_SHZ.h"

//构造函数
SHZCParameterGame::SHZCParameterGame()
{
	//默认参数
	DefaultParameter();

	//属性变量
	memset(m_szRegKeyName, 0, sizeof(m_szRegKeyName));

	return;
}

//析构函数
SHZCParameterGame::~SHZCParameterGame()
{
}

//加载参数
void SHZCParameterGame::LoadParameter()
{
	return;
	//配置表项
	
	//胜率限制
	m_wMinWinRate=(word)JZUserDefault::getInstance()->getIntegerForKey(m_szRegKeyName, "MinWinRate", m_wMinWinRate);
	m_bLimitWinRate=JZUserDefault::getInstance()->getBoolForKey(m_szRegKeyName, "LimitWinRate",m_bLimitWinRate);

	//逃率限制
	m_wMaxFleeRate=(word)JZUserDefault::getInstance()->getIntegerForKey(m_szRegKeyName, "MaxFleeRate",m_wMaxFleeRate);
	m_bLimitFleeRate=JZUserDefault::getInstance()->getBoolForKey(m_szRegKeyName, "LimitFleeRate",m_bLimitFleeRate);
		
	//积分限制
	m_lMaxGameScore=JZUserDefault::getInstance()->getIntegerForKey(m_szRegKeyName, "MaxGameScore",m_lMaxGameScore);
	m_lMinGameScore=JZUserDefault::getInstance()->getIntegerForKey(m_szRegKeyName, "MinGameScore",m_lMinGameScore);
	m_bLimitGameScore=JZUserDefault::getInstance()->getBoolForKey(m_szRegKeyName, "LimitGameScore",m_bLimitGameScore);
	
	return;
}

//保存参数
void SHZCParameterGame::SaveParameter()
{
	return;
	//配置表项

	//胜率限制
	JZUserDefault::getInstance()->setIntegerForKey(m_szRegKeyName, "MinWinRate", m_wMinWinRate);
	JZUserDefault::getInstance()->setBoolForKey(m_szRegKeyName, "LimitWinRate",m_bLimitWinRate);

	//逃率限制
	JZUserDefault::getInstance()->setIntegerForKey(m_szRegKeyName, "MaxFleeRate",m_wMaxFleeRate);
	JZUserDefault::getInstance()->setBoolForKey(m_szRegKeyName, "LimitFleeRate",m_bLimitFleeRate);

	//积分限制
	JZUserDefault::getInstance()->setIntegerForKey(m_szRegKeyName, "MaxGameScore",m_lMaxGameScore);
	JZUserDefault::getInstance()->setIntegerForKey(m_szRegKeyName, "MinGameScore",m_lMinGameScore);
	JZUserDefault::getInstance()->setBoolForKey(m_szRegKeyName, "LimitGameScore",m_bLimitGameScore);

	return;
}

//默认参数
void SHZCParameterGame::DefaultParameter()
{
	//胜率限制
	m_wMinWinRate=0;
	m_bLimitWinRate=false;

	//逃率限制
	m_wMaxFleeRate=5000;
	m_bLimitFleeRate=false;

	//积分限制
	m_bLimitGameScore=false;
	m_lMaxGameScore=2147483647L;
	m_lMinGameScore=-2147483646L;

	return;
}

//配置参数
bool SHZCParameterGame::InitParameter(const char* pszProcessName)
{
	//构造键名
	word wNameIndex=0;
	while (wNameIndex<(countarray(m_szRegKeyName)-1))
	{
		//终止判断
		if (pszProcessName[wNameIndex]==0) break;
		if (pszProcessName[wNameIndex]=='.') break;

		//设置字符
		word wCurrentIndex=wNameIndex++;
		m_szRegKeyName[wCurrentIndex]=pszProcessName[wCurrentIndex];
	}

	//设置变量
	m_szRegKeyName[wNameIndex]=0;

	return true;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
SHZCParameterServer::SHZCParameterServer()
{
	//默认参数
	DefaultParameter();

	return;
}

//析构函数
SHZCParameterServer::~SHZCParameterServer()
{
}

//默认参数
void SHZCParameterServer::DefaultParameter()
{
	//配置变量
	m_bTakePassword=false;
	zeromemory(m_szPassword,sizeof(m_szPassword));

	return;
}

//////////////////////////////////////////////////////////////////////////
//获取对象
//静态变量
SHZCParameterGlobal * __gParameterGlobal_SHZ=0;			//全局配置

SHZCParameterGlobal* SHZCParameterGlobal::shared()
{
	if (__gParameterGlobal_SHZ == 0)
		__gParameterGlobal_SHZ = new SHZCParameterGlobal();
	return __gParameterGlobal_SHZ;
}

void SHZCParameterGlobal::purge()
{
	if (__gParameterGlobal_SHZ)
		delete __gParameterGlobal_SHZ;
	__gParameterGlobal_SHZ = 0;
}

//////////////////////////////////////////////////////////////////////////
//构造函数
SHZCParameterGlobal::SHZCParameterGlobal()
{
	//默认参数
	DefaultParameter();
	return;
}

//析构函数
SHZCParameterGlobal::~SHZCParameterGlobal()
{
	CParameterGameMap_SHZ::iterator GameIter = m_ParameterGameMap.begin();
	for (; GameIter != m_ParameterGameMap.end(); ++GameIter)
	{
		delete GameIter->second;
	}

	m_ParameterGameMap.clear();

	CParameterServerMap_SHZ::iterator ServerIter = m_ParameterServerMap.begin();
	for (; ServerIter != m_ParameterServerMap.end(); ++ServerIter)
	{
		delete ServerIter->second;
	}

	m_ParameterServerMap.clear();
	return;
}

//加载参数
void SHZCParameterGlobal::LoadParameter()
{
	return;
	//自动登录
	m_bLogonAuto=JZUserDefault::getInstance()->getBoolForKey(REG_GOBAL_OPTION_SHZ,"LogonAuto",m_bLogonAuto);
	m_bWriteCookie=JZUserDefault::getInstance()->getBoolForKey(REG_GOBAL_OPTION_SHZ,"WriteCookie",m_bWriteCookie);

	//邀请模式
	m_cbInviteMode=(byte)JZUserDefault::getInstance()->getIntegerForKey(REG_GOBAL_OPTION_SHZ,"InviteMode",m_cbInviteMode);
	switch (m_cbInviteMode)
	{
	case INVITE_MODE_NONE_SHZ:
	case INVITE_MODE_FRIEND_SHZ: { break; }
	default: { m_cbInviteMode=INVITE_MODE_ALL_SHZ; }
	}

	//消息模式
	m_cbMessageMode=(byte)JZUserDefault::getInstance()->getIntegerForKey(REG_GOBAL_OPTION_SHZ,"MessageMode",m_cbMessageMode);
	switch (m_cbMessageMode)
	{
	case MESSAGE_MODE_ALL_SHZ:
	case MESSAGE_MODE_FRIEND_SHZ: { break; }
	default: { m_cbMessageMode=MESSAGE_MODE_DETEST_SHZ; }
	}

	//自动加入
	m_cbActionHitAutoJoin=(byte)JZUserDefault::getInstance()->getIntegerForKey(REG_GOBAL_OPTION_SHZ,"ActionHitAutoJoin",m_cbActionHitAutoJoin);
	switch (m_cbActionHitAutoJoin)
	{
	case ACTION_SHOW_SEARCH_DLG_SHZ: { break; }
	default: { m_cbActionHitAutoJoin=ACTION_SEARCH_TABLE_SHZ; }
	}

	//双击列表
	m_cbActionLeftDoubleList=(byte)JZUserDefault::getInstance()->getIntegerForKey(REG_GOBAL_OPTION_SHZ,"ActionLeftDoubleList",m_cbActionLeftDoubleList);
	switch (m_cbActionLeftDoubleList)
	{
	case ACTION_ORIENTATION_SHZ:
	case ACTION_SHOW_USER_INFO_SHZ: { break; }
	default: { m_cbActionLeftDoubleList=ACTION_SEND_WHISPER_SHZ; }
	}

	//房间配置
	m_bLimitDetest=JZUserDefault::getInstance()->getBoolForKey(REG_GOBAL_OPTION_SHZ,"LimitDetest",m_bLimitDetest);
	m_bLimitSameIP=JZUserDefault::getInstance()->getBoolForKey(REG_GOBAL_OPTION_SHZ,"LimitSameIP",m_bLimitSameIP);
	m_bNotifyUserInOut=JZUserDefault::getInstance()->getBoolForKey(REG_GOBAL_OPTION_SHZ,"NotifyUserInOut",m_bNotifyUserInOut);
	m_bNotifyFriendCome=JZUserDefault::getInstance()->getBoolForKey(REG_GOBAL_OPTION_SHZ,"NotifyFriendCome",m_bNotifyFriendCome);

	//系统配置
	m_bAllowSound=JZUserDefault::getInstance()->getBoolForKey(REG_GOBAL_OPTION_SHZ,"AllowSound",m_bAllowSound);
	m_bAllowBackMusic=JZUserDefault::getInstance()->getBoolForKey(REG_GOBAL_OPTION_SHZ,"AllowBackMusic",m_bAllowBackMusic);
	m_bAutoSitDown=JZUserDefault::getInstance()->getBoolForKey(REG_GOBAL_OPTION_SHZ,"AutoSitDown",m_bAutoSitDown);
	m_bAutoShowWhisper=JZUserDefault::getInstance()->getBoolForKey(REG_GOBAL_OPTION_SHZ,"AutoShowWhisper",m_bAutoShowWhisper);
	m_bSaveWhisperChat=JZUserDefault::getInstance()->getBoolForKey(REG_GOBAL_OPTION_SHZ,"SaveWhisperChat",m_bSaveWhisperChat);
	m_bSendWhisperByEnter=JZUserDefault::getInstance()->getBoolForKey(REG_GOBAL_OPTION_SHZ,"SendWhisperByEnter",m_bSendWhisperByEnter);

	return;
}

//保存参数
void SHZCParameterGlobal::SaveParameter()
{
	return;
	//控制变量
	{
		//自动登录
		JZUserDefault::getInstance()->setBoolForKey(REG_GOBAL_OPTION_SHZ,"LogonAuto",m_bLogonAuto);
		JZUserDefault::getInstance()->setBoolForKey(REG_GOBAL_OPTION_SHZ,"WriteCookie",m_bWriteCookie);

		//邀请模式
		JZUserDefault::getInstance()->setIntegerForKey(REG_GOBAL_OPTION_SHZ,"InviteMode",m_cbInviteMode);
		
		//消息模式
		JZUserDefault::getInstance()->setIntegerForKey(REG_GOBAL_OPTION_SHZ,"MessageMode",m_cbMessageMode);
		
		//自动加入
		JZUserDefault::getInstance()->setIntegerForKey(REG_GOBAL_OPTION_SHZ,"ActionHitAutoJoin",m_cbActionHitAutoJoin);
		
		//双击列表
		JZUserDefault::getInstance()->setIntegerForKey(REG_GOBAL_OPTION_SHZ,"ActionLeftDoubleList",m_cbActionLeftDoubleList);
		
		//房间配置
		JZUserDefault::getInstance()->setBoolForKey(REG_GOBAL_OPTION_SHZ,"LimitDetest",m_bLimitDetest);
		JZUserDefault::getInstance()->setBoolForKey(REG_GOBAL_OPTION_SHZ,"LimitSameIP",m_bLimitSameIP);
		JZUserDefault::getInstance()->setBoolForKey(REG_GOBAL_OPTION_SHZ,"NotifyUserInOut",m_bNotifyUserInOut);
		JZUserDefault::getInstance()->setBoolForKey(REG_GOBAL_OPTION_SHZ,"NotifyFriendCome",m_bNotifyFriendCome);

		//系统配置
		JZUserDefault::getInstance()->setBoolForKey(REG_GOBAL_OPTION_SHZ,"AllowSound",m_bAllowSound);
		JZUserDefault::getInstance()->setBoolForKey(REG_GOBAL_OPTION_SHZ,"AllowBackMusic",m_bAllowBackMusic);
		JZUserDefault::getInstance()->setBoolForKey(REG_GOBAL_OPTION_SHZ,"AutoSitDown",m_bAutoSitDown);
		JZUserDefault::getInstance()->setBoolForKey(REG_GOBAL_OPTION_SHZ,"AutoShowWhisper",m_bAutoShowWhisper);
		JZUserDefault::getInstance()->setBoolForKey(REG_GOBAL_OPTION_SHZ,"SaveWhisperChat",m_bSaveWhisperChat);
		JZUserDefault::getInstance()->setBoolForKey(REG_GOBAL_OPTION_SHZ,"SendWhisperByEnter",m_bSendWhisperByEnter);

	}

	
	CParameterGameMap_SHZ::iterator GameIter = m_ParameterGameMap.begin();
	for (; GameIter != m_ParameterGameMap.end(); ++GameIter)
	{
		GameIter->second->SaveParameter();
	}
	return;
}

//默认参数
void SHZCParameterGlobal::DefaultParameter()
{
	//登录配置
	m_bLogonAuto=false;
	m_bWriteCookie=true;
	m_bRemberPassword=false;

	//时间配置
	m_wIntermitTime=0L;
	m_wOnLineCountTime=0L;

	//房间配置
	m_cbInviteMode=INVITE_MODE_ALL_SHZ;
	m_cbMessageMode=MESSAGE_MODE_ALL_SHZ;
	m_cbActionHitAutoJoin=ACTION_SEARCH_TABLE_SHZ;
	m_cbActionLeftDoubleList=ACTION_SEND_WHISPER_SHZ;

	//房间配置
	m_bLimitDetest=false;
	m_bLimitSameIP=false;
	m_bNotifyUserInOut=false;
	m_bNotifyFriendCome=true;

	//系统配置
	m_bAllowSound=true;
	m_bAllowBackMusic=true;
	m_bAutoSitDown=true;
	m_bSaveWhisperChat=true;
	m_bAutoShowWhisper=false;
	m_bSendWhisperByEnter=true;

	return;
}

//游戏配置
SHZCParameterGame * SHZCParameterGlobal::GetParameterGame(tagGameKind * pGameKind)
{
	//寻找现存
	CParameterGameMap_SHZ::iterator it= m_ParameterGameMap.find(pGameKind->wKindID);
	if (it != m_ParameterGameMap.end())
		return it->second;

	//创建对象
	try
	{
		//创建对象
		SHZCParameterGame* pParameterGame=new SHZCParameterGame;
		if (pParameterGame==0) return 0;

		//配置对象
		pParameterGame->InitParameter(pGameKind->szProcessName);

		//加载参数
		pParameterGame->LoadParameter();

		//设置数组
		m_ParameterGameMap[pGameKind->wKindID]=pParameterGame;

		return pParameterGame;
	}
	catch (...) {}

	return 0;
}

//房间配置
SHZCParameterServer * SHZCParameterGlobal::GetParameterServer(tagSHZ_GameServer * pGameServer)
{
	//寻找现存
	CParameterServerMap_SHZ::iterator it= m_ParameterServerMap.find(pGameServer->wServerID);
	if (it != m_ParameterServerMap.end())
		return it->second;

	//创建对象
	try
	{
		//创建对象
		SHZCParameterServer* pParameterServer=new SHZCParameterServer;
		if (pParameterServer==0) return 0;

		//设置数组
		m_ParameterServerMap[pGameServer->wServerID]=pParameterServer;

		return pParameterServer;
	}
	catch (...) {}

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////
