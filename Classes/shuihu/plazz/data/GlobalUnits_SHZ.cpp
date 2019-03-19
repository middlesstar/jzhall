#include "GlobalUnits_SHZ.h"

#define GAME_GLOBAL_SHZ "GameFrame"
//////////////////////////////////////////////////////////////////////////////////
SHZ_CGlobalUnits gGlobalUnits_SHZ;
//////////////////////////////////////////////////////////////////////////////////

//构造函数
SHZ_CGlobalUnits::SHZ_CGlobalUnits()
{
	//默认参数
	DefaultParameter();
	return;
}

//析构函数
SHZ_CGlobalUnits::~SHZ_CGlobalUnits()
{
	return;
}

//创建组件
bool SHZ_CGlobalUnits::InitGlobalUnits()
{
	//加载参数
	LoadParameter();
	return true;
}

//注销组件
bool SHZ_CGlobalUnits::UnInitGlobalUnits()
{
	//保存参数
	SaveParameter();
	return true;
}

//加载参数
void SHZ_CGlobalUnits::LoadParameter()
{
	return;
	//界面配置
	m_bMuteStatuts=JZUserDefault::getInstance()->getBoolForKey(GAME_GLOBAL_SHZ, ("MuteStatuts"), m_bMuteStatuts);
	m_bAllowSound=JZUserDefault::getInstance()->getBoolForKey(GAME_GLOBAL_SHZ, ("AllowSound"),m_bAllowSound);
	m_bAllowBackGroundSound=JZUserDefault::getInstance()->getBoolForKey(GAME_GLOBAL_SHZ, ("AllowBackGroundSound"),m_bAllowBackGroundSound);
	m_bNotifyUserInOut=JZUserDefault::getInstance()->getBoolForKey(GAME_GLOBAL_SHZ, ("NotifyUserInOut"),m_bNotifyUserInOut);
	m_bDebarDetestChat=JZUserDefault::getInstance()->getBoolForKey(GAME_GLOBAL_SHZ, ("DebarDetestChat"),m_bDebarDetestChat);

	//旁观选项
	m_bAllowLookon=JZUserDefault::getInstance()->getBoolForKey(GAME_GLOBAL_SHZ, ("AllowLookon"),m_bAllowLookon);
	m_bAllowFriendLookon=JZUserDefault::getInstance()->getBoolForKey(GAME_GLOBAL_SHZ, ("AllowFriendLookon"),m_bAllowFriendLookon);

	//消息模式
	m_cbMessageMode=(byte)JZUserDefault::getInstance()->getIntegerForKey(GAME_GLOBAL_SHZ, ("MessageMode"),m_cbMessageMode);
	switch (m_cbMessageMode)
	{
	case MESSAGE_MODE_ALL_SHZ:
	case MESSAGE_MODE_FRIEND_SHZ: { break; }
	default: { m_cbMessageMode=MESSAGE_MODE_DETEST_SHZ; }
	}
}

//保存参数
void SHZ_CGlobalUnits::SaveParameter()
{
	return;
	//界面配置
	JZUserDefault::getInstance()->setBoolForKey(GAME_GLOBAL_SHZ, ("MuteStatuts"), m_bMuteStatuts);
	JZUserDefault::getInstance()->setBoolForKey(GAME_GLOBAL_SHZ, ("AllowSound"),m_bAllowSound);
	JZUserDefault::getInstance()->setBoolForKey(GAME_GLOBAL_SHZ, ("AllowBackGroundSound"),m_bAllowBackGroundSound);
	JZUserDefault::getInstance()->setBoolForKey(GAME_GLOBAL_SHZ, ("NotifyUserInOut"),m_bNotifyUserInOut);
	JZUserDefault::getInstance()->setBoolForKey(GAME_GLOBAL_SHZ, ("DebarDetestChat"),m_bDebarDetestChat);

	//旁观选项
	JZUserDefault::getInstance()->setBoolForKey(GAME_GLOBAL_SHZ, ("AllowLookon"),m_bAllowLookon);
	JZUserDefault::getInstance()->setBoolForKey(GAME_GLOBAL_SHZ, ("AllowFriendLookon"),m_bAllowFriendLookon);

	//消息模式
	JZUserDefault::getInstance()->setIntegerForKey(GAME_GLOBAL_SHZ, ("MessageMode"),m_cbMessageMode);

	return;
}

//默认参数
void SHZ_CGlobalUnits::DefaultParameter()
{
	//其他选项
	m_bMuteStatuts=false;
	m_bAllowSound=true;
	m_bAllowBackGroundSound=true;
	m_bNotifyUserInOut=true;
	m_bDebarDetestChat=false;

	//旁观选项
	m_bAllowLookon=false;
	m_bAllowFriendLookon=true;

	//模式变量
	m_cbMessageMode=MESSAGE_MODE_DETEST_SHZ;

	return;
}
