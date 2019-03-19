#include "OneGameInfo.h"
#include "cocos2d.h"
#include "slwh/slwhDefine.h"
USING_NS_CC;


OneGameInfo* OneGameInfo::m_instance = nullptr;
OneGameInfo::OneGameInfo() :
m_bStartGameFinish(false),
m_bIsCountScore(false),
m_ServerState(enumServerState::_Jet)
{
	//（红）动物0 1 2 3   （绿）动物0 1 2 3    （黄）动物0 1 2 3     庄 和 闲 
	m_rate0 = new int[15]_BeiLv0;
	m_rate1 = new int[15]_BeiLv1;
	m_rate2 = new int[15]_BeiLv2;
	m_rate3 = new int[15]_BeiLv3;
	m_rate4 = new int[15]_BeiLv4;

	m_resRecord = new int[20]{_ZERO};
	m_roomInfo = new slwhRoomInfo();
}

OneGameInfo::~OneGameInfo()
{
	delete[] m_rate0;
	delete[] m_rate1;
	delete[] m_rate2;
	delete[] m_rate3;
	delete[] m_rate4;
	delete[] m_resRecord;
	delete m_roomInfo;
}

void OneGameInfo::setResRecord(int* arr)
{
	memcpy(m_resRecord, arr, sizeof(int)*20);
}

OneGameInfo* OneGameInfo::GetInstance()
{
	if (nullptr == m_instance)
	{
		m_instance = new OneGameInfo();
	}
	return m_instance;
}

void OneGameInfo::relese()
{
	delete m_instance;
	m_instance = nullptr;
}

int* OneGameInfo::getRate()
{
	switch (m_roomInfo->dwRateID)
	{
	case 0:	return m_rate0;
	case 1:	return m_rate1;
	case 2:	return m_rate2;
	case 3: return m_rate3;
	case 4: return m_rate4;
	default:return nullptr;
	}
	return nullptr;
}

std::string OneGameInfo::getSLWHMusicFileFullPath(const std::string& strPath)
{
	std::string strRetPath = strPath;
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	strRetPath = "music/android/" + strRetPath + ".ogg";
#else
	strRetPath = "music/ios/" + strRetPath + ".mp3";
#endif

	return strRetPath;
}