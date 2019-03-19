#include "JCNum.h"

//节点下的艺术数字逻辑标签
#define _NUM_NAME		"Num"

JCNum::JCNum():
m_num(nullptr)
{
}


JCNum::~JCNum()
{
}


void JCNum::Start()
{
	m_num = dynamic_cast<TextAtlas*>(m_node->getChildByName(_NUM_NAME));
	CC_ASSERT(nullptr != m_num);


}

void JCNum::SetNum(int u)
{
	if (nullptr == m_node)
		return;

	std::stringstream str;
	str << u;
	m_num->setString(str.str());
}


int JCNum::GetNum()
{
	if (nullptr == m_node)
		return _ZERO;

	int u;
	std::stringstream str(m_num->getString());
	str >> u;
	return u;
}

