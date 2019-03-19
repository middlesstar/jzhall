#include "UIUserInfo.h"

//�ڵ��µ��߼���ǩ
#define _YUANBAO	1326
#define _GOLD		1324

#define _NUM_NAME	"Num"

#define Hall_Message_Refresh_Coin			"Hall_Message_Refresh_Coin"			//ˢ�½����ʾ

UIUserInfo::UIUserInfo() :
m_yuanbao(nullptr),
m_gold(nullptr)
{
	HALL_NOTIFICATION_REG(Hall_Message_Refresh_Coin, UIUserInfo::refreshPlayerCoin, nullptr);
}


UIUserInfo::~UIUserInfo()
{
	HALL_NOTIFICATION_UNREG(Hall_Message_Refresh_Coin);
}


void UIUserInfo::updateInfo(float t)
{
	update();
}


void UIUserInfo::update()
{
	auto info = Player::getInstance();
	SetYunBao(info->getHallData().llYuanBao);
	SetGold(info->getLogonData().lUserScore);
}

void UIUserInfo::Start()
{

	m_yuanbao = dynamic_cast<TextAtlas*>(m_node->getChildByTag(_YUANBAO)->getChildByName(_NUM_NAME));
	CC_ASSERT(nullptr != m_yuanbao);

	m_gold = dynamic_cast<TextAtlas*>(m_node->getChildByTag(_GOLD)->getChildByName(_NUM_NAME));
	CC_ASSERT(nullptr != m_gold);

	update();
	//ˢ��һ������
	schedule(schedule_selector(UIUserInfo::updateInfo), 0.5f);
}

void UIUserInfo::SetYunBao(int y)
{
	std::stringstream str;
	str << y;
	m_yuanbao->setString(str.str());
}


void UIUserInfo::SetGold(int g)
{
	std::stringstream str;
	str << g;
	m_gold->setString(str.str());
}


//ˢ��vip�ȼ����û��ķ���
void UIUserInfo::refreshPlayerCoin(Ref* ref)
{
	auto info = Player::getInstance();
	SetGold(info->getLogonData().lUserScore);
}


