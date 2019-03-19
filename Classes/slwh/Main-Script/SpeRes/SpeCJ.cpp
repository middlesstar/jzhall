#include "SpeCJ.h"


#define ROTATE_TIME		3.f
#define SCALE_TIME		1.f


//节点下的name
#define _LIGHT			"light"
#define _NAME0			"Name0"
#define _NUM0			"Num0"
#define _GOLD0			"Gold0"
#define _NAME1			"Name1"
#define _NUM1			"Num1"
#define _GOLD1			"Gold1"
#define _NAME2			"Name2"
#define _NUM2			"Num2"
#define _GOLD2			"Gold2"
#define _CJLX_TEXT		"CJLX"

SpeCJ::SpeCJ() :
m_light(nullptr),
m_bIsOpened(false),
m_cjlxText(nullptr)
{
	memset(m_name,NULL,sizeof(m_name));
	memset(m_num, NULL, sizeof(m_num));
	memset(m_gold,NULL,sizeof(m_gold));
}

SpeCJ::~SpeCJ()
{
}

void SpeCJ::Start()
{
	m_light = dynamic_cast<ImageView*>(m_node->getChildByName(_LIGHT));
	CC_ASSERT(nullptr != m_light);

	m_name[0] = dynamic_cast<Text*>(m_node->getChildByName(_NAME0));
	CC_ASSERT(nullptr != m_name[0]);

	m_num[0] = dynamic_cast<TextAtlas*>(m_node->getChildByName(_NUM0));
	CC_ASSERT(nullptr != m_num[0]);

	m_gold[0] = dynamic_cast<ImageView*>(m_node->getChildByName(_GOLD0));
	CC_ASSERT(nullptr != m_gold[0]);

	m_name[1] = dynamic_cast<Text*>(m_node->getChildByName(_NAME1));
	CC_ASSERT(nullptr != m_name[1]);

	m_num[1] = dynamic_cast<TextAtlas*>(m_node->getChildByName(_NUM1));
	CC_ASSERT(nullptr != m_num[1]);

	m_gold[1] = dynamic_cast<ImageView*>(m_node->getChildByName(_GOLD1));
	CC_ASSERT(nullptr != m_gold[1]);

	m_name[2] = dynamic_cast<Text*>(m_node->getChildByName(_NAME2));
	CC_ASSERT(nullptr != m_name[2]);

	m_num[2] = dynamic_cast<TextAtlas*>(m_node->getChildByName(_NUM2));
	CC_ASSERT(nullptr != m_num[2]);

	m_gold[2] = dynamic_cast<ImageView*>(m_node->getChildByName(_GOLD2));
	CC_ASSERT(nullptr != m_gold[2]);

	m_cjlxText = dynamic_cast<Text*>(m_node->getChildByName(_CJLX_TEXT));
	CC_ASSERT(nullptr != m_cjlxText);

	m_node->setScale(_ZERO);
}

void SpeCJ::StartPlay(int ncj,std::string name[3], longlong num[3])
{
	if (nullptr == m_node)
	{
		return;
	}
	if (true == m_bIsOpened)
	{
		return;
	}
	m_cjlxText->setVisible(false);
	int index = _ZERO;
	for (size_t i = _ZERO; i < 3; i++)
	{

		if (i < ncj)
		{
			m_name[i]->setVisible(true);
			m_num[i]->setVisible(true);
			m_gold[i]->setVisible(true);
			m_name[i]->setString(name[i]);
			std::stringstream str;
			str << num[i];
			m_num[i]->setString(str.str());
		}
		else
		{
			m_name[i]->setVisible(false);
			m_num[i]->setVisible(false);
			m_gold[i]->setVisible(false);
		}	
	}

	//无人中奖
	if (ncj == _ZERO)
	{
		m_cjlxText->setVisible(true);
	}

	m_bIsOpened = true;
	m_node->runAction(EaseBounceOut::create(CCScaleTo::create(SCALE_TIME, 1.f)));
	m_light->runAction(CCRepeatForever::create(CCRotateBy::create(ROTATE_TIME, Vec3(_ZERO, _ZERO, 360))));

}

void SpeCJ::StopPlay()
{
	if (nullptr == m_node)
	{
		return;
	}
	if (false == m_bIsOpened)
	{
		return;
	}
	m_node->stopAllActions();
	m_node->runAction(CCSequence::create(
		CCScaleTo::create(SCALE_TIME, _ZERO),
		CCCallFunc::create(this, callfunc_selector(SpeCJ::CloseCallBack)),
		NULL));
}

void SpeCJ::CloseCallBack()
{
	m_light->stopAllActions();
	m_bIsOpened = false;
}