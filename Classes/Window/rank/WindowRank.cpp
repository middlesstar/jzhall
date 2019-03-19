#include "WindowRank.h"

WindowRank::WindowRank()
{

}

WindowRank::~WindowRank()
{
}

void WindowRank::setContent()
{
	Node *pMainNode = _loadCSBLayoutFile("layout/JZMoneyTree.csb");
	CCASSERT(pMainNode, "WindowRank::setContent() layout/JZMoneyTree.json load nullptr!");

	m_pNode->addChild(pMainNode);
}