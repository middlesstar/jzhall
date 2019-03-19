#include "GameInfo.h"
#include "cocos2d.h"
#include "shark/sharkDefine.h"
#include "shark/resourceDef.h"
USING_NS_CC;


GameInfo* GameInfo::m_instance = nullptr;
GameInfo::GameInfo() :
m_resRecord(nullptr),
m_bStartGameFinish(false),
m_ServerState(gameState::STAKE)
{
	m_rate0 = new int[12]_Rate1(0);
	m_rate1 = new int[12]_Rate2(0);
	m_rate2 = new int[12]_Rate3(0);
	m_rate3 = new int[12]_Rate4(0);
	m_rate4 = new int[12]_Rate5(0);
	m_rate5 = new int[12]_Rate6(0);
	m_rate6 = new int[12]_Rate7(0);


	m_resRecord = new int[12]{_ZERO};
	m_roomInfo = new RoomInfo_Shark();
}

GameInfo::~GameInfo()
{
	delete[] m_rate0;
	delete[] m_rate1;
	delete[] m_rate2;
	delete[] m_rate3;
	delete[] m_rate4;
	delete[] m_resRecord;
	delete m_roomInfo;
}

void GameInfo::setResRecord(int* arr)
{
	memcpy(m_resRecord, arr, sizeof(int)* 12);
}

GameInfo* GameInfo::GetInstance()
{
	if (nullptr == m_instance)
	{
		m_instance = new GameInfo();
	}
	return m_instance;
}

int* GameInfo::getRate()
{
	switch (m_roomInfo->dwRateID)
	{
	case 0:
	{
			  m_rate0[1] = m_roomInfo->JsRate;
			  return m_rate0;
	}				
	case 1:
	{
			  m_rate1[1] = m_roomInfo->JsRate;
			  return m_rate1;
	}
	case 2:	
	{
			  m_rate2[1] = m_roomInfo->JsRate;
			  return m_rate2;
	}
	case 3:	
	{
			  m_rate3[1] = m_roomInfo->JsRate;
			  return m_rate3;
	}
	case 4:	
	{
			  m_rate4[1] = m_roomInfo->JsRate;
			  return m_rate4;
	}
	case 5:	
	{
			  m_rate5[1] = m_roomInfo->JsRate;
			  return m_rate5;
	}
	case 6:	
	{
			  m_rate6[1] = m_roomInfo->JsRate;
			  return m_rate6;
	}
	default:return nullptr;
	}
	return nullptr;
}

//决定背景音乐用哪一个
void GameInfo::selectMusicBack()
{
	int musicIndex = abs(rand()) % 2;
	if (musicIndex == 1)
	{
		_PLAY_MUSCI_SHARK(BACKGROUDMUSIC1, true);
	}
	else
	{
		_PLAY_MUSCI_SHARK(BACKGROUDMUSIC2, true);
	}
}







