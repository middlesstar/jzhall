#include "ActiveInfo.h"

ActiveInfo::ActiveInfo() :
m_timeLabel(NULL),
m_goldLabel(NULL),
m_hfLabel(NULL),
m_time(NULL),
m_gold(NULL),
m_hf(NULL),
m_nowTime(_ZERO),
m_goldNum(_ZERO),
m_hfNum(_ZERO)
{
}

ActiveInfo::~ActiveInfo()
{
}

void ActiveInfo::OnInit()
{



	m_timeLabel = Label::create(INFO_TIME, TEXT_FONT, INFO_SIZE);
	CC_ASSERT(NULL != m_timeLabel);
	m_timeLabel->setColor(INFO_LABEL_COLOR);
	m_timeLabel->setPosition(Vec2(50, VISIBLE_SIZE.y - INFO_YOFFSET));
	addChild(m_timeLabel);


	m_goldLabel = Label::create(INFO_GOLD, TEXT_FONT, INFO_SIZE);
	CC_ASSERT(NULL != m_goldLabel);
	m_goldLabel->setColor(INFO_LABEL_COLOR);
	m_goldLabel->setPosition(Vec2(180, VISIBLE_SIZE.y - INFO_YOFFSET));
	addChild(m_goldLabel);

	m_hfLabel = Label::create(INFO_HF, TEXT_FONT, INFO_SIZE);
	CC_ASSERT(NULL != m_hfLabel);
	m_hfLabel->setColor(INFO_LABEL_COLOR);
	m_hfLabel->setPosition(Vec2(360, VISIBLE_SIZE.y - INFO_YOFFSET));
	addChild(m_hfLabel);


	m_time = Text::create("", TEXT_FONT, INFO_SIZE);
	CC_ASSERT(NULL != m_time);
	m_time->setColor(INFO_TEXT_COLOR);
	m_time->setPosition(Vec2(100, VISIBLE_SIZE.y - INFO_YOFFSET));
	addChild(m_time);


	m_gold = Text::create("", TEXT_FONT, INFO_SIZE);
	CC_ASSERT(NULL != m_gold);
	m_gold->setColor(INFO_TEXT_COLOR);
	m_gold->setAnchorPoint(Vec2(0.f, 0.5f));
	m_gold->setPosition(Vec2(210, VISIBLE_SIZE.y - INFO_YOFFSET));
	addChild(m_gold);


	m_hf = Text::create("", TEXT_FONT, INFO_SIZE);
	CC_ASSERT(NULL != m_hf);
	m_hf->setColor(INFO_TEXT_COLOR);
	m_hf->setAnchorPoint(Vec2(0.f, 0.5f));
	m_hf->setPosition(Vec2(400, VISIBLE_SIZE.y - INFO_YOFFSET));
	addChild(m_hf);


	//初始化数据
	InitData();
}


void ActiveInfo::AddTime(float t)
{

	m_nowTime += t;

	std::stringstream str;
	str << INFO_TIME_MAX - (int)(m_nowTime - _RAND0);
	m_time->setString(str.str());
}

void ActiveInfo::InitData()
{
	m_nowTime = _RAND0;
	m_goldNum = _RAND1;
	m_hfNum = _RAND2;

	AddTime(_ZERO);
	AddGold(_ZERO);
	AddHF(_ZERO);

}


void ActiveInfo::AddGold(int t)
{
	m_goldNum += t;

	std::stringstream str;
	str << m_goldNum - _RAND1;
	m_gold->setString(str.str());
}


void ActiveInfo::AddHF(float t)
{
	m_hfNum += t;

	std::stringstream str;
	str << m_hfNum - _RAND2;
	m_hf->setString(str.str());

}

