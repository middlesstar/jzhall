#include "GlobalUnits_SHZ.h"

#define GAME_GLOBAL_SHZ "GameFrame"
//////////////////////////////////////////////////////////////////////////////////
SHZ_CGlobalUnits gGlobalUnits_SHZ;
//////////////////////////////////////////////////////////////////////////////////

//���캯��
SHZ_CGlobalUnits::SHZ_CGlobalUnits()
{
	//Ĭ�ϲ���
	DefaultParameter();
	return;
}

//��������
SHZ_CGlobalUnits::~SHZ_CGlobalUnits()
{
	return;
}

//�������
bool SHZ_CGlobalUnits::InitGlobalUnits()
{
	//���ز���
	LoadParameter();
	return true;
}

//ע�����
bool SHZ_CGlobalUnits::UnInitGlobalUnits()
{
	//�������
	SaveParameter();
	return true;
}

//���ز���
void SHZ_CGlobalUnits::LoadParameter()
{
	return;
	//��������
	m_bMuteStatuts=JZUserDefault::getInstance()->getBoolForKey(GAME_GLOBAL_SHZ, ("MuteStatuts"), m_bMuteStatuts);
	m_bAllowSound=JZUserDefault::getInstance()->getBoolForKey(GAME_GLOBAL_SHZ, ("AllowSound"),m_bAllowSound);
	m_bAllowBackGroundSound=JZUserDefault::getInstance()->getBoolForKey(GAME_GLOBAL_SHZ, ("AllowBackGroundSound"),m_bAllowBackGroundSound);
	m_bNotifyUserInOut=JZUserDefault::getInstance()->getBoolForKey(GAME_GLOBAL_SHZ, ("NotifyUserInOut"),m_bNotifyUserInOut);
	m_bDebarDetestChat=JZUserDefault::getInstance()->getBoolForKey(GAME_GLOBAL_SHZ, ("DebarDetestChat"),m_bDebarDetestChat);

	//�Թ�ѡ��
	m_bAllowLookon=JZUserDefault::getInstance()->getBoolForKey(GAME_GLOBAL_SHZ, ("AllowLookon"),m_bAllowLookon);
	m_bAllowFriendLookon=JZUserDefault::getInstance()->getBoolForKey(GAME_GLOBAL_SHZ, ("AllowFriendLookon"),m_bAllowFriendLookon);

	//��Ϣģʽ
	m_cbMessageMode=(byte)JZUserDefault::getInstance()->getIntegerForKey(GAME_GLOBAL_SHZ, ("MessageMode"),m_cbMessageMode);
	switch (m_cbMessageMode)
	{
	case MESSAGE_MODE_ALL_SHZ:
	case MESSAGE_MODE_FRIEND_SHZ: { break; }
	default: { m_cbMessageMode=MESSAGE_MODE_DETEST_SHZ; }
	}
}

//�������
void SHZ_CGlobalUnits::SaveParameter()
{
	return;
	//��������
	JZUserDefault::getInstance()->setBoolForKey(GAME_GLOBAL_SHZ, ("MuteStatuts"), m_bMuteStatuts);
	JZUserDefault::getInstance()->setBoolForKey(GAME_GLOBAL_SHZ, ("AllowSound"),m_bAllowSound);
	JZUserDefault::getInstance()->setBoolForKey(GAME_GLOBAL_SHZ, ("AllowBackGroundSound"),m_bAllowBackGroundSound);
	JZUserDefault::getInstance()->setBoolForKey(GAME_GLOBAL_SHZ, ("NotifyUserInOut"),m_bNotifyUserInOut);
	JZUserDefault::getInstance()->setBoolForKey(GAME_GLOBAL_SHZ, ("DebarDetestChat"),m_bDebarDetestChat);

	//�Թ�ѡ��
	JZUserDefault::getInstance()->setBoolForKey(GAME_GLOBAL_SHZ, ("AllowLookon"),m_bAllowLookon);
	JZUserDefault::getInstance()->setBoolForKey(GAME_GLOBAL_SHZ, ("AllowFriendLookon"),m_bAllowFriendLookon);

	//��Ϣģʽ
	JZUserDefault::getInstance()->setIntegerForKey(GAME_GLOBAL_SHZ, ("MessageMode"),m_cbMessageMode);

	return;
}

//Ĭ�ϲ���
void SHZ_CGlobalUnits::DefaultParameter()
{
	//����ѡ��
	m_bMuteStatuts=false;
	m_bAllowSound=true;
	m_bAllowBackGroundSound=true;
	m_bNotifyUserInOut=true;
	m_bDebarDetestChat=false;

	//�Թ�ѡ��
	m_bAllowLookon=false;
	m_bAllowFriendLookon=true;

	//ģʽ����
	m_cbMessageMode=MESSAGE_MODE_DETEST_SHZ;

	return;
}
